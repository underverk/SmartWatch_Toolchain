// Davey Taylor, Arduino Verkstad AB
// Touch Arduino object

#include "driver_touch.hpp"

boolean Arduino_Touch::begin(void) {
  return touch_init();
}

boolean Arduino_Touch::read(void) {
  return touch_read(&_touch, &_x, &_y);
}

Arduino_Touch Touch;