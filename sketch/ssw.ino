#include <stdio.h>
#include <stm32f2xx_rtc.h>

static const char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void setup() {
  digitalWrite(BUZZER, HIGH);
  delay(250);
  digitalWrite(BUZZER, LOW);
  while(digitalRead(BUTTON));


  OLED.begin();
  OLED.fillScreen(0x5555);

  OLED.setTextColor(0xFFFF);
  OLED.setTextSize(1);
  
  //OLED.setPower(100);

  //delay(1000);

  OLED.setCursor(0, 0);
  OLED.print((char*)"PMU init...   ");
  if(Battery.begin()) OLED.println((char*)"OK"); else OLED.println((char*)"FAIL");
  OLED.print((char*)"PMU enable... ");
  if(Battery.enableCharging()) OLED.println((char*)"OK"); else OLED.println((char*)"FAIL");
  
  DateTime.begin();
  if(!DateTime.isRunning()) {
    // RTC has lost power, we need to set a new time
    DateTime.setDateTime(13, 05, 03, 6, 20, 41, 00);
  }
  
}


void loop() {
  char text[32];

  OLED.fillRect(0, 24, 128, 56, 0x5555);
  OLED.setCursor(0, 24);
  
  OLED.print("USB connected: ");
  OLED.println((char*)(Battery.canCharge() ? "YES" : "NO"));
    
  OLED.print("PMU charging:  ");
  OLED.println((char*)(Battery.isCharging() ? "YES" : "NO"));
  
  OLED.setCursor(0, 48);

  sprintf(text, "Battery: %.2fV", ((float)Battery.readMilliVolts()) / 1000.0);
  OLED.println(text);
  
  sprintf(text, "Sensor:  %u%%", (LightSensor.readRaw()*100)/4095);
  OLED.println(text);
  
  DateTime.update();
  sprintf(text, "Time: %02u:%02u:%02u", DateTime.hour(), DateTime.minute(), DateTime.second());
  OLED.println(text);


  // Delay in low speed mode
  CPU.setSpeed(CPU_LS);
  delay(150);
  for(uint8_t n = 40; n <= 90; n++) {
    OLED.setPower(n);
    delay(2);
  }
  for(uint8_t n = 90; n >= 40; n--) {
    OLED.setPower(n);
    delay(2);
  }
  delay(150);
  CPU.setSpeed(CPU_HS);

  if(digitalRead(BUTTON)) {
    OLED.shutdown();
    digitalWrite(BUZZER, HIGH);
    delay(250);
    digitalWrite(BUZZER, LOW);
    while(digitalRead(BUTTON));
    digitalWrite(POWER, LOW);
    delay(500);
    while(!digitalRead(BUTTON));
    digitalWrite(POWER, HIGH);
    setup();
    return;
  }
  
}

/*
uint32_t time;

void setup() {
  OLED.begin();

  //OLED.setAddrWindow(10, 10, 20, 20);
  //OLED.pushColor(0x5555);
  OLED.fillScreen(0x5555);
  
  time = millis();
}

uint8_t state = 0;

void loop() {
  
  uint32_t t = millis();
  OLED.fillScreen(0x5555);
  OLED.fillRect(0, 10, (t >> 9) & 0x7F, 10, 0xFF00);
  OLED.fillRect(0, 30, (t >> 1) & 0x7F, 10, 0xFF00);
  
  OLED.setTextColor(0xFFFF);
  OLED.setTextSize(2);
  OLED.setCursor(0, 0);
  OLED.write('0' + (state >> 1));
  OLED.println((char*)"");
  OLED.drawRect(0, 0, 128, 128, 0xFFFF);
  t = millis() - t;

  OLED.write('0' + ((t / 1000) % 10));
  OLED.write('0' + ((t / 100) % 10));
  OLED.write('0' + ((t / 10) % 10));
  OLED.write('0' + ((t / 1) % 10));
  OLED.println((char*)"ms");
  
  
  while((uint32_t)(millis() - time) < 500); time += 500;
  digitalWrite(BUZZER, HIGH);
  while((uint32_t)(millis() - time) < 500); time += 500;
  digitalWrite(BUZZER, LOW);
  
  if(++state == 8) state = 0;
  switch(state >> 1) {
    case 0: CPU.setClockSpeed(CPU_HS); break;
    case 1: CPU.setClockSpeed(CPU_DS); break;
    case 2: CPU.setClockSpeed(CPU_MS); break;
    case 3: CPU.setClockSpeed(CPU_LS); break;
  }
  
  if(digitalRead(BUTTON)) {
   digitalWrite(POWER, LOW);
  }
}
*/