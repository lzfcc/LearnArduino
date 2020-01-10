#include <M5StickC.h>

char incomingByte = 0;

void setup() {
  M5.begin();
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    incomingByte = (char)Serial.read();
    // used for testing on serial moniter
//    Serial.print("Received: ");
//    Serial.println(incomingByte);
    M5.Lcd.print(incomingByte);
  }
