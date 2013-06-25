// Oliver Pahl
// Accelerometer driver

#include "system.h"
#include "driver_i2c.h"
#include "accel_defines.h"

static bool accel_init_ok = false;
static uint8_t accelRange = 2;        /* +/-2, 4, 8 or 16 g */

// Initialize accelerometer
bool accel_init(void) {
  // Only init once
  if(accel_init_ok) return false;

  uint8_t config0 = ACCEL_CTRL_REG1_DATARATE_50HZ |  /* Normal mode, 50Hz */
                    ACCEL_CTRL_REG1_XYZEN;           /* Enable X, Y and Z */
  uint8_t config1 = ACCEL_CTRL_REG4_BLOCKDATAUPDATE |  /* Enable block update */
                    ACCEL_CTRL_REG4_SCALE_2G;          /* +/-2G measurement range */

  if(i2c_write(ACCEL_ADDRESS, ACCEL_REGISTER_CTRL_REG1, &config0, 1)) return false;
  if(i2c_write(ACCEL_ADDRESS, ACCEL_REGISTER_CTRL_REG4, &config1, 1)) return false;

  accelRange = 2; /* If anything other than ACCEL_CTRL_REG4_SCALE_2G is used, this has to be updated */

  return accel_init_ok = true; // failure
}

bool accel_get_xyz(float *x, float *y, float *z)
{
  if(!accel_init_ok) return false;

  uint8_t infoBuffer = 0x00;
  uint8_t dataBuffer[6];
  int16_t xv, yv, zv;

  // Check if there is data to read
  do {
	if(i2c_read(ACCEL_ADDRESS, ACCEL_REGISTER_STATUS_REG_AUX, &infoBuffer, 1)) return false;
  } while (!(infoBuffer & ACCEL_STATUS_REG_ZYXDA));

  // Read the data
  if(i2c_read(ACCEL_ADDRESS, ACCEL_REGISTER_OUT_X_L | (0x80), &dataBuffer, 6)) return false;

  // correct integer to use low byte first
  xv = (int16_t)(dataBuffer[1] << 8 | dataBuffer[0]);
  yv = (int16_t)(dataBuffer[3] << 8 | dataBuffer[2]);
  zv = (int16_t)(dataBuffer[5] << 8 | dataBuffer[4]);

  // Calculate the right values using gravity
  switch (accelRange) {
    case 16:
	  *x = xv * ACCEL_SENSITIVITY_16G * SENSORS_GRAVITY_STANDARD;
	  *y = yv * ACCEL_SENSITIVITY_16G * SENSORS_GRAVITY_STANDARD;
	  *z = zv * ACCEL_SENSITIVITY_16G * SENSORS_GRAVITY_STANDARD;
	  break;

    case 8:
      *x = xv * ACCEL_SENSITIVITY_8G * SENSORS_GRAVITY_STANDARD;
      *y = yv * ACCEL_SENSITIVITY_8G * SENSORS_GRAVITY_STANDARD;
      *z = zv * ACCEL_SENSITIVITY_8G * SENSORS_GRAVITY_STANDARD;
	  break;

    case 4:
      *x = xv * ACCEL_SENSITIVITY_4G * SENSORS_GRAVITY_STANDARD;
      *y = yv * ACCEL_SENSITIVITY_4G * SENSORS_GRAVITY_STANDARD;
      *z = zv * ACCEL_SENSITIVITY_4G * SENSORS_GRAVITY_STANDARD;
	  break;

    case 2:
    default:
      *x = xv * ACCEL_SENSITIVITY_2G * SENSORS_GRAVITY_STANDARD;
      *y = yv * ACCEL_SENSITIVITY_2G * SENSORS_GRAVITY_STANDARD;
      *z = zv * ACCEL_SENSITIVITY_2G * SENSORS_GRAVITY_STANDARD;
	  break;
  }

  return true;
}

// Shut down accelerometer
bool accel_deinit(void) {
  if(!accel_init_ok) return false;

  uint8_t config0 = ACCEL_CTRL_REG1_DATARATE_POWERDOWN; /* Power the Accelerometer Down */
  accel_init_ok = false;

  if(i2c_write(ACCEL_ADDRESS, ACCEL_REGISTER_CTRL_REG1, &config0, 1)) return false;

  return true; // success
}
