// Davey Taylor, Arduino Verkstad AB
// ADC driver

#ifndef _DRIVER_ADC_H_
#define _DRIVER_ADC_H_

#define ADC_BATTERY_ENABLE     (&PIN_PA1)
#define ADC_LIGHTSENSOR_ENABLE (&PIN_PB4)

void adc_init(void);
void adc_deinit(void);
uint16_t adc_battery(void);
uint16_t adc_lightsensor(void);

#endif
