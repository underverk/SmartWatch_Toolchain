#include <stdint.h>
#include <stdbool.h>
#include "system.h"
#include "driver_adc.h"
#include "driver_i2c.h"
#include "driver_power.h"
#include "driver_vcp.h"

#define SET 1
#define CLEAR 0
#define NO_CHANGE -1

// Common pin modes
//             NAME              MODE           SPEED             OUTPUT TYPE    PULL-UP/DOWN      ALTERNATE     SET STATE
const PinCfg_t pincfg_in       = {GPIO_Mode_IN,  GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_NOPULL, 0,            NO_CHANGE};
const PinCfg_t pincfg_in_pd    = {GPIO_Mode_IN,  GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_DOWN,   0,            NO_CHANGE};
const PinCfg_t pincfg_in_pu    = {GPIO_Mode_IN,  GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_UP,     0,            NO_CHANGE};
const PinCfg_t pincfg_ain      = {GPIO_Mode_AIN, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, 0,            NO_CHANGE};
const PinCfg_t pincfg_out_od   = {GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_NOPULL, 0,            NO_CHANGE};
const PinCfg_t pincfg_out_od_s = {GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_NOPULL, 0,            SET      };
const PinCfg_t pincfg_out_od_c = {GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_NOPULL, 0,            CLEAR    };
const PinCfg_t pincfg_out      = {GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, 0,            NO_CHANGE};
const PinCfg_t pincfg_out_s    = {GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, 0,            SET      };
const PinCfg_t pincfg_out_c    = {GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, 0,            CLEAR    };
const PinCfg_t pincfg_af_spi1  = {GPIO_Mode_AF,  GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_AF_SPI1, CLEAR    };
const PinCfg_t pincfg_af_i2c1  = {GPIO_Mode_AF,  GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_AF_I2C1, CLEAR    };
const PinCfg_t pincfg_af_mco1  = {GPIO_Mode_AF,  GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, 0,            NO_CHANGE};

// Pins and their default modes
//             NAME        PORT   PIN(MASK)    PIN(NUMBER)       AHB CLOCK             DEFAULT MODE        SONY'S NAME
const PinDef_t PIN_PA0  = {GPIOA, GPIO_Pin_0,  GPIO_PinSource0,  RCC_AHB1Periph_GPIOA, &pincfg_in      }; // WKUP_Pin
const PinDef_t PIN_PA1  = {GPIOA, GPIO_Pin_1,  GPIO_PinSource1,  RCC_AHB1Periph_GPIOA, &pincfg_out_c   }; // BATT_V_ON_Pin
const PinDef_t PIN_PA2  = {GPIOA, GPIO_Pin_2,  GPIO_PinSource2,  RCC_AHB1Periph_GPIOA, &pincfg_ain     }; // BAT_V_Pin
const PinDef_t PIN_PA4  = {GPIOA, GPIO_Pin_4,  GPIO_PinSource4,  RCC_AHB1Periph_GPIOA, &pincfg_out_s   }; // SPI1_nSS_Pin
const PinDef_t PIN_PA5  = {GPIOA, GPIO_Pin_5,  GPIO_PinSource5,  RCC_AHB1Periph_GPIOA, &pincfg_af_spi1 }; // SPI1_SCK_Pin
const PinDef_t PIN_PA7  = {GPIOA, GPIO_Pin_7,  GPIO_PinSource7,  RCC_AHB1Periph_GPIOA, &pincfg_af_spi1 }; // SPI1_MOSI_Pin
const PinDef_t PIN_PA8  = {GPIOA, GPIO_Pin_8,  GPIO_PinSource8,  RCC_AHB1Periph_GPIOA, &pincfg_af_mco1 }; // MCO1_Pin
const PinDef_t PIN_PA9  = {GPIOA, GPIO_Pin_9,  GPIO_PinSource9,  RCC_AHB1Periph_GPIOA, &pincfg_in_pd   }; // USB5V_Detected_Pin
const PinDef_t PIN_PA11 = {GPIOA, GPIO_Pin_11, GPIO_PinSource11, RCC_AHB1Periph_GPIOA, &pincfg_in      }; // USB_DM_Pin
const PinDef_t PIN_PA12 = {GPIOA, GPIO_Pin_12, GPIO_PinSource12, RCC_AHB1Periph_GPIOA, &pincfg_in      }; // USB_DP_Pin
const PinDef_t PIN_PA15 = {GPIOA, GPIO_Pin_15, GPIO_PinSource15, RCC_AHB1Periph_GPIOA, &pincfg_out_od_c}; // Reset_BT_Pin
const PinDef_t PIN_PB0  = {GPIOB, GPIO_Pin_0,  GPIO_PinSource0,  RCC_AHB1Periph_GPIOB, &pincfg_out_c   }; // Touch_Reset_Pin
const PinDef_t PIN_PB1  = {GPIOB, GPIO_Pin_1,  GPIO_PinSource1,  RCC_AHB1Periph_GPIOB, &pincfg_out_od_c}; // Connect_USB_Pin
const PinDef_t PIN_PB2  = {GPIOB, GPIO_Pin_2,  GPIO_PinSource2,  RCC_AHB1Periph_GPIOB, &pincfg_out_c   }; // POWER_3V3_ON_1V8_OFF_Pin
const PinDef_t PIN_PB3  = {GPIOB, GPIO_Pin_3,  GPIO_PinSource3,  RCC_AHB1Periph_GPIOB, &pincfg_out_c   }; // ENABLE_2V8_Pin
const PinDef_t PIN_PB4  = {GPIOB, GPIO_Pin_4,  GPIO_PinSource4,  RCC_AHB1Periph_GPIOB, &pincfg_out_c   }; // ENABLE_LIGHT_SENSOR_Pin
const PinDef_t PIN_PB5  = {GPIOB, GPIO_Pin_5,  GPIO_PinSource5,  RCC_AHB1Periph_GPIOB, &pincfg_out_c   }; // OLED_Reset_Pin
const PinDef_t PIN_PB6  = {GPIOB, GPIO_Pin_6,  GPIO_PinSource6,  RCC_AHB1Periph_GPIOB, &pincfg_af_i2c1 }; // I2C_SCL_Pin  
const PinDef_t PIN_PB7  = {GPIOB, GPIO_Pin_7,  GPIO_PinSource7,  RCC_AHB1Periph_GPIOB, &pincfg_af_i2c1 }; // I2C_SDA_Pin
const PinDef_t PIN_PB8  = {GPIOB, GPIO_Pin_8,  GPIO_PinSource8,  RCC_AHB1Periph_GPIOB, &pincfg_out_c   }; // BUZER_PWM_Pin
const PinDef_t PIN_PB9  = {GPIOB, GPIO_Pin_9,  GPIO_PinSource9,  RCC_AHB1Periph_GPIOB, &pincfg_in_pd   }; // ACCELEROMETER_INT_Pin
const PinDef_t PIN_PB10 = {GPIOB, GPIO_Pin_10, GPIO_PinSource10, RCC_AHB1Periph_GPIOB, &pincfg_in      }; // BT_CLK_REQ_Pin
const PinDef_t PIN_PB11 = {GPIOB, GPIO_Pin_11, GPIO_PinSource11, RCC_AHB1Periph_GPIOB, &pincfg_in      }; // POWER_BTN_PRESS_Pin
const PinDef_t PIN_PB12 = {GPIOB, GPIO_Pin_12, GPIO_PinSource12, RCC_AHB1Periph_GPIOB, &pincfg_out_s   }; // SPI2_nSS_Pin
const PinDef_t PIN_PB13 = {GPIOB, GPIO_Pin_13, GPIO_PinSource13, RCC_AHB1Periph_GPIOB, &pincfg_out_c   }; // SPI2_SCK_Pin
const PinDef_t PIN_PB14 = {GPIOB, GPIO_Pin_14, GPIO_PinSource14, RCC_AHB1Periph_GPIOB, &pincfg_in_pd   }; // SPI2_MISO_Pin
const PinDef_t PIN_PB15 = {GPIOB, GPIO_Pin_15, GPIO_PinSource15, RCC_AHB1Periph_GPIOB, &pincfg_out_c   }; // SPI2_MOSI_Pin
const PinDef_t PIN_PC0  = {GPIOC, GPIO_Pin_0,  GPIO_PinSource0,  RCC_AHB1Periph_GPIOC, &pincfg_out_c   }; // OLED_A0_Pin
const PinDef_t PIN_PC1  = {GPIOC, GPIO_Pin_1,  GPIO_PinSource1,  RCC_AHB1Periph_GPIOC, &pincfg_out_c   }; // OLED_V_ENABLE_Pin
const PinDef_t PIN_PC3  = {GPIOC, GPIO_Pin_3,  GPIO_PinSource3,  RCC_AHB1Periph_GPIOC, &pincfg_out_od_s}; // HoldPower_Pin
const PinDef_t PIN_PC4  = {GPIOC, GPIO_Pin_4,  GPIO_PinSource4,  RCC_AHB1Periph_GPIOC, &pincfg_ain     }; // LIGHT_SENSOR_ANALOG_OUT_Pin
const PinDef_t PIN_PC6  = {GPIOC, GPIO_Pin_6,  GPIO_PinSource6,  RCC_AHB1Periph_GPIOC, &pincfg_in      }; // TOUCH_WKUP_INT_Pin
const PinDef_t PIN_PC7  = {GPIOC, GPIO_Pin_7,  GPIO_PinSource7,  RCC_AHB1Periph_GPIOC, &pincfg_out_od_s}; // Enable_1V8_BT_Power_Pin
const PinDef_t PIN_PC9  = {GPIOC, GPIO_Pin_9,  GPIO_PinSource9,  RCC_AHB1Periph_GPIOC, &pincfg_out_c   }; // SPI3_nSS_Pin
const PinDef_t PIN_PC10 = {GPIOC, GPIO_Pin_10, GPIO_PinSource10, RCC_AHB1Periph_GPIOC, &pincfg_out_c   }; // SPI3_SCK_Pin
const PinDef_t PIN_PC11 = {GPIOC, GPIO_Pin_11, GPIO_PinSource11, RCC_AHB1Periph_GPIOC, &pincfg_in_pd   }; // SPI3_MISO_Pin
const PinDef_t PIN_PC12 = {GPIOC, GPIO_Pin_12, GPIO_PinSource12, RCC_AHB1Periph_GPIOC, &pincfg_out_c   }; // SPI3_MOSI_Pin
const PinDef_t PIN_PC13 = {GPIOC, GPIO_Pin_13, GPIO_PinSource13, RCC_AHB1Periph_GPIOC, &pincfg_in      }; // HOST_WAKE_UP_Pin

static volatile uint32_t tick_ctr; // System tick count
volatile uint16_t int_ctr;      // Interrupt nesting count

// The Arduino system tick
//__attribute__ ((interrupt ("IRQ")))
void SysTick_Handler(void) {
  tick_ctr++;
}

// Changes the "mode"/configuration of a pin
void pin_mode(const PinDef_t *pin, const PinCfg_t *cfg) {
  GPIO_InitTypeDef ConfigPin;
  ConfigPin.GPIO_Pin   = pin->pin;
  ConfigPin.GPIO_Mode  = cfg->mode;
  ConfigPin.GPIO_Speed = cfg->speed;
  ConfigPin.GPIO_OType = cfg->type;
  ConfigPin.GPIO_PuPd  = cfg->pupd;
  GPIO_Init(pin->port, &ConfigPin);
  if(cfg->mode == GPIO_Mode_AF) {
    GPIO_PinAFConfig(pin->port,  pin->source, cfg->af);
  } else if(cfg->set != NO_CHANGE) {
    GPIO_WriteBit(pin->port, pin->pin, cfg->set ? Bit_SET : Bit_RESET);
  }
}

// Initializes all pins to their default configuration
void pins_init(void) {
  const PinDef_t * pins[38] = {
    &PIN_PA0, &PIN_PA1, &PIN_PA2, &PIN_PA4, &PIN_PA5, &PIN_PA7, &PIN_PA8, &PIN_PA9,
    &PIN_PA11, &PIN_PA12, &PIN_PA15, &PIN_PB0, &PIN_PB1, &PIN_PB2, &PIN_PB3,
    &PIN_PB4, &PIN_PB5, &PIN_PB6, &PIN_PB7, &PIN_PB8, &PIN_PB9, &PIN_PB10,
    &PIN_PB11, &PIN_PB12, &PIN_PB13, &PIN_PB14, &PIN_PB15, &PIN_PC0,
    &PIN_PC1, &PIN_PC3, &PIN_PC4, &PIN_PC6, &PIN_PC7, &PIN_PC9,
    &PIN_PC10, &PIN_PC11, &PIN_PC12, &PIN_PC13
  };
  for(uint8_t n = 0; n < 38; n++) {
    pin_mode(pins[n], pins[n]->cfg);
  }
}

// Writes digital pin
void pin_write(const PinDef_t *pin, bool value) {
  GPIO_WriteBit(pin->port, pin->pin, value ? Bit_SET : Bit_RESET);
}

// Sets digital pin
void pin_set(const PinDef_t *pin) {
  GPIO_WriteBit(pin->port, pin->pin, Bit_SET);
}

// Clears digital pin
void pin_clear(const PinDef_t *pin) {
  GPIO_WriteBit(pin->port, pin->pin, Bit_RESET);
}

// Reads digital pin
bool pin_read(const PinDef_t *pin) {
  return GPIO_ReadInputDataBit(pin->port, pin->pin) ? true : false;
}

// Uptime in milliseconds
uint32_t ticks(void) {
  uint32_t copy;
  di();
  copy = tick_ctr;
  ei();
  return copy;
}

// Reads devices unique ID
// TODO: Verify if working!
// id = 0: Low    32-bits of 96-bit uID
// id = 1: Middle 32-bits of 96-bit uID
// id = 2: High   32-bits of 96-bit uID
uint32_t sys_id(uint8_t id) {
  if(id > 2) return 0;
  return *((uint32_t*)(0x1FFF7A10L + (4L << id)));
}

// Delay milliseconds
void delay(uint32_t ms) {
  uint32_t start = ticks();
  while(((uint32_t)(ticks() - start)) < ms);
}

// USB pins
#define USB_DP        (&PIN_PA12)
#define USB_DM        (&PIN_PA11)


// This can be called from anywhere and is good to use as a test
// if your clock does not boot, to find out exactly how far the
// startup procedure reaches. The startup procedure begins in
// libstm32f2/startup_stm32f2xx.S at Reset_Handler:
// To call this function from assembly, use "bl buzztest"
/*
void buzztest(void) {
  pin_mode(BUZZER, BUZZER->cfg);
  pin_set(BUZZER);  
}
*/

void fault_default(void) {
  pin_mode(BUZZER, BUZZER->cfg);
  pin_set(BUZZER); 
  pin_mode(POWER, POWER->cfg);
  pin_clear(POWER); 
  while(1);
}

// System initialization
void sys_init(void) {
  
  // Initializes all pins to their default settings (see pins.c)
  pins_init();

  // Initialize system clocks
  cpu_init();

  // Start system tick (used for timing, delay, etc)
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock / 1000);

  // Tip from Sony:
  // Prevents hard-faults when booting from USB
  delay(50);

  // Tip from Sony:
  // Not quite sure, but I believe a pullup on DP enables charging of a device even if
  // it does not do USB any communication by removing the pre-enum current limit
  if(pin_read(USB_CONNECTED)) pin_mode(USB_DP, &pincfg_in_pu);

  // Initialize ADC
  adc_init();

  // Initialize I2C
  i2c_init();
  
  __asm volatile ("cpsie i");
    
}