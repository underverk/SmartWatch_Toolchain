// Davey Taylor, Arduino Verkstad AB
// Platform configuration and Arduino internals

#include <stm32f2xx_gpio.h>
#include <stm32f2xx_rcc.h>

#include "pins.h"

// Common pin modes
//             NAME              MODE           SPEED             OUTPUT TYPE    PULL-UP/DOWN      ALTERNATE     SET STATE
const PinCfg_t pincfg_in       = {GPIO_Mode_IN,  GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_NOPULL, 0,            NO_CHANGE};
const PinCfg_t pincfg_in_pd    = {GPIO_Mode_IN,  GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_DOWN,   0,            NO_CHANGE};
const PinCfg_t pincfg_in_pu    = {GPIO_Mode_IN,  GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_UP,     0,            NO_CHANGE};
const PinCfg_t pincfg_ain      = {GPIO_Mode_AIN, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, 0,            NO_CHANGE};
const PinCfg_t pincfg_out_od   = {GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_NOPULL, 0,            NO_CHANGE};
const PinCfg_t pincfg_out_od_s = {GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_NOPULL, 0,            HIGH     };
const PinCfg_t pincfg_out_od_r = {GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_NOPULL, 0,            LOW      };
const PinCfg_t pincfg_out      = {GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, 0,            NO_CHANGE};
const PinCfg_t pincfg_out_s    = {GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, 0,            HIGH     };
const PinCfg_t pincfg_out_r    = {GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, 0,            LOW      };
const PinCfg_t pincfg_af_spi1  = {GPIO_Mode_AF,  GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_AF_SPI1, LOW      };
const PinCfg_t pincfg_af_i2c1  = {GPIO_Mode_AF,  GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_AF_I2C1, LOW      };
const PinCfg_t pincfg_af_mco1  = {GPIO_Mode_AF,  GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, 0,            NO_CHANGE};

// Pins and their default modes
//             NAME        PORT   PIN(MASK)    PIN(NUMBER)       AHB CLOCK             DEFAULT MODE        SONY'S NAME
const PinDef_t PIN_PA0  = {GPIOA, GPIO_Pin_0,  GPIO_PinSource0,  RCC_AHB1Periph_GPIOA, INPUT};          // WKUP_Pin
const PinDef_t PIN_PA1  = {GPIOA, GPIO_Pin_1,  GPIO_PinSource1,  RCC_AHB1Periph_GPIOA, OUTPUT_LOW};     // BATT_V_ON_Pin
const PinDef_t PIN_PA2  = {GPIOA, GPIO_Pin_2,  GPIO_PinSource2,  RCC_AHB1Periph_GPIOA, ANALOG_INPUT};   // BAT_V_Pin
const PinDef_t PIN_PA4  = {GPIOA, GPIO_Pin_4,  GPIO_PinSource4,  RCC_AHB1Periph_GPIOA, OUTPUT_HIGH};    // SPI1_nSS_Pin
const PinDef_t PIN_PA5  = {GPIOA, GPIO_Pin_5,  GPIO_PinSource5,  RCC_AHB1Periph_GPIOA, AF_SPI1};        // SPI1_SCK_Pin
const PinDef_t PIN_PA7  = {GPIOA, GPIO_Pin_7,  GPIO_PinSource7,  RCC_AHB1Periph_GPIOA, AF_SPI1};        // SPI1_MOSI_Pin
const PinDef_t PIN_PA8  = {GPIOA, GPIO_Pin_8,  GPIO_PinSource8,  RCC_AHB1Periph_GPIOA, AF_MCO1};        // MCO1_Pin
const PinDef_t PIN_PA9  = {GPIOA, GPIO_Pin_9,  GPIO_PinSource9,  RCC_AHB1Periph_GPIOA, INPUT_PULLDOWN}; // USB5V_Detected_Pin
const PinDef_t PIN_PA11 = {GPIOA, GPIO_Pin_11, GPIO_PinSource11, RCC_AHB1Periph_GPIOA, INPUT};          // USB_DM_Pin
const PinDef_t PIN_PA12 = {GPIOA, GPIO_Pin_12, GPIO_PinSource12, RCC_AHB1Periph_GPIOA, INPUT};          // USB_DP_Pin
const PinDef_t PIN_PA15 = {GPIOA, GPIO_Pin_15, GPIO_PinSource15, RCC_AHB1Periph_GPIOA, OUTPUT_OD_LOW};  // Reset_BT_Pin
const PinDef_t PIN_PB0  = {GPIOB, GPIO_Pin_0,  GPIO_PinSource0,  RCC_AHB1Periph_GPIOB, OUTPUT_LOW};     // Touch_Reset_Pin
const PinDef_t PIN_PB1  = {GPIOB, GPIO_Pin_1,  GPIO_PinSource1,  RCC_AHB1Periph_GPIOB, OUTPUT_OD_LOW};  // Connect_USB_Pin
const PinDef_t PIN_PB2  = {GPIOB, GPIO_Pin_2,  GPIO_PinSource2,  RCC_AHB1Periph_GPIOB, OUTPUT_LOW};     // POWER_3V3_ON_1V8_OFF_Pin
const PinDef_t PIN_PB3  = {GPIOB, GPIO_Pin_3,  GPIO_PinSource3,  RCC_AHB1Periph_GPIOB, OUTPUT_LOW};     // ENABLE_2V8_Pin
const PinDef_t PIN_PB4  = {GPIOB, GPIO_Pin_4,  GPIO_PinSource4,  RCC_AHB1Periph_GPIOB, OUTPUT_LOW};     // ENABLE_LIGHT_SENSOR_Pin
const PinDef_t PIN_PB5  = {GPIOB, GPIO_Pin_5,  GPIO_PinSource5,  RCC_AHB1Periph_GPIOB, OUTPUT_LOW};     // OLED_Reset_Pin
const PinDef_t PIN_PB6  = {GPIOB, GPIO_Pin_6,  GPIO_PinSource6,  RCC_AHB1Periph_GPIOB, AF_I2C1};        // I2C_SCL_Pin  
const PinDef_t PIN_PB7  = {GPIOB, GPIO_Pin_7,  GPIO_PinSource7,  RCC_AHB1Periph_GPIOB, AF_I2C1};        // I2C_SDA_Pin
const PinDef_t PIN_PB8  = {GPIOB, GPIO_Pin_8,  GPIO_PinSource8,  RCC_AHB1Periph_GPIOB, OUTPUT_LOW};     // BUZER_PWM_Pin
const PinDef_t PIN_PB9  = {GPIOB, GPIO_Pin_9,  GPIO_PinSource9,  RCC_AHB1Periph_GPIOB, INPUT_PULLDOWN}; // ACCELEROMETER_INT_Pin
const PinDef_t PIN_PB10 = {GPIOB, GPIO_Pin_10, GPIO_PinSource10, RCC_AHB1Periph_GPIOB, INPUT};          // BT_CLK_REQ_Pin
const PinDef_t PIN_PB11 = {GPIOB, GPIO_Pin_11, GPIO_PinSource11, RCC_AHB1Periph_GPIOB, INPUT};          // POWER_BTN_PRESS_Pin
const PinDef_t PIN_PB12 = {GPIOB, GPIO_Pin_12, GPIO_PinSource12, RCC_AHB1Periph_GPIOB, OUTPUT_HIGH};    // SPI2_nSS_Pin
const PinDef_t PIN_PB13 = {GPIOB, GPIO_Pin_13, GPIO_PinSource13, RCC_AHB1Periph_GPIOB, OUTPUT_LOW};     // SPI2_SCK_Pin
const PinDef_t PIN_PB14 = {GPIOB, GPIO_Pin_14, GPIO_PinSource14, RCC_AHB1Periph_GPIOB, INPUT_PULLDOWN}; // SPI2_MISO_Pin
const PinDef_t PIN_PB15 = {GPIOB, GPIO_Pin_15, GPIO_PinSource15, RCC_AHB1Periph_GPIOB, OUTPUT_LOW};     // SPI2_MOSI_Pin
const PinDef_t PIN_PC0  = {GPIOC, GPIO_Pin_0,  GPIO_PinSource0,  RCC_AHB1Periph_GPIOC, OUTPUT_LOW};     // OLED_A0_Pin
const PinDef_t PIN_PC1  = {GPIOC, GPIO_Pin_1,  GPIO_PinSource1,  RCC_AHB1Periph_GPIOC, OUTPUT_LOW};     // OLED_V_ENABLE_Pin
const PinDef_t PIN_PC3  = {GPIOC, GPIO_Pin_3,  GPIO_PinSource3,  RCC_AHB1Periph_GPIOC, OUTPUT_OD_HIGH}; // HoldPower_Pin
const PinDef_t PIN_PC4  = {GPIOC, GPIO_Pin_4,  GPIO_PinSource4,  RCC_AHB1Periph_GPIOC, ANALOG_INPUT};   // LIGHT_SENSOR_ANALOG_OUT_Pin
const PinDef_t PIN_PC6  = {GPIOC, GPIO_Pin_6,  GPIO_PinSource6,  RCC_AHB1Periph_GPIOC, INPUT};          // TOUCH_WKUP_INT_Pin
const PinDef_t PIN_PC7  = {GPIOC, GPIO_Pin_7,  GPIO_PinSource7,  RCC_AHB1Periph_GPIOC, OUTPUT_OD_HIGH}; // Enable_1V8_BT_Power_Pin
const PinDef_t PIN_PC9  = {GPIOC, GPIO_Pin_9,  GPIO_PinSource9,  RCC_AHB1Periph_GPIOC, OUTPUT_LOW};     // SPI3_nSS_Pin
const PinDef_t PIN_PC10 = {GPIOC, GPIO_Pin_10, GPIO_PinSource10, RCC_AHB1Periph_GPIOC, OUTPUT_LOW};     // SPI3_SCK_Pin
const PinDef_t PIN_PC11 = {GPIOC, GPIO_Pin_11, GPIO_PinSource11, RCC_AHB1Periph_GPIOC, INPUT_PULLDOWN}; // SPI3_MISO_Pin
const PinDef_t PIN_PC12 = {GPIOC, GPIO_Pin_12, GPIO_PinSource12, RCC_AHB1Periph_GPIOC, OUTPUT_LOW};     // SPI3_MOSI_Pin
const PinDef_t PIN_PC13 = {GPIOC, GPIO_Pin_13, GPIO_PinSource13, RCC_AHB1Periph_GPIOC, INPUT};          // HOST_WAKE_UP_Pin

// Changes the "mode"/configuration of a pin
void pinMode(const PinDef_t *pin, const PinCfg_t *cfg) {
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
void initializePins() {
  const PinDef_t * pins[38] = {
    &PIN_PA0, &PIN_PA1, &PIN_PA2, &PIN_PA4, &PIN_PA5, &PIN_PA7, &PIN_PA8, &PIN_PA9,
    &PIN_PA11, &PIN_PA12, &PIN_PA15, &PIN_PB0, &PIN_PB1, &PIN_PB2, &PIN_PB3,
    &PIN_PB4, &PIN_PB5, &PIN_PB6, &PIN_PB7, &PIN_PB8, &PIN_PB9, &PIN_PB10,
    &PIN_PB11, &PIN_PB12, &PIN_PB13, &PIN_PB14, &PIN_PB15, &PIN_PC0,
    &PIN_PC1, &PIN_PC3, &PIN_PC4, &PIN_PC6, &PIN_PC7, &PIN_PC9,
    &PIN_PC10, &PIN_PC11, &PIN_PC12, &PIN_PC13
  };
  for(uint8_t n = 0; n < 38; n++) {
    pinMode(pins[n], pins[n]->cfg);
  }
}

// Writes digital output
void digitalWrite(const PinDef_t *pin, uint8_t value) {
  GPIO_WriteBit(pin->port, pin->pin, value == HIGH ? Bit_SET : Bit_RESET);
}

// Reads digital input
uint8_t digitalRead(const PinDef_t *pin) {
  return GPIO_ReadInputDataBit(pin->port, pin->pin) ? HIGH : LOW;
}

static volatile uint32_t ticks;
volatile uint16_t int_ctr;

uint32_t millis() {
  uint32_t copy;
  cli();
  copy = ticks;
  sei();
  return copy;
}

// Soft loop, cpu speed dependent
void delay_loop(uint32_t volatile n) {
  while(n--);  
}

// Delay
void delay(uint32_t us) {
  uint32_t start = millis();
  while((millis() - start) < us);
}

__attribute__ ((interrupt ("IRQ")))
void SysTick_Handler(void) {
  ticks++;
}

