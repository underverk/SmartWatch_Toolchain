// Davey Taylor, Arduino Verkstad AB
// Accelerometer driver

#include "system.h"

#define ACCEL_ADDRESS 0x30

#define ACCEL_REG_STATUS 0x27
#define ACCEL_REG_STATUS_AUX 0x07
#define ACCEL_REG_FIFO_CTRL 0x2E
#define ACCEL_REG_IDENT 0x0F
#define ACCEL_REG_CTRL1 0x20
#define ACCEL_REG_CTRL2 0x21
#define ACCEL_REG_CTRL3 0x22
#define ACCEL_REG_CTRL4 0x23
#define ACCEL_REG_CTRL5 0x24
#define ACCEL_REG_CTRL6 0x25

// Initialize accelerometer
bool accel_init(void) {
  return false; // failure
}

// Shut down accelerometer
bool accel_deinit(void) {
  return true; // success
}
