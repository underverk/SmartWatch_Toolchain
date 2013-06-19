#include <stdio.h>
uint32_t time;

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

  OLED.begin();

  OLED.fillScreen(0x5555);
  OLED.setTextColor(0xFFFF);
  OLED.setTextSize(1);
  
  time = millis();
}

uint8_t state = 0;

void loop() {
  char text[30], speed[10];

  if(++state == 4) state = 0;
  switch(state) {
    case 0: CPU.setSpeed(CPU_HS); sprintf(speed, "120MHz"); break;
    case 1: CPU.setSpeed(CPU_DS); sprintf(speed, "48MHz"); break;
    case 2: CPU.setSpeed(CPU_MS); sprintf(speed, "15MHz"); break;
    case 3: CPU.setSpeed(CPU_LS); sprintf(speed, "3.25MHz"); break;
  }

  digitalWrite(BUZZER, HIGH);
  delay(50);
  digitalWrite(BUZZER, LOW);
  
  uint32_t begin = millis();
  OLED.fillScreen(0x5555);
  OLED.fillRect(0, 5, (begin >> 9) & 0x7F, 10, 0xFF00);
  OLED.drawRect(0, 5, 128, 10, 0xFFFF);
  OLED.fillRect(0, 20, (begin >> 1) & 0x7F, 10, 0xFF00);
  OLED.drawRect(0, 20, 128, 10, 0xFFFF);
  sprintf(text, "CPU speed: %s", speed);
  OLED.setCursor(4, 48);
  OLED.println(text);
  OLED.drawRect(0, 0, 128, 128, 0xFFFF);
  sprintf(text, "Drawing: %ums", (unsigned int)(millis() - begin));
  OLED.setCursor(4, 56);
  OLED.println(text);
  
  digitalWrite(BUZZER, HIGH);
  delay(50);
  digitalWrite(BUZZER, LOW);
  
  delay(1000);
  
  checkShutdown();
}