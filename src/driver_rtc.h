// Davey Taylor, Arduino Verkstad AB
// RTC Driver
#ifndef _DRIVER_RTC_H_
#define _DRIVER_RTC_H_
#include <stdint.h>
#include <stdbool.h>

extern uint8_t rtc_hour;
extern uint8_t rtc_minute;
extern uint8_t rtc_second;
extern uint8_t rtc_day;
extern uint8_t rtc_date;
extern uint8_t rtc_month;
extern uint8_t rtc_year;

void rtc_init(void);
bool rtc_running(void);
void rtc_read(void);
void rtc_write(void);


#endif