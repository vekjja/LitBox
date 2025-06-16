#ifndef LITBOX_COLORS_H
#define LITBOX_COLORS_H

#pragma once
#include <FastLED.h>
// Default Colors using FastLED's CRGB
// CRGB BLACK(0, 0, 0);
// CRGB BLUE(0, 0, 255);
// CRGB RED(255, 0, 0);
// CRGB GREEN(0, 255, 0);
// CRGB CYAN(0, 255, 255);
// CRGB PURPLE(128, 0, 128);
// CRGB GRAY(128, 128, 128);
// CRGB MAGENTA(255, 0, 255);
// CRGB YELLOW(255, 255, 0);
// CRGB WHITE(255, 255, 255);
// CRGB PINK(255, 192, 203);
// CRGB ORANGE(255, 165, 0);
// CRGB TEAL(0, 128, 128);
// CRGB VIOLET(148, 0, 211);
// CRGB OLIVE(128, 128, 0);
// CRGB GOLD(255, 215, 0);
// CRGB SILVER(192, 192, 192);
// CRGB DARK_GREEN(0, 100, 0);
// CRGB FOREST_GREEN(34, 139, 34);
// CRGB CORAL(255, 127, 80);
// CRGB SALMON(250, 128, 114);
// CRGB ROSE(255, 228, 225);
// CRGB PEACH(255, 218, 185);

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
