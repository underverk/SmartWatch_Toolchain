// Davey Taylor, Arduino Verkstad AB
// RTC Arduino Object
#ifndef _DRIVER_RTC_HPP_
#define _DRIVER_RTC_HPP_

class Arduino_DateTime {
  public:
    // Initialize RTC (Real Time Clock - it keeps the time, even when power is off, using the battery)
    inline void begin() { rtc_init(); };
    // Returns true if the RTC is working or false if power was lost and we need to set it again
    inline boolean isRunning() { return rtc_running(); };
    // Updates the time from the RTC
    void update();
    // Writes time to the RTC
    void setDateTime(byte year, byte month, byte date, byte day, byte hour, byte minute, byte second);
    // Refreshed on call to update
    byte year()   { return _year; };
    byte month()  { return _month; };
    byte date()   { return _date; };
    byte day()    { return _day; };
    byte hour()   { return _hour; };
    byte minute() { return _minute; };
    byte second() { return _second; };
    
  private:
    byte _year, _month, _date, _day, _hour, _minute, _second;
};

extern Arduino_DateTime DateTime;

#endif