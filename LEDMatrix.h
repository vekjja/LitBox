#ifndef LITBOX_LED_MATRIX_H
#define LITBOX_LED_MATRIX_H

#include <NeoPixelBus.h>

#include "colors.h"

#define LED_PIN 12
#define LED_WIDTH 32
#define LED_HEIGHT 8
#define NUM_LEDS (LED_WIDTH * LED_HEIGHT)

// Use the RMT method for WS2812B LEDs
NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0800KbpsMethod> matrix(NUM_LEDS, LED_PIN);

// Brightness
const uint8_t minBrightness = 1;
const uint8_t maxBrightness = 255;
uint8_t brightness = 9;

void initializeMatrix() {
  matrix.Begin();
  matrix.Show();
  Serial.println("LED Matrix Initialized");
}

uint16_t XY(uint8_t x, uint8_t y) {
  y = LED_HEIGHT - 1 - y;  // Adjust y for the matrix orientation
  if (x % 2 == 0) {
    return x * LED_HEIGHT + y;
  } else {
    return x * LED_HEIGHT + (LED_HEIGHT - 1 - y);
  }
}

void drawPixel(uint8_t x, uint8_t y, RgbColor color) {
  uint16_t index = XY(x, y);
  if (index < NUM_LEDS) {
    matrix.SetPixelColor(index, color.Dim(brightness));
  }
}

void testMatrix() {
  RgbColor testColor = pixelColor;

  for (int x = 0; x < LED_WIDTH; x++) {
    for (int y = 0; y < LED_HEIGHT; y++) {
      drawPixel(x, y, testColor);
      matrix.Show();
      delay(10);
      drawPixel(x, y, RgbColor(0, 0, 0));
    }
  }

  int del = 100;
  // bottom left
  matrix.ClearTo(BLACK);
  drawPixel(0, 0, testColor);
  matrix.Show();
  delay(del);

  // top left
  matrix.ClearTo(BLACK);
  drawPixel(0, LED_HEIGHT - 1, testColor);
  matrix.Show();
  delay(del);

  // top right
  matrix.ClearTo(BLACK);
  drawPixel(LED_WIDTH - 1, LED_HEIGHT - 1, testColor);
  matrix.Show();
  delay(del);

  // bottom right
  matrix.ClearTo(BLACK);
  drawPixel(LED_WIDTH - 1, 0, testColor);
  matrix.Show();
  delay(del);

  // middle
  matrix.ClearTo(BLACK);
  drawPixel(LED_WIDTH / 2, LED_HEIGHT / 2, testColor);
  matrix.Show();
  delay(del);

  // clear
  matrix.ClearTo(BLACK);
  matrix.Show();
}

#endif  // LITBOX_LED_MATRIX_H