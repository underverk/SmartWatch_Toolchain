// Davey Taylor, Arduino Verkstad AB
// RTC driver

// TODO: This does not quite work... the RTC looses it's time when the clock powers down.
//       But at least we have a starting point, yeah?

#include <stm32f2xx_rtc.h>
#include "pins.h"
#include "driver_rtc.h"

static bool rtc_ok;

uint8_t rtc_hour;
uint8_t rtc_minute;
uint8_t rtc_second;
uint8_t rtc_day;
uint8_t rtc_date;
uint8_t rtc_month;
uint8_t rtc_year;

void rtc_init(void) {
  rtc_ok = RTC_ReadBackupRegister(RTC_BKP_DR0) == 0xCA7E;
  if(rtc_ok) {
    
    // Enable the PWR clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    // Allow access to RTC
    PWR_BackupAccessCmd(ENABLE);

    // Wait for RTC APB registers synchronisation
    RTC_WaitForSynchro();
  }
}

bool rtc_running(void) {
  return rtc_ok;
}

void rtc_write(void) {
  if(!rtc_ok) {
    // Enable the PWR clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    // Allow access to RTC
    PWR_BackupAccessCmd(ENABLE);

    // Enable the LSE OSC
    RCC_LSEConfig(RCC_LSE_ON);
  
    // Wait till LSE is ready
    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
  
    // Select the RTC Clock Source
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  
    // Enable the RTC Clock
    RCC_RTCCLKCmd(ENABLE);

    // Wait for RTC APB registers synchronisation
    RTC_WaitForSynchro();

    // Enable The TimeStamp 
    RTC_TimeStampCmd(RTC_TimeStampEdge_Falling, ENABLE); // TODO: can return ERROR

    RTC_InitTypeDef RTC_InitStructure;
  
    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
    RTC_InitStructure.RTC_SynchPrediv = 0xFF;
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    RTC_Init(&RTC_InitStructure);
  }
  
  RTC_TimeTypeDef TimeRTC;
  RTC_DateTypeDef DateRTC;

  TimeRTC.RTC_H12     = RTC_H12_AM; // Always AM for 24H
  TimeRTC.RTC_Hours   = rtc_hour;
  TimeRTC.RTC_Minutes = rtc_minute;
  TimeRTC.RTC_Seconds = rtc_second;
  
  RTC_SetTime(RTC_Format_BIN, &TimeRTC);
  
  (void)RTC->DR;
  
  DateRTC.RTC_WeekDay = rtc_day;
  DateRTC.RTC_Date    = rtc_date;
  DateRTC.RTC_Month   = rtc_month;
  DateRTC.RTC_Year    = rtc_year;
  
  RTC_SetDate(RTC_Format_BIN, &DateRTC); // TODO: can return ERROR

  RTC_WriteBackupRegister(RTC_BKP_DR0, 0xCA7E);
  
  rtc_ok = true;
}

void rtc_read(void) {
  RTC_TimeTypeDef TimeRTC;
  RTC_DateTypeDef DateRTC;
  uint8_t atom;
  if(!rtc_ok) return;
  RTC_GetDate(RTC_Format_BIN, &DateRTC);
  do {
    atom = DateRTC.RTC_Date;
    RTC_GetTime(RTC_Format_BIN, &TimeRTC);
    RTC_GetDate(RTC_Format_BIN, &DateRTC);
  } while(atom != DateRTC.RTC_Date);
  rtc_hour   = TimeRTC.RTC_Hours;
  rtc_minute = TimeRTC.RTC_Minutes;
  rtc_second = TimeRTC.RTC_Seconds;
  rtc_day    = DateRTC.RTC_WeekDay;
  rtc_date   = DateRTC.RTC_Date; 
  rtc_month  = DateRTC.RTC_Month;  
  rtc_year   = DateRTC.RTC_Year;
  
}