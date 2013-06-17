// Davey Taylor, Arduino Verkstad AB
// Power Management Driver

#include <stm32f2xx_rcc.h>
#include <stm32f2xx_pwr.h>
#include <stm32f2xx_flash.h>
#include "pins.h"
#include "driver_power.h"
#include "driver_i2c.h"
#include "pmu_defines.h"

#define MHZ (HSE_VALUE / 1000000) // External crystal speed in Mhz

const ClockDef_t sysclock_3m25 = {  3250000, 0,     0, 0, 0, 0, RCC_SYSCLK_Div8, RCC_HCLK_Div1, RCC_HCLK_Div1}; //    3.250 MHz
const ClockDef_t sysclock_15m  = { 15000000, MHZ, 240, 2, 5, 0, RCC_SYSCLK_Div8, RCC_HCLK_Div1, RCC_HCLK_Div1}; //   15.000 Mhz with PLL
const ClockDef_t sysclock_26m  = { 26000000, 0,     0, 0, 0, 1, RCC_SYSCLK_Div1, RCC_HCLK_Div1, RCC_HCLK_Div1}; //   26.000 MHz
const ClockDef_t sysclock_120m = {120000000, MHZ, 240, 2, 5, 7, RCC_SYSCLK_Div1, RCC_HCLK_Div2, RCC_HCLK_Div4}; //  120.000 MHz with PLL 

void cpu_reclock(const ClockDef_t *clk) {
  // Disable interrupts
  cli();
  
  // Switch to HSE (as changing PLL while using it is asking for trouble)
  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);
  while(RCC_GetSYSCLKSource() != (uint32_t)RCC_CFGR_SWS_HSE); // Wait for completion

  // Disable PLL
  RCC_PLLCmd(DISABLE);

  // Set HCLK & PCLK from table
  RCC_HCLKConfig(clk->hps);
  RCC_PCLK2Config(clk->pps2);
  RCC_PCLK1Config(clk->pps1);

  // Set flash latency from table
  FLASH_SetLatency(clk->f_latency);

  // Use PLL?
  if(clk->m) {
    // Configure PLL
    RCC_PLLConfig(RCC_PLLCFGR_PLLSRC_HSE, clk->m, clk->n, clk->p, clk->q);
    // Enable PLL
    RCC_PLLCmd(ENABLE);
    // Wait for PLL to become stable
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    // Switch to PLL
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while (RCC_GetSYSCLKSource() != (uint32_t)RCC_CFGR_SWS_PLL); // Wait for completion
  }

  // Update system tick
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock / 1000);

  sei();
}

void cpu_init(void) {
  RCC_DeInit();

  RCC_HSEConfig(RCC_HSE_ON);
  ErrorStatus HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS) {
    
    // Enable flash buffers and prefetching
    FLASH_PrefetchBufferCmd(ENABLE);
    FLASH_InstructionCacheCmd(ENABLE);
    FLASH_DataCacheCmd(ENABLE);

    // Set startup speed
    cpu_reclock(&sysclock_120m);
  }
}

static bool pmu_init_ok;

// Initializes PMU (Power Management Unit)
bool pmu_init(void) {
    // Only initialize once
    if(pmu_init_ok) return false;
    
    // Write default configuration
    uint8_t config0 = VSYS_4_4V | ACIC_100mA_DPPM_ENABLE | TH_LOOP | DYN_TMR | TERM_EN | CH_EN;
    uint8_t config1 = I_TERM_10 | ISET_100 | I_PRE_10;
    uint8_t config2 = SFTY_TMR_5h | PRE_TMR_30m | NTC_10k | V_DPPM_4_3_V | VBAT_COMP_ENABLE;
    uint8_t defdcdc = DCDC_DISCH | DCDC1_DEFAULT;
    if(i2c_write(PMU_ADDRESS, CHGCONFIG0, &config0, 1)) return false;
    if(i2c_write(PMU_ADDRESS, CHGCONFIG1, &config1, 1)) return false;
    if(i2c_write(PMU_ADDRESS, CHGCONFIG2, &config2, 1)) return false;
    if(i2c_write(PMU_ADDRESS, DEFDCDC,    &defdcdc, 1)) return false;
      
    // Success
    return pmu_init_ok = true;
}

// Enables or disables charging of the battery
// Enabling ALLOWS the PMU to charge the battery, to find out if it actually IS
// actively charging the battery, you need to call pmu_charging.
bool pmu_charge(bool enable) {
  // Only if initialized
  if(!pmu_init_ok) return false;
  
  // Read current configuration
  uint8_t config0;
  if(i2c_read(PMU_ADDRESS, CHGSTATUS, &config0, 1)) return false;
    
  // Set/clear charge enable and write configuration
  config0 = enable ? (config0 | CH_EN) : (config0 & ~CH_EN);
  if(i2c_write(PMU_ADDRESS, CHGCONFIG0, &config0, 1)) return false;
    
  // Success
  return true;
}

// Checks if the PMU is actively charging the battery
bool pmu_charging(bool *charging) {
  if(!pmu_init_ok) return false;

  static bool last_charging;
  uint8_t config0, status;
  
  *charging = last_charging;
  if(i2c_read(PMU_ADDRESS, CHGCONFIG0, &config0, 1)) return false;
  if(i2c_read(PMU_ADDRESS, CHGSTATUS, &status, 1)) return false;
 
  *charging = last_charging = (status & CH_ACTIVE_MSK) != 0;
  return true;
}