// Davey Taylor, Arduino Verkstad AB
// Touch Arduino object
#ifndef _DRIVER_TOUCH_HPP_
#define _DRIVER_TOUCH_HPP_
#include <stdint.h>
#include <stdbool.h>
#include "driver_touch.h"

class Arduino_Touch {
  public:
    boolean        begin();
    boolean        read();
    inline byte    getX() { return _x; }
    inline byte    getY() { return _y; }
    inline boolean isTouched() { return _touch; }
  private:
    bool _touch;
    uint8_t _x;
    uint8_t _y;
};

extern Arduino_Touch Touch;

#endif