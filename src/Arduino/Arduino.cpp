// Davey Taylor, Arduino Verkstad AB
// Sony SmartWatch startup procedure for Arduino-type build (make sketch)

// Include our drivers and libs
#include <stdarg.h>
#include "Arduino.h"
#include "driver_i2c.h"
#include "driver_adc.h"
#include "driver_rtc.h"
#include "driver_display.h"
#include "driver_power.h"
#include "driver_touch.h"
#include "driver_vcp.h"
 
// Include our Arduino style C++ objects
// This should eventually be moved to the makefile
#include "OLED.cpp"
#include "Battery.cpp"
#include "CPU.cpp"
#include "DateTime.cpp"
#include "Touch.cpp"
#include "Movement.cpp"

static const char uHex[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

static int8_t analog_shift = 2; 

// Found in sketch (.ino file)
void setup();
void loop();

int main(void) {

  // System initialization
  sys_init();
  
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

// Reads analog input
uint16_t analogRead(const PinDef_t *pin) {
  uint16_t value;
  if(pin == LIGHT_SENSOR) {
    value = adc_lightsensor();
  } else if(pin == BATTERY_VOLTAGE) {
    value = adc_battery();
  } else {
    value = 0;
  }
  if(analog_shift < 0) {
    value <<= -analog_shift;
  } else {
    value >>= analog_shift;
  }
  return value;
}

// Set analogRead resolution
void analogReadResolution(int8_t bits) {
  analog_shift = 12 - bits;
}

// Writes analog output
void analogWrite(const PinDef_t *pin, uint16_t value) {
  // Dummy
}
// Set analogWrite resolution
void analogWriteResolution(int8_t bits) {
  // Dummy
}

// Uptime in microseconds (worthless hack)
uint32_t micros() {  
  return (millis() & 0x3FFFFF) * 1000;
}

// Delay microseconds (worthless hack)
void delayMicroseconds(uint32_t us) {
  delay((us + 999) / 1000);
}

// Seeds the random number generator
void randomSeed(unsigned int seed) {
  if(seed != 0) srand(seed);
}

// Return random number between 0 and howbig
long random(long howbig) {
  if(howbig == 0) return 0;
  return rand() % howbig;
}

// Return random number between howsmall and howbig
long random(long howsmall, long howbig) {
  if (howsmall >= howbig) return howsmall;
  long diff = howbig - howsmall;
  return random(diff) + howsmall;
}

// Maps a value from range:in to range:out
long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// TODO: We're in C-mode here, so I had to remove the makeWord(int)
unsigned int makeWord(unsigned char h, unsigned char l) { return (h << 8) | l; }

// Include the sketch
#pragma GCC diagnostic ignored "-Wwrite-strings"
#include "../../sketch/ssw.ino"