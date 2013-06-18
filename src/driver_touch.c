// Davey Taylor, Arduino Verkstad AB
// Touch driver

#include <stm32f2xx_rtc.h>
#include "driver_i2c.h"
#include "Arduino.h"
#include "touch_defines.h"
#include "driver_touch.h"

typedef struct __attribute__ ((__packed__)) {
  uint16_t raw[9];
  uint16_t base[9];
} TouchRegsEx_t;


typedef struct __attribute__ ((__packed__)) {
  uint8_t mode;     // modes (flags)
  uint8_t cmd;      // command
  uint16_t mask;    // sensor activity, bitwise
  uint8_t x;        // touch x position
  uint8_t y;        // touch y position
  uint16_t data[9]; // data (depends on command/mode)
  TouchRegsEx_t ex; // extra data (for B0, B1 versions)
} TouchRegs_t;

static TouchRegs_t regs;

static uint16_t version;
static bool ex_en;


static bool     wd_first;
static uint16_t wd_mask;
static uint16_t wd_time[9];

#define MAX_RETRY 10

static bool regs_read(bool ex) {
  ex &= ex_en;
  for(uint8_t n = 0; n < MAX_RETRY; n++) {
    if(!i2c_read(TOUCH_ADDRESS, 0, &regs, sizeof(TouchRegs_t) - ex ? sizeof(TouchRegsEx_t) : 0)) {
      return true;
    }
  }
  return false;
}

static bool regs_write(bool ex) {
  ex &= ex_en;
  for(uint8_t n = 0; n < MAX_RETRY; n++) {
    if(!i2c_read(TOUCH_ADDRESS, 0, &regs, sizeof(TouchRegs_t) - ex ? sizeof(TouchRegsEx_t) : 0)) {
      return true;
    }
  }
  return false;
}

// Initialize touch controller (aka read its version)
bool touch_init(void) {
  // Reset touch controller
  digitalWrite(TOUCH_RESET, HIGH);
  delay(50);
  digitalWrite(TOUCH_RESET, LOW);
  delay(50);
  // Read device
  if(!regs_read(false)) return false;
  // Issue version command
  regs.cmd = TOUCH_CMD_VERSION;
  if(!regs_write(false)) return false;
  // Read device
  if(!regs_read(false)) return false;
  // Get version, check for extended registers
  version = ntohs(regs.data[0]);
  ex_en = (version == 0xB0 || version == 0xB1);
  // Issue "no command"
  regs.cmd = TOUCH_CMD_NONE;
  if(!regs_write(false)) return false;
  // Clear modes
  if(ex_en) touch_mode_clear(TOUCH_MODE_CYPRESS); // Disable cypress mode
  touch_mode_set(TOUCH_MODE_POLLED_ONLY); // Disables interrupt
  touch_mode_clear(TOUCH_MODE_IRQ); // Clear interrupt flag
  wd_first = true;
  return true;
}

bool touch_mode_set(TouchMode_e mode) {
  if(!regs_read(false)) return false;
  regs.mode |= mode;
  if(!regs_write(false)) return false;
  if(!regs_read(false)) return false;
  return (regs.mode & mode) != 0;
}

bool touch_mode_clear(TouchMode_e mode) {
  if(!regs_read(false)) return false;
  regs.mode &= ~mode;
  if(!regs_write(false)) return false;
  if(!regs_read(false)) return false;
  return (regs.mode & mode) == 0;
}

bool touch_read(bool *touch, uint8_t *x, uint8_t *y) {
  if(!regs_read(true)) return false;

  // Convert to little-endian
  for(uint8_t n = 0; n < 9; n++) {
    regs.data[n] = ntohs(regs.data[n]);
    if(ex_en) {
      regs.ex.raw[n] = ntohs(regs.ex.raw[n]);
      regs.ex.base[n] = ntohs(regs.ex.base[n]);
    }
  }

  // Tip from sony:
  // Sensor may freeze, so we need to watchdog each individual sensor
  {
    regs.mask = ntohs(regs.mask);
    if(!wd_first) {
      // Get activity mask
      uint16_t activity = wd_mask ^ regs.mask;
      uint32_t now = millis();
      for(uint8_t n = 0; n < 9; n++) {
        // Sensor activity?
        if(activity & 1) {
          // Reset watchdog
          wd_time[n] = now;
        } else {
          // No activity, check time since last activity
          if(((uint32_t)now - wd_time[n]) > TOUCH_WATCHDOG_TIME) {
            // Idle for too long, reset
            touch_init();
            return false; // TODO: should probably return last data and true
          }
        }
        // Next in mask
        activity >>= 1;
      }
    } else {
      wd_first = false;
      // Reset watchdog timers
      uint32_t now = millis();
      for(uint8_t n = 0; n < 9; n++) wd_time[n] = now;
   }
    wd_mask = regs.mask;
  }

  *x = regs.x;
  *y = regs.y;
  *touch = regs.mask != 0;
  return true;
}