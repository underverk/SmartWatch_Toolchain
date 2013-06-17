// Davey Taylor, Arduino Verkstad AB
// Power Management Arduino Objects

#ifndef _DRIVER_POWER_HPP_
#define _DRIVER_POWER_HPP_
#include "pins.h"
#include "driver_power.h"
#include "driver_adc.h"

class Arduino_CPU {
  public:
    // Change CPU speed
    inline void setSpeed(const ClockDef_t *clk) { cpu_reclock(clk); };
};

extern Arduino_CPU CPU;

class Arduino_Battery {
  public:
    // Initialize PMU (Power Management Unit)
    // The PMU is an I2C external circuit that manages battery charging
    inline boolean begin() { return pmu_init(); };
    // Returns true if charging is possible (means USB is connected)
    inline boolean canCharge() { return digitalRead(USB_CONNECTED) == HIGH; };
    // Allow the PMU to charge the battery (does not mean it will)
    inline boolean enableCharging() { return pmu_charge(true); };
    // Prevent the PMU from charging the battery
    inline boolean disableCharging() { return pmu_charge(false); };
    // Check if the PMU is charging the battery (it will stop on errors, full battery, safety timeout, etc)
    inline boolean isCharging() { bool c; pmu_charging(&c); return c; };
    // Return RAW battery level from ADC
    inline word readRaw() { return adc_battery(); };
    // Return battery voltage in millivolts
    inline word readMilliVolts() { return (adc_battery() * 4440) / 4095; };
    // Return useful battery level as a percentage (0-100)
    // TODO: This should probably have a lookup table to generate more accurate results
    inline byte readLevel() { return (min(max(readMilliVolts(), 3000), 3700) - 3000) / 7; };
};

extern Arduino_Battery Battery;

#endif