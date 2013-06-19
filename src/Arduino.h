// Davey Taylor, Arduino Verkstad AB
// Arduino internals and platform configuration

#ifndef _ARDUINO_H_
#define _ARDUINO_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stm32f2xx_gpio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "system.h"

// Arduino-style HIGH/LÖW pin states
#define HIGH 1
#define LOW  0

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

// Arduino-style types
typedef bool     boolean;
typedef uint16_t word;
typedef uint8_t  byte;

// Other system wide functions not strictly part of the Arduino model...
// TODO: randomSource(PRNG/TRNG) - To enable the analog True Random Number Generator for random()
void standardStartup(void);    // Perform standard startup stuff
void standardShutdown(void);   // Perform a standard shutdown

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

// Arduino constants
#define PI 3.14159

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

// Arduino basic functions
uint16_t analogRead(const PinDef_t *);                // Read analog input
void     analogWrite(const PinDef_t *, uint16_t);     // Write analog output
void     delay(uint32_t);                             // Millisecond delay
void     delayMicroseconds(uint32_t);                 // Microsecond delay
uint32_t micros(void);                                // Get system up-time (µs)
void     randomSeed(unsigned int seed);               // Seed PRNG
long     random(long howbig);                         // Get random number
long     randomRange(long howsmall, long howbig);     // Get random number
long     map(long x, long in_min, long in_max, long out_min, long out_max);

// Gets the devices unique identifier (id=0 to 2, each with its own 32-bit id)
__attribute__( ( always_inline ) ) __STATIC_INLINE
uint32_t uinqueId(uint8_t id) { return sys_id(id); }

// Read digital input
__attribute__( ( always_inline ) ) __STATIC_INLINE
uint8_t digitalRead(const PinDef_t *pin) { return pin_read(pin); }

// Write digital output
__attribute__( ( always_inline ) ) __STATIC_INLINE
void digitalWrite(const PinDef_t *pin, uint8_t value) { pin_write(pin, value); }

// Set pin mode
__attribute__( ( always_inline ) ) __STATIC_INLINE
void pinMode(const PinDef_t *pin, const PinCfg_t *cfg) { pin_mode(pin, cfg); }

// Milliseconds sice system start
__attribute__( ( always_inline ) ) __STATIC_INLINE
uint32_t millis() { return ticks(); }

#ifdef __cplusplus
}
#endif
#endif

