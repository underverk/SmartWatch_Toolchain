
#ifndef _ARDUINO_H_
#define _ARDUINO_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stm32f2xx_gpio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "driver_adc.h"

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

// Arduino-style HIGH/LÖW pin states
#define HIGH 1
#define LOW  0

// Make pin configurations accessible from anywhere, though from an Arduino perspective,
// these should not be used as they require prefixing with '&', see below.
extern const PinCfg_t pincfg_in;
extern const PinCfg_t pincfg_in_pd;
extern const PinCfg_t pincfg_in_pu;
extern const PinCfg_t pincfg_ain;
extern const PinCfg_t pincfg_out_od;
extern const PinCfg_t pincfg_out_od_s;
extern const PinCfg_t pincfg_out_od_r;
extern const PinCfg_t pincfg_out;
extern const PinCfg_t pincfg_out_s;
extern const PinCfg_t pincfg_out_r;
extern const PinCfg_t pincfg_af_spi1;
extern const PinCfg_t pincfg_af_i2c1;
extern const PinCfg_t pincfg_af_mco1;

// Arduino-style pin modes (not completely standard arduino)
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

// Make pins accessible from anywhere, though from an Arduino perspective,
// these should not be used as they require prefixing with '&', see below.
// This is good though, as is semi-protects these from use in the sketch,
// given that most pins should only be used by their corresponding drivers.
// If a user knows what he/she's doing, there will be no problem slapping
// an ampersand prefix onto these when you want to fiddle with them.
extern const PinDef_t PIN_PA0,  PIN_PA1,  PIN_PA2,  PIN_PA4,  PIN_PA5,  PIN_PA7,  PIN_PA8,  PIN_PA9,
                      PIN_PA11, PIN_PA12, PIN_PA15, PIN_PB0,  PIN_PB1,  PIN_PB2,  PIN_PB3,  PIN_PB4,
                      PIN_PB5,  PIN_PB6,  PIN_PB7,  PIN_PB8,  PIN_PB9,  PIN_PB10, PIN_PB11, PIN_PB12,
                      PIN_PB13, PIN_PB14, PIN_PB15, PIN_PC0,  PIN_PC1,  PIN_PC3,  PIN_PC4,  PIN_PC6,  
                      PIN_PC7,  PIN_PC9,  PIN_PC10, PIN_PC11, PIN_PC12, PIN_PC13;

// Arduino-style pin names
#define BUZZER          (&PIN_PB8 )
#define BUTTON          (&PIN_PB11)
#define POWER           (&PIN_PC3 )
#define USB_CONNECTED   (&PIN_PA9 )
#define LIGHT_SENSOR    (&PIN_PA9 )
#define BATTERY_VOLTAGE (&PIN_PA9 )

// Arduino-style types
typedef bool     boolean;
typedef uint16_t word;
typedef uint8_t  byte;

// Arduino basic functions
void     pinMode(const PinDef_t *, const PinCfg_t *); // Set pin mode
void     digitalWrite(const PinDef_t *, uint8_t);     // Write digital output
uint8_t  digitalRead(const PinDef_t *);               // Read digital input
uint16_t analogRead(const PinDef_t *);                // Read analog input
void     analogWrite(const PinDef_t *, uint16_t);     // Write analog output
void     delay(uint32_t);                             // Millisecond delay
void     delayMicroseconds(uint32_t);                 // Microsecond delay
uint32_t millis(void);                                // Get system up-time (ms)
uint32_t micros(void);                                // Get system up-time (µs)
void     randomSeed(unsigned int seed);               // Seed PRNG
long     random(long howbig);                         // Get random number
long     randomRange(long howsmall, long howbig);     // Get random number
long     map(long x, long in_min, long in_max, long out_min, long out_max);



// Other system wide functions not strictly part of the Arduino model...
void     delay_loop(uint32_t); // Delay by looping
void     initializePins(void); // Initializes all pins to their defaults
uint32_t uinqueId(uint8_t id); // Gets the devices unique identifier (id=0 to 2, each with its own 32-bit id)
// TODO: randomSource(PRNG/TRNG) - To enable the analog True Random Number Generator for random()

// Just good to have around, even though not strictly part of the Arduino model...
#define htons(a) ({ __typeof__ (a) _a = (a); ((_a>>8) & 0xff) | ((_a<<8) & 0xff00); })
#define ntohs(a) ({ __typeof__ (a) _a = (a); ((_a>>8) & 0xff) | ((_a<<8) & 0xff00); })
#define htonl(a) ({ __typeof__ (a) _a = (a); ((_a>>24) & 0xffL) | ((_a>>8) & 0xff00L) | ((_a<<8) & 0xff0000L) | ((_a<<24) & 0xff000000L); })
#define ntohl(a) ({ __typeof__ (a) _a = (a); ((_a>>24) & 0xffL) | ((_a>>8) & 0xff00L) | ((_a<<8) & 0xff0000L) | ((_a<<24) & 0xff000000L); })

// Arduino-style data manipulation
// TODO: should probably be forced inline functions instead (or some_object.max will fail for example)
// TODO: test all of these!
#define max(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#define min(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
//#define abs(a)   ({ __typeof__ (a) _a = (a); (_a < 0 ? -_a : _a); }) // TODO: this breaks stdlib.h
#define constrain(a, b, c) (min(max(a, b), c)) 
#define lowByte(a) ((byte)((a) & 0xFF))
#define highByte(a) ((byte)(((a) >> 8) & 0xFF))
#define bitRead(a, b) ((a) & (1 << (b)))
#define bitWrite(a, b, c) ((a) = ((c) ? ((a) | (1 << (b))) : ((a) & ~(1 << (b)))))
#define bitSet(a, b) ((a) |= (1 << (b)))
#define bitClear(a, b) ((a) &= ~(1 << (b)))
#define bit(a) (1 << (a))


// Arduino-style and Atmel-style (because alot of Arduino code uses them!) enable and disable interrupts 
//
// Looks kinda crazy but basically just defines:
// cli() and noInterrupts() - disable interrupts
// sei() and interrupts() - enable interrupts
//
// Also adds a counter for nesting - ie two calls to cli()
// requires two calls to sei(), which is really good
// under many circumstances.

extern volatile uint16_t int_ctr;

// Nested interrupt enable
__attribute__( ( always_inline ) ) __STATIC_INLINE void cli(void) {
  __ASM volatile ("cpsid i");
  int_ctr++;
}
__attribute__( ( always_inline ) ) __STATIC_INLINE void noInterrupts(void) {
  __ASM volatile ("cpsid i");
  int_ctr++;
}
// Nested interrupt disable
__attribute__( ( always_inline ) ) __STATIC_INLINE void sei(void) {
  __ASM volatile ("cpsid i");
  if(!--int_ctr) {__ASM volatile ("cpsie i");}
}
__attribute__( ( always_inline ) ) __STATIC_INLINE void interrupts(void) {
  __ASM volatile ("cpsid i");
  if(!--int_ctr) {__ASM volatile ("cpsie i");}
}
#ifdef __cplusplus
}
#endif
#endif
