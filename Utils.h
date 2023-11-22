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

  // Convert the hex string to a long integer
  unsigned long number = strtoul(hexColor.c_str(), NULL, 16);

  // Split into RGB components
  int r = (number >> 16) & 0xFF;
  int g = (number >> 8) & 0xFF;
  int b = number & 0xFF;

  // Reorder the components to match the GRB format for WS2812B LEDs
  return ((g & 0xFF) << 16) | ((r & 0xFF) << 8) | (b & 0xFF);
}

void runAtFrameRate(void (*functionToRun)(), unsigned int fps) {
  static unsigned long lastFrameTime = 0;
  static unsigned int frameDuration = 0;
  if (fps != 0) {
    frameDuration = 1000 / fps;
  }

  unsigned long currentTime = millis();
  if (currentTime - lastFrameTime >= frameDuration) {
    lastFrameTime = currentTime;
    functionToRun();  // Call the passed function
  }
}

// void testMatrix(Adafruit_NeoMatrix matrix, int ledColumns, int ledRows) {
//   Serial.println("Begin Testing LED Matrix");
//   int testDelay = 1000;
//   int pixelColor = 0xFFFFFF;
//   // loop through each pixel from bottom left to top right
//   Serial.println("Looping through each pixel from bottom left to top right");
//   for (int x = 0; x < ledColumns; x++) {
//     for (int y = 0; y < ledRows; y++) {
//       matrix.fillScreen(0);
//       matrix.drawPixel(x, y, pixelColor);
//       matrix.show();
//       delay(1);
//     }
//   }
// }
#endif