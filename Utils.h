#ifndef UTILS_H
#define UTILS_H
#include <Adafruit_NeoMatrix.h>

struct Pixel {
  int x;
  int y;
  byte intensity;
};

uint32_t hexToColor(String hexColor) {
  // Remove the '#' character if present
  if (hexColor.startsWith("#")) {
    hexColor = hexColor.substring(1);
  }

  // Convert the hex string to an integer
  unsigned long number = strtoul(hexColor.c_str(), NULL, 16);

  // Extract RGB components
  uint8_t r = (number >> 16) & 0xFF;  // Extract the RR byte
  uint8_t g = (number >> 8) & 0xFF;   // Extract the GG byte
  uint8_t b = number & 0xFF;          // Extract the BB byte

  // Convert to 5-6-5 format
  uint16_t rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);

  return rgb565;
}

void runAtFrameRate(void (*functionToRun)(), unsigned int fps) {
  static unsigned long lastFrameTime = 0;
  unsigned long currentFrameTime = millis();
  unsigned int frameDuration =
      1000 / fps;  // Duration of each frame in milliseconds

  if (currentFrameTime - lastFrameTime >= frameDuration) {
    lastFrameTime = currentFrameTime;
    functionToRun();
  }
}

void testMatrix(Adafruit_NeoMatrix* matrix, int ledColumns, int ledRows) {
  Serial.println("Begin Testing LED Matrix");
  int pixelColor = 0xFFFF;
  // loop through each pixel from bottom left to top right
  Serial.println("Looping through each pixel from bottom left to top right");
  for (int x = 0; x < ledColumns; x++) {
    for (int y = 0; y < ledRows; y++) {
      matrix->fillScreen(0);
      matrix->drawPixel(x, y, pixelColor);
      matrix->show();
      delay(1);
    }
  }
}
#endif