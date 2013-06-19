// Davey Taylor, Arduino Verkstad AB
// Accelerometer Arduino object
#ifndef _DRIVER_ACCEL_HPP_
#define _DRIVER_ACCEL_HPP_
#include "driver_accel.h"

class Arduino_Accel {
  public:
    inline bool begin() { return accel_init(); }
    inline bool shutdown() { return accel_deinit(); }
};

extern Arduino_Accel Movement;

#endif