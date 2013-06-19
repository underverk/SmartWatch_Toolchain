#include <stdio.h>

// This example does not work very well
// TODO: investigate problems with the touch driver, when polling too often?

void setup() {
  // Run standard startup procedure
  standardStartup();

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