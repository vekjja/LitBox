#ifndef LITBOX_COLORS_H
#define LITBOX_COLORS_H

#include <ArduinoJson.h>
#include <NeoPixelBus.h>

// Default Colors using NeoPixelBus's RgbColor
RgbColor BLACK(0, 0, 0);
RgbColor BLUE(0, 0, 255);
RgbColor RED(255, 0, 0);
RgbColor GREEN(0, 255, 0);
RgbColor CYAN(0, 255, 255);
RgbColor PURPLE(128, 0, 128);
RgbColor GRAY(128, 128, 128);
RgbColor MAGENTA(255, 0, 255);
RgbColor YELLOW(255, 255, 0);
RgbColor WHITE(255, 255, 255);
RgbColor PINK(255, 192, 203);
RgbColor ORANGE(255, 165, 0);
RgbColor TEAL(0, 128, 128);
RgbColor VIOLET(148, 0, 211);
RgbColor OLIVE(128, 128, 0);
RgbColor GOLD(255, 215, 0);
RgbColor SILVER(192, 192, 192);
RgbColor DARK_GREEN(0, 100, 0);
RgbColor FOREST_GREEN(34, 139, 34);
RgbColor CORAL(255, 127, 80);
RgbColor SALMON(250, 128, 114);
RgbColor ROSE(255, 228, 225);
RgbColor PEACH(255, 218, 185);

// Palette and Configurations
const int palletSize = 4;
RgbColor pixelColor = WHITE;    // Default color
RgbColor pixelBgColor = BLACK;  // Default background color
RgbColor colorPallet[palletSize] = {BLUE, CYAN, VIOLET, WHITE};

#endif  // LITBOX_COLORS_H
