#include <stdio.h>

// This example does not work very well
// TODO: investigate problems with the touch driver, when polling too often?

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
    return;
  }
}

// Shows error to user and goes into infiniloop with buzzing
void fatalError(const char *error) {
  OLED.fillScreen(0x0000);
  OLED.setCursor(0, 0);
  OLED.setTextColor(0xFFFF);
  OLED.setTextSize(1);
  OLED.println((char*)error);
  while(1) {
    digitalWrite(BUZZER, HIGH);
    delay(50);
    digitalWrite(BUZZER, LOW);
    delay(100);
    delay(50);
    digitalWrite(BUZZER, LOW);
    delay(800);
    checkShutdown();
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
  
  // Init touch
  if(!Touch.begin()) fatalError("Touch controller malfunction");
}

void loop() {
  static boolean wasTouched = false;
  static uint8_t drawX, drawY;
  if(Touch.read()) {
    if(Touch.isTouched()) {
      if(!wasTouched) {
        drawX = Touch.getX();
        drawY = Touch.getY();
      } else {
        if(drawX < Touch.getX()) drawX++;
        else if(drawX > Touch.getX()) drawX--;
        
        if(drawY < Touch.getY()) drawY++;
        else if(drawY > Touch.getY()) drawY--;
      }
      drawX = constrain(drawX, 3, 124);
      drawY = constrain(drawY, 3, 124);
      OLED.fillCircle(drawX, drawY, 3, 0xFFFF);
    }
    wasTouched = Touch.isTouched();
  } else {
    // ignore errors, nothing we can do anyway
  }

  checkShutdown();
}