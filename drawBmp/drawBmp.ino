#include <M5StickC.h>        // Hardware-specific library
#include "portrait.h"

void setup() {
  M5.begin();               // Initialise the display
  M5.Lcd.fillScreen(BLACK); // Black screen fill
  M5.Lcd.drawBitmap(0, 0, imageWidth, imageHeight, image);
}

void loop() {
}
