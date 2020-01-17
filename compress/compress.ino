#include <M5StickC.h>
#include "minilzo.h"

#include "xbm.h"
#include "compressdata.h"

#define IN_LEN      (1*1024ul)
#define OUT_LEN     (IN_LEN + IN_LEN / 16 + 64 + 3)

unsigned char __LZO_MMODEL in  [ IN_LEN ];
unsigned char __LZO_MMODEL out [ OUT_LEN ];

const int videoWidth = 96;
const int videoHeight = 72;

int byteIndex = 0;
int frameIndex = 0;

void setup() {
  M5.begin();               // Initialise the display
  M5.Lcd.fillScreen(0xa514);
  M5.Lcd.setRotation(3);

  M5.Lcd.setCursor(0, 10);
  M5.Lcd.setTextColor(BLUE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.print("Bad");
  M5.Lcd.drawXBitmap(0, 30, apple, iconWidth, iconHeight, TFT_RED);
  
  Serial.begin(115200);
}

void loop() {
  int len = lengths[frameIndex];
  memset(out, 0, OUT_LEN);
  memcpy(out, frames + byteIndex, len);
  
  lzo_uint new_len = IN_LEN;
  lzo1x_decompress(out, len, in, &new_len, NULL);
  
  M5.Lcd.drawXBitmap(60, 0, in, videoWidth, videoHeight, TFT_WHITE, TFT_BLACK);

  byteIndex += len;
  if (++frameIndex >= FRAME_NUM) {
    frameIndex = 0;
    byteIndex = 0;
  }

  Serial.println("frame #" + String(frameIndex) + ", byteIndex #" + String(byteIndex));
  
  delay(167);
}
