#include <M5StickC.h>
//#include "hop.h"

#include "minilzo.h"
#include "lendata.h"

#define IN_LEN      (16*1024ul)
#define OUT_LEN     (IN_LEN + IN_LEN / 16 + 64 + 3)

unsigned char __LZO_MMODEL in  [ IN_LEN ];
unsigned char __LZO_MMODEL out [ OUT_LEN ];

const int videoWidth = 144;
const int videoHeight = 80;
const int IMG_BYTES = videoWidth * videoHeight;
uint16_t image[IMG_BYTES] = {0};

int FRAME_NUM = 0;
int byteIndex = 0;
int frameIndex = 0;

void setup() {
  M5.begin();               // Initialise the display

  M5.Lcd.setRotation(3);
//  M5.Lcd.drawBitmap(0, 0, videoWidth, videoHeight, gImage_hop);

  FRAME_NUM = sizeof(lengths) / sizeof(int);
}

void loop() {
  if (Serial.available() > 0) {
    out[byteIndex] = (uint8_t)Serial.read();
    if (++byteIndex >= lengths[frameIndex]) {
//      pinMode(GPIO_NUM_10, OUTPUT);
//      digitalWrite(GPIO_NUM_10, LOW);
//      delay(200);
//      digitalWrite(GPIO_NUM_10, HIGH);

      lzo_uint new_len = lengths[frameIndex];
      lzo1x_decompress(out, OUT_LEN, in, &new_len, NULL);

      for (int i = 0; i < IMG_BYTES; i++) {
        uint16_t trueColor = M5.Lcd.color8to16(in[i]);
        M5.Lcd.drawPixel(i % videoWidth, i / videoWidth, trueColor);
      }

      byteIndex = 0;
      if (++frameIndex > FRAME_NUM) {
        frameIndex = 0;
      }
    }
  }
//  delay(200);
}
