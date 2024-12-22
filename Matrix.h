#include <NeoPixelBus.h>

#define LED_PIN 12
#define LED_WIDTH 32
#define LED_HEIGHT 8
#define NUM_LEDS (LED_WIDTH * LED_HEIGHT)

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(NUM_LEDS, LED_PIN);

void initializeMatrix() {
  strip.Begin();
  strip.Show();
}

void clearMatrix() { strip.ClearTo(RgbColor(0, 0, 0)); }

uint16_t XY(uint8_t x, uint8_t y) {
  y = LED_HEIGHT - 1 - y;  // Adjust y for the matrix orientation
  if (x % 2 == 0) {
    return x * LED_HEIGHT + y;
  } else {
    return x * LED_HEIGHT + (LED_HEIGHT - 1 - y);
  }
}

void setPixelColor(uint8_t x, uint8_t y, RgbColor color) {
  uint16_t index = XY(x, y);
  if (index < NUM_LEDS) {
    strip.SetPixelColor(index, color);
  }
}

void testMatrix() {
  RgbColor testColor(128, 128, 128);

  for (int x = 0; x < LED_WIDTH; x++) {
    for (int y = 0; y < LED_HEIGHT; y++) {
      setPixelColor(x, y, testColor);
      strip.Show();
      delay(10);
      setPixelColor(x, y, RgbColor(0, 0, 0));
    }
  }

  int del = 100;
  // bottom left
  clearMatrix();
  setPixelColor(0, 0, testColor);
  strip.Show();
  delay(del);

  // top left
  clearMatrix();
  setPixelColor(0, LED_HEIGHT - 1, testColor);
  strip.Show();
  delay(del);

  // top right
  clearMatrix();
  setPixelColor(LED_WIDTH - 1, LED_HEIGHT - 1, testColor);
  strip.Show();
  delay(del);

  // bottom right
  clearMatrix();
  setPixelColor(LED_WIDTH - 1, 0, testColor);
  strip.Show();
  delay(del);

  // middle
  clearMatrix();
  setPixelColor(LED_WIDTH / 2, LED_HEIGHT / 2, testColor);
  strip.Show();
  delay(del);

  // clear
  clearMatrix();
  strip.Show();
}
