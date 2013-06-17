// Davey Taylor, Arduino Verkstad AB
// Sony SmartWatch startup procedure

// Include our drivers and libs
extern "C" {
  #include "pins.h"
  #include "driver_i2c.h"
  #include "driver_adc.h"
  #include "driver_rtc.h"
  #include "driver_display.h"
  #include "driver_power.h"
}
 
// Include our Arduino style C++ objects
// This should eventually be moved to the makefile
#include "driver_display.cpp"
#include "driver_power.cpp"
#include "driver_rtc.cpp"

// System initialization
void init() {
  // Tip from Sony:
  // Prevents hard-faults when booting from USB
  delay_loop(200000);
  
  // Initializes all pins to their default settings (see pins.c)
  initializePins();
  
  // Tip from Sony:
  // Not quite sure, but I believe a pullup on DP enables charging of a device even if
  // it does not do USB any communication by removing the pre-enum current limit
  if(digitalRead(USB_CONNECTED)) pinMode(USB_DP, INPUT_PULLUP);
  
  // Initialize system clocks
  cpu_init();

  // Disable interrupts until we are done setting everything up
  cli();

  // Start system tick (used for timing, delay, etc)
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock / 1000);
  
  // Initialize ADC
  adc_init();

  // Enable interrupts
  sei();

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

// Include the sketch
#pragma GCC diagnostic ignored "-Wwrite-strings"
#include "../sketch/ssw.ino"