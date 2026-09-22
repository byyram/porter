#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#define TFT_CS   5
#define TFT_DC   17
#define TFT_RST  16

#define SPI_MODE_TO_TRY 0

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
const char *names[] = {"RED", "GREEN", "BLUE", "WHITE", "BLACK"};
uint16_t colors[] = {ST77XX_RED, ST77XX_GREEN, ST77XX_BLUE,
                     ST77XX_WHITE, ST77XX_BLACK};
int i = 0;

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println();
  Serial.println("screen_test: sketch is running");
  Serial.printf("screen_test: CS=%d DC=%d RST=%d, SPI mode %d\n",
                TFT_CS, TFT_DC, TFT_RST, SPI_MODE_TO_TRY);

#if SPI_MODE_TO_TRY == 3
  tft.init(240, 320, SPI_MODE3);
#else
  tft.init(240, 320);
#endif

  tft.setSPISpeed(10000000);
  tft.setRotation(1);

  Serial.println("screen_test: init returned. Watch the panel now.");
}

void loop() {
  tft.fillScreen(colors[i]);
  Serial.printf("screen_test: filled %s\n", names[i]);

  if (colors[i] != ST77XX_WHITE) {
    tft.fillRect(0, 0, 30, 30, ST77XX_WHITE);
  }

  i = (i + 1) % 5;
  delay(1000);
}
