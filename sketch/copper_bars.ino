void setup() {
  // Run standard startup procedure
  standardStartup();

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
  
  // Shut down if button is pressed
  if(digitalRead(BUTTON)) standardShutdown();
  
}