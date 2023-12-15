#ifndef MATRIX_ANIMATION_H
#define MATRIX_ANIMATION_H

#include <Adafruit_NeoMatrix.h>

// Matrix Animation Configuration
struct MatrixColumn {
  int x;       // Column X position
  int y;       // Current Y position of the leading character
  int speed;   // Speed of the column
  int length;  // Length of the trail
};

MatrixColumn* matrixColumns = nullptr;
const int matCharHeight = 8;      // Character height
const int matMinColSpeed = 1;     // Minimum column speed
const int matMaxColSpeed = 3;     // Maximum column speed
const int matMaxTailLength = 15;  // Maximum tail length

void initializeMatrixAnimation(int maxX) {
  matrixColumns = new MatrixColumn[maxX];
  for (int i = 0; i < maxX; i++) {
    matrixColumns[i].x = i;
    matrixColumns[i].y = random(-matCharHeight, 0);
    matrixColumns[i].speed = random(matMinColSpeed, matMaxColSpeed + 1);
    matrixColumns[i].length =
        random(5, matMaxTailLength + 1);  // Random length for each column
  }
}

void matrixAnimation(Adafruit_NeoMatrix* matrix, int maxX, int maxY) {
  if (matrixColumns == nullptr) {
    initializeMatrixAnimation(maxX);
  }
  matrix->fillScreen(0);
  for (int i = 0; i < maxX; i++) {
    // White leading pixel
    int headY = matrixColumns[i].y;
    if (headY >= 0 && headY < maxY) {
      matrix->drawPixel(matrixColumns[i].x, headY,
                        matrix->Color(255, 255, 255));
    }

    // Green tail with randomly varying brightness
    for (int j = 1; j <= matrixColumns[i].length; j++) {
      int tailY = headY - j;
      if (tailY >= 0 && tailY < maxY) {
        uint8_t brightness =
            random(90, 255);  // Random brightness for each pixel
        matrix->drawPixel(matrixColumns[i].x, tailY,
                          matrix->Color(0, brightness, 0));
      }
    }

    matrixColumns[i].y += matrixColumns[i].speed;
    // Reset the column when it goes off the screen
    if (matrixColumns[i].y - matrixColumns[i].length > maxY) {
      matrixColumns[i].y = -matCharHeight;
      matrixColumns[i].speed = random(matMinColSpeed, matMaxColSpeed + 1);
      matrixColumns[i].length = random(5, matMaxTailLength + 1);
    }
  }
  matrix->show();
}

#endif  // MATRIX_ANIMATION_H
