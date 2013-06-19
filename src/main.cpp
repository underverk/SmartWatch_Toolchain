// Davey Taylor, Arduino Verkstad AB
// Sony SmartWatch startup procedure

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Include our drivers and libs
#include "Arduino.h"
#include "driver_i2c.h"
#include "driver_adc.h"
#include "driver_rtc.h"
#include "driver_display.h"
#include "driver_power.h"
#include "driver_touch.h"
 
// Include our Arduino style C++ objects
// This should eventually be moved to the makefile
#include "driver_display.cpp"
#include "driver_power.cpp"
#include "driver_rtc.cpp"
#include "driver_touch.cpp"
#include "driver_accel.cpp"

// USB pins
#define USB_DP        (&PIN_PA12)
#define USB_DM        (&PIN_PA11)

/*
// This can be called from anywhere and is good to use as a test
// if your clock does not boot, to find out exactly how far the
// startup procedure reaches. The startup procedure begins in
// libstm32f2/startup_stm32f2xx.S at Reset_Handler:
// To call this function from assembly, use "bl buzztest"
extern "C" {
void buzztest(void) {
  pinMode(BUZZER, BUZZER->cfg);
  digitalWrite(BUZZER, HIGH);  
}
}
*/

static const char uHex[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

// System initialization
void init() {
  // Initializes all pins to their default settings (see pins.c)
  initializePins();

  // Initialize system clocks
  cpu_init();

  // Start system tick (used for timing, delay, etc)
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock / 1000);
    
  // Tip from Sony:
  // Prevents hard-faults when booting from USB
  delay(50);
  
  // Tip from Sony:
  // Not quite sure, but I believe a pullup on DP enables charging of a device even if
  // it does not do USB any communication by removing the pre-enum current limit
  if(digitalRead(USB_CONNECTED)) pinMode(USB_DP, INPUT_PULLUP);
  
  // Initialize ADC
  adc_init();

  // Initialize I2C
  i2c_init();
}

// Found in sketch (.ino file)
void setup();
void loop();

int main(void) {

  // System initialization
  init();
  
  // User initialization
  setup();

	// Forever loop, I want to be, forever... loop
	while(1) loop();
	
	return 0;
}

// Standard startup
void standardStartup() {
  // Buzz to indicate start
  digitalWrite(BUZZER, HIGH);
  delay(250);
  digitalWrite(BUZZER, LOW);
  // Don't start until button is released
  while(digitalRead(BUTTON));
}

// Standsard shutdown procedure
void standardShutdown() {
  // Turn off everything
  OLED.shutdown();
  Touch.shutdown();
  Movement.shutdown();
  // Buzz to indicate shutdown
  digitalWrite(BUZZER, HIGH);
  delay(250);
  digitalWrite(BUZZER, LOW);
  // Don't turn off until button is released
  while(digitalRead(BUTTON));
  // Turn off power
  digitalWrite(POWER, LOW);
  // Device won't turn off if USB powered
  delay(500);
  // So, wait for another button press
  while(!digitalRead(BUTTON));
  // Power up again
  digitalWrite(POWER, HIGH);
  // Reboot
  cli();
  ((void(*)())(((uint32_t*)SCB->VTOR)[1]))();
  return;
}

// Shows error to user and goes into infiniloop with buzzing
void fatalError(char *error, ...) {
  char numbers[18];
  OLED.shutdown();
  OLED.begin();
  OLED.fillScreen(0x0000);
  //OLED.Color565(0xFF, 0x00, 0xFF)
  OLED.fillRect(0, 0, 128, 28, 0xF800);
  OLED.fillRect(4, 4, 120, 20, 0x0000);
  OLED.setTextColor(0xF800);
  OLED.setTextSize(1);

  OLED.setCursor(20, 6);
  OLED.print((char*)"GURU MEDITATION");

  va_list ap;
  uint32_t a, b;
  va_start(ap, error);
  a = va_arg(ap, uint32_t);
  b = va_arg(ap, uint32_t);
  b = va_arg(ap, uint32_t);
  numbers[7] = uHex[a & 0xF]; a >>= 4; numbers[6] = uHex[a & 0xF]; a >>= 4;
  numbers[5] = uHex[a & 0xF]; a >>= 4; numbers[4] = uHex[a & 0xF]; a >>= 4;
  numbers[3] = uHex[a & 0xF]; a >>= 4; numbers[2] = uHex[a & 0xF]; a >>= 4;
  numbers[1] = uHex[a & 0xF]; a >>= 4; numbers[0] = uHex[a & 0xF]; a >>= 4;
  numbers[16] = uHex[b & 0xF]; b >>= 4; numbers[15] = uHex[b & 0xF]; b >>= 4;
  numbers[14] = uHex[b & 0xF]; b >>= 4; numbers[13] = uHex[b & 0xF]; b >>= 4;
  numbers[12] = uHex[b & 0xF]; b >>= 4; numbers[11] = uHex[b & 0xF]; b >>= 4;
  numbers[10] = uHex[b & 0xF]; b >>= 4; numbers[9] = uHex[b & 0xF]; b >>= 4;
  numbers[8] = ':'; numbers[17] = 0;

  OLED.setCursor(15, 14);
  OLED.print(numbers);
  
  OLED.setCursor(0, 32);
  OLED.print(error);  
  uint32_t time = millis();
  while(1) {
    OLED.fillRect(  0,  0, 128,  4, 0x0000);
    OLED.fillRect(  0,  4,   4, 20, 0x0000);
    OLED.fillRect(124,  4,   4, 20, 0x0000);
    OLED.fillRect(  0, 24, 128,  4, 0x0000);
    digitalWrite(BUZZER, HIGH); delay(50); digitalWrite(BUZZER, LOW);
    delay(100);
    digitalWrite(BUZZER, HIGH); delay(50); digitalWrite(BUZZER, LOW);
    OLED.fillRect(  0,  0, 128,  4, 0xF800);
    OLED.fillRect(  0,  4,   4, 20, 0xF800);
    OLED.fillRect(124,  4,   4, 20, 0xF800);
    OLED.fillRect(  0, 24, 128,  4, 0xF800);
    delay(800);
    if(digitalRead(BUTTON) || millis() - time > 20000) standardShutdown();
  }
}

// Include the sketch
#pragma GCC diagnostic ignored "-Wwrite-strings"
#include "../sketch/ssw.ino"