// Davey Taylor, Arduino Verkstad AB
// Accelerometer Arduino object
#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_
#include "../driver_accel.h"

class Arduino_Accel {
  public:
    inline bool begin() { return accel_init(); }

    // get the X Y and Z coordinates together
    inline boolean getXyz(float *x, float *y, float *z) { return accel_get_xyz(x, y, z); };

    // Get the Coordinates of one Axis
    inline float getX() { return accel_get_x(); }
    inline float getY() { return accel_get_y(); }
    inline float getZ() { return accel_get_z(); }

    // Enable/Disable specific Axis
    inline bool enableAxis(bool bXAxis, bool bYAxis, bool bZAxis) { return accel_set_xyz_enabled(bXAxis, bYAxis, bZAxis); }

    inline bool shutdown() { return accel_deinit(); }
};

extern Arduino_Accel Movement;

#endif
