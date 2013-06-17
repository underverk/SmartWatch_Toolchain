// Davey Taylor, Arduino Verkstad AB
// Power Management Driver

#ifndef _DRIVER_POWER_H_
#define _DRIVER_POWER_H_
#include <stm32f2xx_rcc.h>
#include <stm32f2xx_pwr.h>
#include <stdbool.h>

// Clock configuration
typedef struct {
  uint32_t freq;      // resulting main system (SYSCLK) frequency
  uint16_t m;         // input divider
  uint16_t n;         // output multiplier
  uint16_t p;         // main system (SYSCLK) clock divider
  uint16_t q;         // peripheral (OTG FS, SDIO, RNG) clocks divider
  uint32_t f_latency; // flash latency
  uint32_t hps;       // hclk prescaler
  uint32_t pps2;      // pclk2 prescaler
  uint32_t pps1;      // pclk1 prescaler
} ClockDef_t;

// Saves battery!
extern const ClockDef_t sysclock_3m25; // "Low Speed"     3.25MHz
// Used for start up
extern const ClockDef_t sysclock_15m;  // "Medium Speed"  15MHz
// Used for normal operation
extern const ClockDef_t sysclock_26m;  // "Default Speed" 26MHz
// Used for time critical operation
extern const ClockDef_t sysclock_120m; // "High Speed"    120MHz

// Important note: it's been said that most peripheral communication require the PLL to be active.
// 15M and 120M modes activate the PLL, the others do not.
// OLED has been verified to work with any setting, even without PLL and it uses SPI.
// BlueTooth, I2C, Touch, Accelerometer may only work with 120M mode (and possibly 15M mode).
// Update this after testing!

#define CPU_LS (&sysclock_3m25)
#define CPU_MS (&sysclock_15m)
#define CPU_DS (&sysclock_26m)
#define CPU_HS (&sysclock_120m)

// == CPU POWER MANAGEMENT == //

void cpu_init(void);                     // Initialize CPU clock
void cpu_reclock(const ClockDef_t *clk); // Change CPU clock speed

// == PMU POWER MANAGEMENT == //

bool pmu_init(void);                     // Initialize Power Management Unit
bool pmu_charge(bool enable);            // Enable (allow) battery charging
bool pmu_charging(bool *charging);       // Check if charging is actually active

#endif