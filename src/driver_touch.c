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
static uint32_t wd_time[9];

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
  wd_first = true;
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

  // Tip from sony:
  // Sensor may lock up, so we need to watchdog each individual sensor and
  // re-init the controller if any of them stay "active" for too long.
  {
    uint32_t now = millis();
    uint16_t mask = regs.mask = ntohs(regs.mask);
    if(wd_first) {
      wd_first = false;
      for(uint8_t n = 0; n < 9; n++) wd_time[n] = now; // Reset timeouts
    } else {
      for(uint8_t n = 0; n < 9; n++) {
        if(!(mask & 1)) wd_time[n] = now;    // Sensor inactive - reset timeout
        else if(((uint32_t)now - wd_time[n]) > TOUCH_WATCHDOG_TIME) {
          if(!touch_init()) return false;    // Sensor was active for too long
          if(!regs_read(true)) return false; // Read again
          break;
        }
        mask >>= 1; // Next sensor
      }
    }
  }


  // Convert to little-endian
  for(uint8_t n = 0; n < 9; n++) {
    regs.data[n] = ntohs(regs.data[n]);
    if(ex_en) {
      regs.ex.raw[n] = ntohs(regs.ex.raw[n]);
      regs.ex.base[n] = ntohs(regs.ex.base[n]);
    }
  }

  // Return some data
  *x = regs.x;
  *y = regs.y;
  *touch = regs.mask != 0;

  return true;
}