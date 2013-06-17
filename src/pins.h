// Davey Taylor, Arduino Verkstad AB
// Platform configuration and Arduino internals

#ifndef _PINS_H_
#define _PINS_H_
#include "stm32f2xx_gpio.h"
#include <stdint.h>
#include <stdbool.h>

#define HIGH 1
#define LOW 0
#define NO_CHANGE -1

typedef struct {
  GPIOMode_TypeDef  mode;  // operating mode @ref GPIOMode_TypeDef
  GPIOSpeed_TypeDef speed; // speed @ref GPIOSpeed_TypeDef
  GPIOOType_TypeDef type;  // output type @ref GPIOOType_TypeDef
  GPIOPuPd_TypeDef  pupd;  // pull-up/down @ref GPIOPuPd_TypeDef
  uint8_t af;              // alternate function
  int16_t set;             // pin setting
} PinCfg_t;

typedef struct {
  GPIO_TypeDef *port; // Pin port
  uint32_t pin;       // Pin mask @ref GPIO_pins_define
  uint8_t  source;    // Pin number @ref GPIO_Pin_sources 
  uint32_t clock;     // clock source
  const PinCfg_t *cfg;      // Default configuration
} PinDef_t;

extern const PinCfg_t INPUT;
extern const PinCfg_t INPUT_PULLDOWN;
extern const PinCfg_t INPUT_PULLUP;
extern const PinCfg_t ANALOG_INPUT;
extern const PinCfg_t OUTPUT;
extern const PinCfg_t OUTPUT_HIGH;
extern const PinCfg_t OUTPUT_LOW;
extern const PinCfg_t AF_SPI1;
extern const PinCfg_t AF_MCO1;

extern const PinDef_t PIN_PA0 ;
extern const PinDef_t PIN_PA1 ;
extern const PinDef_t PIN_PA2 ;
extern const PinDef_t PIN_PA4 ;
extern const PinDef_t PIN_PA5 ;
extern const PinDef_t PIN_PA7 ;
extern const PinDef_t PIN_PA8 ;
extern const PinDef_t PIN_PA9 ;
extern const PinDef_t PIN_PA11;
extern const PinDef_t PIN_PA12;
extern const PinDef_t PIN_PA15;
extern const PinDef_t PIN_PB0 ;
extern const PinDef_t PIN_PB1 ;
extern const PinDef_t PIN_PB2 ;
extern const PinDef_t PIN_PB3 ;
extern const PinDef_t PIN_PB4 ;
extern const PinDef_t PIN_PB5 ;
extern const PinDef_t PIN_PB6 ;
extern const PinDef_t PIN_PB7 ;
extern const PinDef_t PIN_PB8 ;
extern const PinDef_t PIN_PB9 ;
extern const PinDef_t PIN_PB10;
extern const PinDef_t PIN_PB11;
extern const PinDef_t PIN_PB12;
extern const PinDef_t PIN_PB13;
extern const PinDef_t PIN_PB14;
extern const PinDef_t PIN_PB15; 
extern const PinDef_t PIN_PC0 ;
extern const PinDef_t PIN_PC1 ;
extern const PinDef_t PIN_PC3 ;
extern const PinDef_t PIN_PC4 ;
extern const PinDef_t PIN_PC6 ;
extern const PinDef_t PIN_PC7 ;
extern const PinDef_t PIN_PC9 ;
extern const PinDef_t PIN_PC10;
extern const PinDef_t PIN_PC11;
extern const PinDef_t PIN_PC12; 
extern const PinDef_t PIN_PC13;

void pinMode(const PinDef_t *pin, const PinCfg_t *cfg);
void initializePins(void);
void digitalWrite(const PinDef_t *pin, uint8_t value);
uint8_t digitalRead(const PinDef_t *pin);

extern volatile uint16_t int_ctr;

// Nested interrupt enable
__attribute__( ( always_inline ) ) __STATIC_INLINE void cli(void) {
  __ASM volatile ("cpsid i");
  int_ctr++;
}
// Nested interrupt disable
__attribute__( ( always_inline ) ) __STATIC_INLINE void sei(void) {
  __ASM volatile ("cpsid i");
  if(!--int_ctr) {__ASM volatile ("cpsie i");}
}

extern const PinCfg_t pincfg_in      ;
extern const PinCfg_t pincfg_in_pd   ;
extern const PinCfg_t pincfg_in_pu   ;
extern const PinCfg_t pincfg_ain     ;
extern const PinCfg_t pincfg_out_od  ;
extern const PinCfg_t pincfg_out_od_s;
extern const PinCfg_t pincfg_out_od_r;
extern const PinCfg_t pincfg_out     ;
extern const PinCfg_t pincfg_out_s   ;
extern const PinCfg_t pincfg_out_r   ;
extern const PinCfg_t pincfg_af_spi1 ;
extern const PinCfg_t pincfg_af_i2c1 ;
extern const PinCfg_t pincfg_af_mco1 ;


#define INPUT          (&pincfg_in)
#define INPUT_PULLDOWN (&pincfg_in_pd)
#define INPUT_PULLUP   (&pincfg_in_pu)
#define ANALOG_INPUT   (&pincfg_ain)
#define OUTPUT_OD      (&pincfg_out_od)
#define OUTPUT_OD_HIGH (&pincfg_out_od_s)
#define OUTPUT_OD_LOW  (&pincfg_out_od_r)
#define OUTPUT_        (&pincfg_out)
#define OUTPUT_HIGH    (&pincfg_out_s)
#define OUTPUT_LOW     (&pincfg_out_r)
#define AF_SPI1        (&pincfg_af_spi1)
#define AF_I2C1        (&pincfg_af_i2c1)
#define AF_MCO1        (&pincfg_af_mco1)

#define BUZZER        (&PIN_PB8 )
#define BUTTON        (&PIN_PB11)
#define POWER         (&PIN_PC3 )
#define USB_CONNECTED (&PIN_PA9 )
#define USB_DP        (&PIN_PA12)
#define USB_DM        (&PIN_PA11)

#define max(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#define min(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })

void delay_loop(uint32_t);
void delay(uint32_t);
uint32_t millis(void);

typedef bool boolean;
typedef uint16_t word;
typedef uint8_t byte;

#endif