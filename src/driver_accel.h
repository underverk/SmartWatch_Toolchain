// Davey Taylor, Arduino Verkstad AB
// Accelerometer driver
#ifndef _DRIVER_ACCEL_H_
#define _DRIVER_ACCEL_H_
#ifdef __cplusplus
extern "C" {
#endif


// Initialize accelerometer
bool accel_init(void);

// Shut down accelerometer
bool accel_deinit(void);


#ifdef __cplusplus
}
#endif
#endif