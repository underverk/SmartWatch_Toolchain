#ifndef _ACCEL_DEFINES_H_
#define _ACCEL_DEFINES_H_

// I2C Address of the Accelerometer
#define ACCEL_ADDRESS 0x30
#define ACCEL_READBIT 0x01

// Gravity Constants - Taken from https://github.com/microbuilder/LPC11U_LPC13U_CodeBase
#define SENSORS_GRAVITY_EARTH             (9.80665F)              /**< Earth's gravity in m/s^2 */
#define SENSORS_GRAVITY_MOON              (1.6F)                  /**< The moon's gravity in m/s^2 */
#define SENSORS_GRAVITY_SUN               (275.0F)                /**< The sun's gravity in m/s^2 */
#define SENSORS_GRAVITY_STANDARD          (SENSORS_GRAVITY_EARTH)
#define SENSORS_MAGFIELD_EARTH_MAX        (60.0F)                 /**< Maximum magnetic field on Earth's surface */
#define SENSORS_MAGFIELD_EARTH_MIN        (30.0F)                 /**< Minimum magnetic field on Earth's surface */
#define SENSORS_PRESSURE_SEALEVELHPA      (1013.25F)              /**< Average sea level pressure is 1013.25 hPa */
#define SENSORS_DPS_TO_RADS               (0.017453293F)          /**< Degrees/s to rad/s multiplier */
#define SENSORS_GAUSS_TO_MICROTESLA       (100)                   /**< Gauss to micro-Tesla multiplier */

// Senisitivities - Taken from https://github.com/microbuilder/LPC11U_LPC13U_CodeBase
#define ACCEL_SENSITIVITY_2G  (0.001F)
#define ACCEL_SENSITIVITY_4G  (0.002F)
#define ACCEL_SENSITIVITY_8G  (0.004F)
#define ACCEL_SENSITIVITY_16G (0.012F)

// Core registers - Taken from https://github.com/microbuilder/LPC11U_LPC13U_CodeBase
										   // BINARY   DEFAULT   TYPE
#define ACCEL_REGISTER_STATUS_REG_AUX      0x07   // 00000111           r
#define ACCEL_REGISTER_OUT_ADC1_L          0x08   // 00001000 output    r
#define ACCEL_REGISTER_OUT_ADC1_H          0x09   // 00001001 output    r
#define ACCEL_REGISTER_OUT_ADC2_L          0x0A   // 00001010 output    r
#define ACCEL_REGISTER_OUT_ADC2_H          0x0B   // 00001011 output    r
#define ACCEL_REGISTER_OUT_ADC3_L          0x0C   // 00001100 output    r
#define ACCEL_REGISTER_OUT_ADC3_H          0x0D   // 00001101 output    r
#define ACCEL_REGISTER_INT_COUNTER_REG     0x0E   // 00001110           r
#define ACCEL_REGISTER_WHO_AM_I            0x0F   // 00001111 00110011  r
#define ACCEL_REGISTER_TEMP_CFG_REG        0x1F   // 00011111           rw
#define ACCEL_REGISTER_CTRL_REG1           0x20   // 00100000 00000111  rw
#define ACCEL_REGISTER_CTRL_REG2           0x21   // 00100001 00000000  rw
#define ACCEL_REGISTER_CTRL_REG3           0x22   // 00100010 00000000  rw
#define ACCEL_REGISTER_CTRL_REG4           0x23   // 00100011 00000000  rw
#define ACCEL_REGISTER_CTRL_REG5           0x24   // 00100100 00000000  rw
#define ACCEL_REGISTER_CTRL_REG6           0x25   // 00100101 00000000  rw
#define ACCEL_REGISTER_REFERENCE           0x26   // 00100110 00000000  rw
#define ACCEL_REGISTER_STATUS_REG2         0x27   // 00100111 00000000  r
#define ACCEL_REGISTER_OUT_X_L             0x28   // 00101000 output    r
#define ACCEL_REGISTER_OUT_X_H             0x29   // 00101001 output    r
#define ACCEL_REGISTER_OUT_Y_L             0x2A   // 00101010 output    r
#define ACCEL_REGISTER_OUT_Y_H             0x2B   // 00101011 output    r
#define ACCEL_REGISTER_OUT_Z_L             0x2C   // 00101100 output    r
#define ACCEL_REGISTER_OUT_Z_H             0x2D   // 00101101 output    r
#define ACCEL_REGISTER_FIFO_CTRL_REG       0x2E   // 00101110 00000000  rw
#define ACCEL_REGISTER_FIFO_SRC_REG        0x2F   // 00101111           r
#define ACCEL_REGISTER_INT1_CFG            0x30   // 00110000 00000000  rw
#define ACCEL_REGISTER_INT1_SOURCE         0x31   // 00110001 00000000  r
#define ACCEL_REGISTER_INT1_THS            0x32   // 00110010 00000000  rw
#define ACCEL_REGISTER_INT1_DURATION       0x33   // 00110011 00000000  rw
#define ACCEL_REGISTER_CLICK_CFG           0x38   // 00111000 00000000  rw
#define ACCEL_REGISTER_CLICK_SRC           0x39   // 00111001 00000000  r
#define ACCEL_REGISTER_CLICK_THS           0x3A   // 00111010 00000000  rw
#define ACCEL_REGISTER_TIME_LIMIT          0x3B   // 00111011 00000000  rw
#define ACCEL_REGISTER_TIME_LATENCY        0x3C   // 00111100 00000000  rw
#define ACCEL_REGISTER_TIME_WINDOW         0x3D    // 00111101 00000000  rw

// Bit twiddling keys for use with different registers - Taken from https://github.com/microbuilder/LPC11U_LPC13U_CodeBase
#define ACCEL_STATUS_REG_ZYXDA             0x08   // STATUS_REG: XYZ - sample ready
#define ACCEL_STATUS_REG_ZYXOR             0x80   // STATUS_REG: XYZ - new set of data has overwritten the previous ones
#define ACCEL_CTRL_REG1_XEN                0x01   // CTRL_REG1: X enable
#define ACCEL_CTRL_REG1_YEN                0x02   // CTRL_REG1: Y enable
#define ACCEL_CTRL_REG1_ZEN                0x04   // CTRL_REG1: Z enable
#define ACCEL_CTRL_REG1_XYZEN              0x07   // CTRL_REG1: X+Y+Z enable
#define ACCEL_CTRL_REG1_LPEN               0x08   // CTRL_REG1: Low power enable
#define ACCEL_CTRL_REG1_DATARATE_POWERDOWN 0x00   // CTRL_REG1: 0000 xxxx
#define ACCEL_CTRL_REG1_DATARATE_1HZ       0x10   // CTRL_REG1: 0001 xxxx
#define ACCEL_CTRL_REG1_DATARATE_10HZ      0x20   // CTRL_REG1: 0010 xxxx
#define ACCEL_CTRL_REG1_DATARATE_25HZ      0x30   // CTRL_REG1: 0011 xxxx
#define ACCEL_CTRL_REG1_DATARATE_50HZ      0x40   // CTRL_REG1: 0100 xxxx
#define ACCEL_CTRL_REG1_DATARATE_100HZ     0x50   // CTRL_REG1: 0101 xxxx
#define ACCEL_CTRL_REG1_DATARATE_200HZ     0x60   // CTRL_REG1: 0110 xxxx
#define ACCEL_CTRL_REG1_DATARATE_400HZ     0x70   // CTRL_REG1: 0111 xxxx
#define ACCEL_CTRL_REG1_DATARATE_1500HZ    0x80   // CTRL_REG1: 1000 xxxx
#define ACCEL_CTRL_REG1_DATARATE_5000HZ    0x90   // CTRL_REG1: 1001 xxxx
#define ACCEL_CTRL_REG4_BLOCKDATAUPDATE    0x80   // CTRL_REG4: 1xxx xxxx
#define ACCEL_CTRL_REG4_SCALE_2G           0x00   // CTRL_REG4: xx00 xxxx
#define ACCEL_CTRL_REG4_SCALE_4G           0x10   // CTRL_REG4: xx01 xxxx
#define ACCEL_CTRL_REG4_SCALE_8G           0x20   // CTRL_REG4: xx10 xxxx
#define ACCEL_CTRL_REG4_SCALE_16G          0x30   // CTRL_REG4: xx11 xxxx

#endif
