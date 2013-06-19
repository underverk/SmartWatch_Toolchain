#include <stdio.h>

// Performs tidy shutdown if user presses button
void checkShutdown() {
  // User requests shutdown?
  if(digitalRead(BUTTON)) {
    // Turn off screen
    OLED.shutdown();
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
    // Run setup again
    setup();
    // Reboot
    ((void(*)())(((uint32_t*)SCB->VTOR)[1]))();
    return;
  }
}

void setup() {
  // Buzz to indicate start
  digitalWrite(BUZZER, HIGH);
  delay(250);
  digitalWrite(BUZZER, LOW);
  // Don't start until button is released
  while(digitalRead(BUTTON));

  // Init display
  OLED.begin();
  OLED.setPower(100);
}


void loop() {
  static float rv, gv, bv;
  rv += 0.07; gv += 0.03; bv += 0.11;
  uint8_t rp = sin(rv) * 48 + 64;
  uint8_t gp = sin(gv) * 48 + 64;
  uint8_t bp = sin(bv) * 48 + 64;
  
  for(uint8_t y = 0; y < 128; y++) {
    uint8_t rc = 15 * max(16 - abs((char)y - (char)rp), 0);
    uint8_t gc = 15 * max(16 - abs((char)y - (char)gp), 0);
    uint8_t bc = 15 * max(16 - abs((char)y - (char)bp), 0);
    OLED.drawFastHLine(0, y, 128, OLED.Color565(rc, gc, bc));
  }
  

  checkShutdown();
  
}