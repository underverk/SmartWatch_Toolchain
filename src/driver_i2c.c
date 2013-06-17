// Davey Taylor, Arduino Verkstad AB
// I2C driver

#include <stm32f2xx_i2c.h>
#include <stm32f2xx_dma.h>
#include "driver_i2c.h"

void i2c_init(void) {    
  // Enable I2C & pin clocks
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  RCC_AHB1PeriphClockCmd(I2C_SDA->clock | I2C_SCL->clock, ENABLE);

  // Reset I2C
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

  // Enable I2C
  I2C_Cmd(I2C1, ENABLE);

  // I2C Configuration
  I2C_InitTypeDef  ConfigI2C;
  ConfigI2C.I2C_Mode                = I2C_Mode_I2C;
  ConfigI2C.I2C_DutyCycle           = I2C_DUTYCYCLE;
  ConfigI2C.I2C_OwnAddress1         = 0xA0;
  ConfigI2C.I2C_Ack                 = I2C_Ack_Enable;
  ConfigI2C.I2C_ClockSpeed          = I2C_SPEED;
  ConfigI2C.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  
  // Initialize I2C
  I2C_Init(I2C1, &ConfigI2C);

}

#define uses_timeout \
  uint32_t timer; \
  uint8_t timeout; \

#define timeout(TIME, CONDITION) \
  timer = millis(); \
  timeout = 0; \
  while(CONDITION) { \
    if(((uint32_t)(millis() - timer)) >= (TIME)) { \
      timeout = 1; \
      break; \
    } \
  } \
  if(timeout)

static uint8_t i2c_reset(uint8_t errorcode) {
	I2C_AcknowledgeConfig(I2C1, ENABLE);
  I2C_GenerateSTOP(I2C1, ENABLE);
  return errorcode;
}

uint8_t i2c_write(uint8_t device, uint8_t reg, void *buffer, uint32_t bytes) {
  uses_timeout

	// START
	I2C_GenerateSTART(I2C1, ENABLE); 
	timeout(100, !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) return i2c_reset(2);

	// Address
	I2C_Send7bitAddress(I2C1, device, I2C_Direction_Transmitter); 
	timeout(100, !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) return i2c_reset(3);

	// Register
	I2C_SendData(I2C1, reg); 
	timeout(100, !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) return i2c_reset(4);

	// Data
	while(bytes--) {
  	I2C_SendData(I2C1, *(uint8_t*)buffer); 
  	buffer = ((uint8_t*)buffer) + 1;
  	timeout(100, !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) return i2c_reset(4);
  }

	// STOP
	I2C_GenerateSTOP(I2C1, ENABLE);
	
	return 0;
}

uint8_t i2c_read(uint8_t device, uint8_t reg, void *buffer, uint32_t bytes) {
  uses_timeout
  
  // While the bus is busy
	timeout(100, I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) return 1;

	/// START
	I2C_GenerateSTART(I2C1, ENABLE);
	timeout(100, !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) return i2c_reset(2);

	// Address:WRITE
	I2C_Send7bitAddress(I2C1, device, I2C_Direction_Transmitter); 
	timeout(100, !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) return i2c_reset(3);

	I2C_Cmd(I2C1, ENABLE); // ???

	// Register
	I2C_SendData(I2C1, reg);
	timeout(100, !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) return i2c_reset(4);

	// REPEATED-START
	I2C_GenerateSTART(I2C1, ENABLE);
	timeout(100, !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) return i2c_reset(5);

  // Address:READ
	I2C_Send7bitAddress(I2C1, device, I2C_Direction_Receiver);
	timeout(100, !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) return i2c_reset(6);

	// Data
	while(bytes--) {
		if(!bytes) {
		  // Disable ACK
		  I2C_AcknowledgeConfig(I2C1, DISABLE);
		  I2C_GenerateSTOP(I2C1, ENABLE);
		}
		timeout(100, !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) return i2c_reset(7);
    *((uint8_t*)buffer) = I2C_ReceiveData(I2C1);
    buffer = ((uint8_t*)buffer) + 1;
	}

	// Enable ACK
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	
	return 0;
		
}