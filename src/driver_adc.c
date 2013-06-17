// Davey Taylor, Arduino Verkstad AB
// ADC driver

#include <stm32f2xx_rcc.h>
#include "pins.h"
#include "driver_adc.h"
  
void adc_init(void) {
  // Initialize ADC
  ADC_CommonInitTypeDef ConfigCADC;
  ADC_CommonStructInit(&ConfigCADC);
  ADC_CommonInit(&ConfigCADC);

  // Initialize ADC1 & ADC2
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2, ENABLE);
  ADC_InitTypeDef ConfigADC;
  ADC_StructInit(&ConfigADC);
  ADC_Init(ADC1, &ConfigADC);
  ADC_Init(ADC2, &ConfigADC);

/*#define ADC_Channel_TempSensor                      ((uint8_t)ADC_Channel_16)
#define ADC_Channel_Vrefint                         ((uint8_t)ADC_Channel_17)
#define ADC_Channel_Vbat                            ((uint8_t)ADC_Channel_18)
*/
  // Configure channels
  ADC_RegularChannelConfig(ADC1, ADC_Channel_2,  1, ADC_SampleTime_15Cycles); // Battery voltage
  ADC_RegularChannelConfig(ADC2, ADC_Channel_14, 1, ADC_SampleTime_15Cycles); // Light sensor

  // Enable ADC1 & 2
  ADC_Cmd(ADC1, ENABLE);
  ADC_Cmd(ADC2, ENABLE);

}

uint16_t adc_battery(void) {
  digitalWrite(ADC_BATTERY_ENABLE, HIGH);
  ADC_SoftwareStartConv(ADC1);
  while(ADC_GetSoftwareStartConvStatus(ADC1));
  return ADC_GetConversionValue(ADC1);
  digitalWrite(ADC_BATTERY_ENABLE, LOW);
}

uint16_t adc_lightsensor(void) {
  digitalWrite(ADC_LIGHTSENSOR_ENABLE, HIGH);
  ADC_SoftwareStartConv(ADC2);
  while(ADC_GetSoftwareStartConvStatus(ADC2));
  return ADC_GetConversionValue(ADC2);
  digitalWrite(ADC_LIGHTSENSOR_ENABLE, LOW);
}

void adc_deinit(void) {
  ADC_DeInit();
}

/*
void adc_initint() {
  NVIC_InitTypeDef ConfigNVIC;
  ConfigNVIC.NVIC_IQRChannel = ADC_IRQn?;//
  ConfigNVIC.NVIC_IQRChannelPreemptionPriority = ADC_PRIO?;//
  ConfigNVIC.NVIC_IQRChannelSubPriority = ADC_PRIO?;//
  ConfigNVIC.NVIC_IQRChannelCmd = ENABLE;
  NVIC_Init(&ConfigNVIC);
  ADC_ITConfig(ADCx?. ADC_IT_EOC, ENABLE);
  
}

void ADC_IRQHandler(void) {
  
}
*/