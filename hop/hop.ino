#include <M5StickC.h>
#include "hop.h"

const int videoWidth = 144;
const int videoHeight = 80;
const int IMG_BYTES = (videoWidth * videoHeight) << 1;

unsigned char image[IMG_BYTES] = {0};
int byteIndex = 0;
int b2 = 0;

void setup() {
  M5.begin();               // Initialise the display

  M5.Lcd.setRotation(3);

  M5.Lcd.drawBitmap(0, 0, videoWidth, videoHeight, gImage_hop);
}

void loop() {
  if (Serial.available() > 0) {
    image[byteIndex++] = (char)Serial.read();
//    b2 ^= 1;
//    if (b2 == 0) {
//      int x = (byteIndex >> 1) / videoWidth;
//      int y = (byteIndex >> 1) % videoWidth;
//      M5.Lcd.drawPixel(x, y, (image[byteIndex]<<8)&image[byteIndex-1]);
//    }

    if (byteIndex >= IMG_BYTES) {
      M5.Lcd.drawBitmap(0, 0, videoWidth, videoHeight, image);
      byteIndex = 0;
    }
  }
}
