#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#define TFT_CS   5
#define TFT_DC   17
#define TFT_RST  16
const int LED_PIN = 2;


Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

const int W = 320;
const int H = 240;

const int WAVE_TOP = 120;
const int WAVE_H   = 100;
const int WAVE_MID = WAVE_TOP + WAVE_H / 2;

uint16_t BG, INK, DIM, ACCENT;

int col = 0;
unsigned long lastFrame = 0;
unsigned long lastBlink = 0;
bool ledOn = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  tft.init(240, 320);
  tft.setRotation(1);
  tft.setSPISpeed(40000000);

  BG     = ST77XX_BLACK;
  INK    = tft.color565(235, 240, 240);
  DIM    = tft.color565(70, 88, 94);
  ACCENT = tft.color565(86, 199, 207);

  tft.fillScreen(BG);

  tft.setTextColor(INK);
  tft.setTextSize(3);
  tft.setCursor(14, 20);
  tft.print("PORTER");

  tft.setTextColor(DIM);
  tft.setTextSize(1);
  tft.setCursor(15, 54);
  tft.print("stage 0  /  display test  /  synthetic data");

  tft.drawFastHLine(14, 72, W - 28, DIM);

  tft.setTextColor(ACCENT);
  tft.setTextSize(2);
  tft.setCursor(14, 90);
  tft.print("speaking");

  Serial.println("porter: display up");
}

float fakeAmplitude() {
  float t = millis() / 1000.0;
  float envelope = 0.55 + 0.45 * sin(t * 1.7);
  float detail   = 0.5 * sin(t * 23.0) + 0.3 * sin(t * 37.0);
  float noise    = (random(0, 100) / 100.0 - 0.5) * 0.3;

  float v = envelope * (0.6 + 0.4 * detail + noise);
  if (v < 0) v = -v;
  if (v > 1) v = 1;
  return v;
}

void loop() {
  unsigned long now = millis();

  if (now - lastBlink >= 500) {
    lastBlink = now;
    ledOn = !ledOn;
    digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
  }

  if (now - lastFrame >= 20) {
    lastFrame = now;

    int half = (int)(fakeAmplitude() * (WAVE_H / 2));
    if (half < 1) half = 1;

    tft.drawFastVLine(col, WAVE_TOP, WAVE_H, BG);
    tft.drawFastVLine(col, WAVE_MID - half, half * 2, ACCENT);

    int nextCol = (col + 1) % W;
    tft.drawFastVLine(nextCol, WAVE_TOP, WAVE_H, DIM);

    col = nextCol;
  }
}
