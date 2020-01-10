#include <M5StickC.h> 

int colors[] = {
  0xfd79, 0xe8e4, 0xfbe4, 0xff80, 0x2589,
  0x051d, 0x3a59, 0xa254
};

char const *colorNames[] = {
  "Pink", "Red", "Orange", "Yellow", "Green", "Blue",
  "Dark Blue", "Violet"
};

// pure R(5 bits), G(6 bits), B(5 bits)
//int colors[] = {0xf800, 0x7e0, 0x1f};

int size = 0;
int i = 0;

void setup() {
  M5.begin();
  size = sizeof(colors) / sizeof(int);
  
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE);
}

void loop() {
  // clear only text...Any good way?
  M5.Lcd.fillRect(0, 0, 80, 40, BLACK);
  M5.Lcd.fillCircle(35, 80, 30, colors[i]);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("%s", colorNames[i]);
  ++i;
  if (i >= size) i = 0;
  delay(1000);
}
