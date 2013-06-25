// Davey Taylor, Arduino Verkstad AB
// Accelerometer Arduino object
#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_
#include "../driver_accel.h"

class Arduino_Accel {
  public:
    inline bool begin() { return accel_init(); }

    // get the X Y and Z coordinates
    inline boolean getXyz(float *x, float *y, float *z) { return accel_get_xyz(x, y, z); };

    inline bool shutdown() { return accel_deinit(); }
};

extern Arduino_Accel Movement;

#endif
