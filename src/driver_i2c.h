// Davey Taylor, Arduino Verkstad AB
// I2C driver

#ifndef _DRIVER_I2C_H_
#define _DRIVER_I2C_H_
#include "pins.h"

/***
 EXAMPLE here subversion.assembla.com/svn/freertos/STM32F2xx_StdPeriph_Lib_V1.0.0/Project/STM32F2xx_StdPeriph_Examples/I2C/I2C_TwoBoards/DataExchangeDMA/
 ***/

//FAST!
//#define I2C_SPEED 340000
//#define I2C_DUTYCYCLE I2C_DutyCycle_16_9  

#define I2C_SPEED                     100000
#define I2C_DUTYCYCLE                 I2C_DutyCycle_2

#define I2C_SDA (&PIN_PB7)
#define I2C_SCL (&PIN_PB6)

void i2c_init(void);
uint8_t i2c_write(uint8_t device, uint8_t reg, void *buffer, uint32_t bytes);
uint8_t i2c_read(uint8_t device, uint8_t reg, void *buffer, uint32_t bytes);
#endif