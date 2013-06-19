#include <stdio.h>
uint32_t time;

void setup() {
  // Run standard startup procedure
  standardStartup();

  // Initialize screen
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
  
  // Shut down if button is pressed
  if(digitalRead(BUTTON)) standardShutdown();
}