#ifndef _SYSTEM_H_
#define _SYSTEM_H_
#include <stdint.h>
#include <stdbool.h>
#include <stm32f2xx_gpio.h>
#ifdef __cplusplus
extern "C" {
#endif

// Pin configuration (platform specific)
typedef struct {
  GPIOMode_TypeDef  mode;  // operating mode @ref GPIOMode_TypeDef
  GPIOSpeed_TypeDef speed; // speed @ref GPIOSpeed_TypeDef
  GPIOOType_TypeDef type;  // output type @ref GPIOOType_TypeDef
  GPIOPuPd_TypeDef  pupd;  // pull-up/down @ref GPIOPuPd_TypeDef
  uint8_t af;              // alternate function
  int16_t set;             // pin setting
} PinCfg_t;

// Pin definition (platform specific)
typedef struct {
  GPIO_TypeDef *port;      // Pin port
  uint32_t pin;            // Pin mask @ref GPIO_pins_define
  uint8_t  source;         // Pin number @ref GPIO_Pin_sources 
  uint32_t clock;          // clock source
  const PinCfg_t *cfg;     // Default configuration
} PinDef_t;

// All pin modes
extern const PinCfg_t pincfg_in,     pincfg_in_pd,    pincfg_in_pu,    pincfg_ain,
                      pincfg_out_od, pincfg_out_od_s, pincfg_out_od_c, pincfg_out,
                      pincfg_out_s,  pincfg_out_c,    pincfg_af_spi1,  pincfg_af_i2c1,
                      pincfg_af_mco1;


// All system pins
extern const PinDef_t PIN_PA0,  PIN_PA1,  PIN_PA2,  PIN_PA4,  PIN_PA5,  PIN_PA7,  PIN_PA8,  PIN_PA9,
                      PIN_PA11, PIN_PA12, PIN_PA15, PIN_PB0,  PIN_PB1,  PIN_PB2,  PIN_PB3,  PIN_PB4,
                      PIN_PB5,  PIN_PB6,  PIN_PB7,  PIN_PB8,  PIN_PB9,  PIN_PB10, PIN_PB11, PIN_PB12,
                      PIN_PB13, PIN_PB14, PIN_PB15, PIN_PC0,  PIN_PC1,  PIN_PC3,  PIN_PC4,  PIN_PC6,  
                      PIN_PC7,  PIN_PC9,  PIN_PC10, PIN_PC11, PIN_PC12, PIN_PC13;


// Friendly pin names
#define BUZZER          (&PIN_PB8 )
#define BUTTON          (&PIN_PB11)
#define POWER           (&PIN_PC3 )
#define USB_CONNECTED   (&PIN_PA9 )
#define LIGHT_SENSOR    (&PIN_PA9 )
#define BATTERY_VOLTAGE (&PIN_PA9 )

void     sys_init(void);                                     // System initialization
void     pin_mode(const PinDef_t *pin, const PinCfg_t *cfg); // Changes pin mode
void     pins_init(void);                                    // Initializes all pins
void     pin_write(const PinDef_t *pin, bool value);         // Writes digital pin
void     pin_set(const PinDef_t *pin);                       // Sets digital pin
void     pin_clear(const PinDef_t *pin);                     // Clears digital pin
bool     pin_read(const PinDef_t *pin);                      // Reads digital pin
uint32_t ticks(void);                                        // Uptime ticks
uint32_t sys_id(uint8_t id);                                 // Reads devices unique ID
void     delay(uint32_t ms);                                 // Delay milliseconds

extern volatile uint16_t int_ctr;

// Nested interrupt enable
__attribute__( ( always_inline ) ) static __inline void di(void) {
  __asm volatile ("cpsid i");
  int_ctr++;
}
// Nested interrupt disable
__attribute__( ( always_inline ) ) static __inline void ei(void) {
  __asm volatile ("cpsid i");
  if(!--int_ctr) {__asm volatile ("cpsie i");}
}

// Good to have around, but not part of our toolchain where it should be
#define htons(a) ({ __typeof__ (a) _a = (a); ((_a>>8) & 0xff) | ((_a<<8) & 0xff00); })
#define ntohs(a) ({ __typeof__ (a) _a = (a); ((_a>>8) & 0xff) | ((_a<<8) & 0xff00); })
#define htonl(a) ({ __typeof__ (a) _a = (a); ((_a>>24) & 0xffL) | ((_a>>8) & 0xff00L) | ((_a<<8) & 0xff0000L) | ((_a<<24) & 0xff000000L); })
#define ntohl(a) ({ __typeof__ (a) _a = (a); ((_a>>24) & 0xffL) | ((_a>>8) & 0xff00L) | ((_a<<8) & 0xff0000L) | ((_a<<24) & 0xff000000L); })

#ifdef __cplusplus
}
#endif
#endif
