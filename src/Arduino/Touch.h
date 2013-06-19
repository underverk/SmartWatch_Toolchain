// Davey Taylor, Arduino Verkstad AB
// Touch Arduino object
#ifndef _TOUCH_H_
#define _TOUCH_H_
#include <stdint.h>
#include <stdbool.h>
#include "../driver_touch.h"

class Arduino_Touch {
  public:
    inline boolean begin()     { return touch_init(); }
    inline boolean shutdown()  { return touch_deinit(); }
    inline boolean read()      { return touch_read(&_touch, &_x, &_y); }
    inline byte    getX()      { return _x; }
    inline byte    getY()      { return _y; }
    inline boolean isTouched() { return _touch; }
  private:
    bool _touch;
    uint8_t _x;
    uint8_t _y;
};

extern Arduino_Touch Touch;

#endif