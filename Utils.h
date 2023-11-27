#ifndef UTILS_H
#define UTILS_H

#include <Adafruit_NeoMatrix.h>

struct Pixel {
  int x;
  int y;
  byte intensity;
};

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