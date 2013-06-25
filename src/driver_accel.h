// Oliver Pahl
// Accelerometer driver
#ifndef _DRIVER_ACCEL_H_
#define _DRIVER_ACCEL_H_
#ifdef __cplusplus
extern "C" {
#endif

// Initialize accelerometer
bool accel_init(void);

// Get accelerometer Data (all x y and z)
bool accel_get_xyz(float *x, float *y, float *z);

// Get accelerometer Data (specific)
float accel_get_x(void);
float accel_get_y(void);
float accel_get_z(void);

// Enable and disable specific axes
bool accel_set_xyz_enabled(bool xEnabled, bool yEnabled, bool zEnabled);

// Shut down accelerometer
bool accel_deinit(void);


#ifdef __cplusplus
}
#endif
#endif
