#ifndef LITBOX_MATRIX_H
#define LITBOX_MATRIX_H

#include <FastLED.h>

// LED Matrix config
#define LED_PIN 12
#define LED_WIDTH 32
#define LED_HEIGHT 8
#define NUM_LEDS (LED_WIDTH * LED_HEIGHT)

CRGB leds[NUM_LEDS];

// Brightness Config
const int maxBrightness = 255;
const int minBrightness = 1;
int brightness = 18;

void clearMatrix() { fill_solid(leds, NUM_LEDS, CRGB::Black); }
void fillMatrix(CRGB color) { fill_solid(leds, NUM_LEDS, color); }

void initializeMatrix() {
  // Initialize FastLED
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS)
      .setCorrection(TypicalSMD5050);
  FastLED.setBrightness(brightness);
  clearMatrix();
  FastLED.show();
}

// Helper function to map 2D coordinates to 1D array index
uint16_t XY(uint8_t x, uint8_t y) {
  // Flip y-coordinate for bottom-left origin
  uint8_t adjustedY = LED_HEIGHT - 1 - y;
  if (x % 2 == 0) {
    // Even columns go bottom to top
    return (x * LED_HEIGHT) + adjustedY;
  } else {
    // Odd columns go top to bottom
    return (x * LED_HEIGHT) + y;
  }
}

void testMatrix(CRGB* leds) {
  clearMatrix();
  CRGB::HTMLColorCode testColor = CRGB::Gray;
  // loop through x and y to light up the entire matrix
  for (int x = 0; x < LED_WIDTH; x++) {
    for (int y = 0; y < LED_HEIGHT; y++) {
      leds[XY(x, y)] = testColor;
      FastLED.show();
      delay(10);
      leds[XY(x, y)] = CRGB::Black;
    }
  }
}

#endif  // LITBOX_MATRIX_H