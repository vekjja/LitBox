#ifndef LITBOX_COLORS_H
#define LITBOX_COLORS_H

#include <FastLED.h>

struct Pixel {
  float x = 0, y = 0;
  float vx = 0, vy = 0;
  CRGB color = CRGB(0, 0, 0);
};

// Palette and Configurations
const int palletSize = 4;
CRGB pixelColor = CRGB(255, 255, 255);  // Default color
CRGB pixelBgColor = CRGB(0, 0, 0);      // Default background color
CRGB colorPallet[palletSize] = {CRGB(0, 0, 255), CRGB(0, 255, 255),
                                CRGB(148, 0, 211), CRGB(255, 255, 255)};

#endif  // LITBOX_COLORS_H
