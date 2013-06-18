// Davey Taylor, Arduino Verkstad AB
// RTC Arduino Object
#include "driver_rtc.hpp"
#include "driver_rtc.h"

void Arduino_DateTime::update(void) {
  rtc_read();
  _year   = rtc_year;
  _month  = rtc_month;
  _date   = rtc_date;
  _day    = rtc_day;
  _hour   = rtc_hour;
  _minute = rtc_minute;
  _second = rtc_second;
}

void Arduino_DateTime::setDateTime(byte year, byte month, byte date, byte day, byte hour, byte minute, byte second) {
  rtc_year   = year;
  rtc_month  = month;
  rtc_date   = date;
  rtc_day    = day;
  rtc_hour   = hour;
  rtc_minute = minute;
  rtc_second = second;
  rtc_write();
}


Arduino_DateTime DateTime;