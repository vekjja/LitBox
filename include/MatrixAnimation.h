#ifndef MATRIX_ANIMATION_H
#define MATRIX_ANIMATION_H

#include "Colors.h"
#include "LEDMatrix.h"
#include <Arduino.h>

// Matrix Animation Configuration
struct MatrixColumn : public Pixel {
  int speed;  // Speed of the column
  int length; // Length of the trail
};

MatrixColumn *matrixColumns = nullptr;
const int matCharHeight = 8;     // Character height
const int matMinColSpeed = 1;    // Minimum column speed
const int matMaxColSpeed = 3;    // Maximum column speed
const int matMaxTailLength = 15; // Maximum tail length

void initializeMatrixAnimation(int maxX, int maxY) {
  matrixColumns = new MatrixColumn[maxX];
  for (int i = 0; i < maxX; i++) {
    matrixColumns[i].x = i;
    matrixColumns[i].y =
        random(maxY, maxY + matCharHeight); // Start from bottom
    matrixColumns[i].speed = random(matMinColSpeed, matMaxColSpeed + 1);
    matrixColumns[i].length =
        random(5, matMaxTailLength + 1); // Random length for each column
  }
}

void matrixAnimation(int maxX, int maxY) {
  if (matrixColumns == nullptr) {
    initializeMatrixAnimation(maxX, maxY);
  }
  FastLED.clear(); // Clear the matrix before drawing
  for (int i = 0; i < maxX; i++) {
    // Bright white leading pixel
    int headY = matrixColumns[i].y;
    if (headY >= 0 && headY < maxY) {
      drawPixel(matrixColumns[i].x, headY, CRGB(255, 255, 255));
    }

    // Green tail with very gradual fade
    for (int j = 1; j <= matrixColumns[i].length; j++) {
      int tailY = headY + j;
      if (tailY >= 0 && tailY < maxY) {
        // More gradual fade - start brighter and fade to black
        uint8_t brightness = map(j, 1, matrixColumns[i].length, 180, 0);
        brightness =
            constrain(brightness, 0, 180); // Ensure it stays in valid range
        drawPixel(matrixColumns[i].x, tailY, CRGB(0, brightness, 0));
      }
    }
    // Move upward (flow downward effect)
    matrixColumns[i].y -= matrixColumns[i].speed;
    // Reset the column when it goes off the top of the screen
    if (matrixColumns[i].y + matrixColumns[i].length < 0) {
      matrixColumns[i].y = maxY + matCharHeight;
      matrixColumns[i].speed = random(matMinColSpeed, matMaxColSpeed + 1);
      matrixColumns[i].length = random(5, matMaxTailLength + 1);
    }
  }
  FastLED.show(); // Show the updated matrix
}

#endif // MATRIX_ANIMATION_H