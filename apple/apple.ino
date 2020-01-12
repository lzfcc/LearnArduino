#include "xbm.h"             // Sketch tab header for xbm images
#include <M5StickC.h>        // Hardware-specific library

const int videoWidth = 96;
const int videoHeight = 72;
const int IMG_BYTES = (videoWidth * videoHeight) >> 3;

unsigned char image[IMG_BYTES] = {0};

int byteIndex = 0;

void setup() {
  M5.begin();               // Initialise the display

  M5.Lcd.fillScreen(0xa514); // gray
  M5.Lcd.setRotation(3);
 
  M5.Lcd.setCursor(0, 10);
  M5.Lcd.setTextColor(BLUE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.print("Bad");
  M5.Lcd.drawXBitmap(0, 30, apple, iconWidth, iconHeight, TFT_RED);
}

void loop() {
  if (Serial.available() > 0) {
    image[byteIndex++] = (char)Serial.read();
    if (byteIndex >= IMG_BYTES) {
      // erase last frame
      //M5.Lcd.drawXBitmap(60, 0, image, videoWidth, videoHeight, TFT_BLACK, TFT_BLACK);
      // draw current frame
      M5.Lcd.drawXBitmap(60, 0, image, videoWidth, videoHeight, TFT_WHITE, TFT_BLACK);
      byteIndex = 0;
    }
  }
}
