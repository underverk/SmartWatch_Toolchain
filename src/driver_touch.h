// Davey Taylor, Arduino Verkstad AB
// Touch driver
#ifndef _DRIVER_TOUCH_H_
#define _DRIVER_TOUCH_H_

#include "Arduino.h"

typedef enum {
  TOUCH_MODE_NONE          = 0x00,
  TOUCH_MODE_IRQ           = 0x01,
  TOUCH_MODE_WAKE_ON_TOUCH = 0x02,
  TOUCH_MODE_SCAN_DISABLE  = 0x04,
  TOUCH_MODE_CYPRESS       = 0x08,
  TOUCH_MODE_RAW_COUNTS    = 0x10,
  TOUCH_MODE_COMPENSATION  = 0x20,
  TOUCH_MODE_POLLED_ONLY   = 0x40,
  TOUCH_MODE_SLEEP         = 0x80,
} TouchMode_e;

bool touch_init(void);
bool touch_mode_set(TouchMode_e mode);
bool touch_mode_clear(TouchMode_e mode);
bool touch_read(bool *touch, uint8_t *x, uint8_t *y);

#endif