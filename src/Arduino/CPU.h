// Davey Taylor, Arduino Verkstad AB
// Power Management Arduino Objects

#ifndef _CPU_H_
#define _CPU_H_
#include "../Arduino.h"
#include "../driver_power.h"
#include "../driver_adc.h"

class Arduino_CPU {
  public:
    // Change CPU speed
    inline void setSpeed(const ClockDef_t *clk) { cpu_reclock(clk); };
};

extern Arduino_CPU CPU;

#endif