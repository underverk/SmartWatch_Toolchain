// Davey Taylor, Arduino Verkstad AB
// OLED Display Driver

#ifndef _DRIVER_DISPLAY_H_
#define _DRIVER_DISPLAY_H_
#include <stdint.h>
#include "pins.h"

// OLED SPI
#define OLED_SPI         SPI1
#define OLED_SPI_CLK     RCC_APB2Periph_SPI1

// OLED PINS
#define OLED_SPI_SCK     (&PIN_PA5) // Clock
#define OLED_SPI_MOSI    (&PIN_PA7) // Data
#define OLED_NCS         (&PIN_PA4) // Chip-select
#define OLED_A0          (&PIN_PC0) // Command/Data
#define OLED_RESET       (&PIN_PB5) // Reset
#define OLED_VEN         (&PIN_PC1) // 1.8V-Enable
#define ENABLE_2V8       (&PIN_PB3) // 2.8V-Enable

// Power up and initialize screen
void oled_init(void);

// Shut down the screen (saves power, doh)
void oled_deinit(void);

// Sets screen driving power/brightness (strongly affects power consumption)
void oled_power(uint8_t level);

// Sets screen "window" = drawing region
void oled_window(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

// Pushes a number of pixels into the "window"
void oled_push(uint16_t pixel, uint16_t count);

// Blits a sprite to screen, buffer is w*h uint16_t's
void oled_blit(uint8_t x, uint8_t y, uint8_t w, uint8_t h, void *buffer);

#endif
