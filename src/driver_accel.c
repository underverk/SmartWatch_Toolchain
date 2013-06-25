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

bool accel_set_xyz_enabled(bool xEnabled, bool yEnabled, bool zEnabled) {
	if(!accel_init_ok) return false;

	uint8_t config0 = ACCEL_CTRL_REG1_DATARATE_50HZ;

	// Enable/Disable X-Axis
	if(xEnabled) config0 |= ACCEL_CTRL_REG1_XEN;

	// Enable/Disable Y-Axis
	if(yEnabled) config0 |= ACCEL_CTRL_REG1_YEN;

	// Enable/Disable Z-Axis
	if(zEnabled) config0 |= ACCEL_CTRL_REG1_ZEN;

	if(i2c_write(ACCEL_ADDRESS, ACCEL_REGISTER_CTRL_REG1, &config0, 1)) return false;

	return true;
}

float accel_get_x(void) {
  if(!accel_init_ok) return 0.0f;

  uint8_t infoBuffer = 0x00;
  uint8_t dataBuffer[2];
  int16_t xv;
  float x;

  // Check if there is data to read
  do {
	if(i2c_read(ACCEL_ADDRESS, ACCEL_REGISTER_STATUS_REG_AUX, &infoBuffer, 1)) return false;
  } while (!(infoBuffer & ACCEL_STATUS_REG_ZYXDA));

  // Read the data
  if(i2c_read(ACCEL_ADDRESS, ACCEL_REGISTER_OUT_X_L | (0x80), &dataBuffer, 2)) return false;

  // correct integer to use low byte first
  xv = (int16_t)(dataBuffer[1] << 8 | dataBuffer[0]);

  // Calculate the right values using gravity
  switch (accelRange) {
	case 16:
	  x = xv * ACCEL_SENSITIVITY_16G * SENSORS_GRAVITY_STANDARD;
	  break;

	case 8:
	  x = xv * ACCEL_SENSITIVITY_8G * SENSORS_GRAVITY_STANDARD;
	  break;

	case 4:
	  x = xv * ACCEL_SENSITIVITY_4G * SENSORS_GRAVITY_STANDARD;
	  break;

	case 2:
	default:
	  x = xv * ACCEL_SENSITIVITY_2G * SENSORS_GRAVITY_STANDARD;
	  break;
  }

  return x;
}

float accel_get_y(void) {
  if(!accel_init_ok) return 0.0f;

  uint8_t infoBuffer = 0x00;
  uint8_t dataBuffer[2];
  int16_t yv;
  float y;

  // Check if there is data to read
  do {
	if(i2c_read(ACCEL_ADDRESS, ACCEL_REGISTER_STATUS_REG_AUX, &infoBuffer, 1)) return false;
  } while (!(infoBuffer & ACCEL_STATUS_REG_ZYXDA));

  // Read the data
  if(i2c_read(ACCEL_ADDRESS, ACCEL_REGISTER_OUT_Y_L | (0x80), &dataBuffer, 2)) return false;

  // correct integer to use low byte first
  yv = (int16_t)(dataBuffer[1] << 8 | dataBuffer[0]);

  // Calculate the right values using gravity
  switch (accelRange) {
	case 16:
	  y = yv * ACCEL_SENSITIVITY_16G * SENSORS_GRAVITY_STANDARD;
	  break;

	case 8:
	  y = yv * ACCEL_SENSITIVITY_8G * SENSORS_GRAVITY_STANDARD;
	  break;

	case 4:
	  y = yv * ACCEL_SENSITIVITY_4G * SENSORS_GRAVITY_STANDARD;
	  break;

	case 2:
	default:
	  y = yv * ACCEL_SENSITIVITY_2G * SENSORS_GRAVITY_STANDARD;
	  break;
  }

  return y;
}

float accel_get_z(void) {
  if(!accel_init_ok) return 0.0f;

  uint8_t infoBuffer = 0x00;
  uint8_t dataBuffer[2];
  int16_t zv;
  float z;

  // Check if there is data to read
  do {
	if(i2c_read(ACCEL_ADDRESS, ACCEL_REGISTER_STATUS_REG_AUX, &infoBuffer, 1)) return false;
  } while (!(infoBuffer & ACCEL_STATUS_REG_ZYXDA));

  // Read the data
  if(i2c_read(ACCEL_ADDRESS, ACCEL_REGISTER_OUT_Z_L | (0x80), &dataBuffer, 2)) return false;

  // correct integer to use low byte first
  zv = (int16_t)(dataBuffer[1] << 8 | dataBuffer[0]);

  // Calculate the right values using gravity
  switch (accelRange) {
	case 16:
	  z = zv * ACCEL_SENSITIVITY_16G * SENSORS_GRAVITY_STANDARD;
	  break;

	case 8:
	  z = zv * ACCEL_SENSITIVITY_8G * SENSORS_GRAVITY_STANDARD;
	  break;

	case 4:
	  z = zv * ACCEL_SENSITIVITY_4G * SENSORS_GRAVITY_STANDARD;
	  break;

	case 2:
	default:
	  z = zv * ACCEL_SENSITIVITY_2G * SENSORS_GRAVITY_STANDARD;
	  break;
  }

  return z;
}

bool accel_get_xyz(float *x, float *y, float *z) {
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
