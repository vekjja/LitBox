#ifndef STARS_H
#define STARS_H
#include <cmath>  // for floor()

#include "Motion.h"
#include "colors.h"
struct Star : public Pixel {
  int audioBand;
};

int starCount = 32;
Star* stars = nullptr;

// Function to initialize stars with random values
void initializeStars(int maxX, int maxY) {
  srand(time(0));
  stars = new Star[starCount];
  randomSeed(analogRead(A0));
  for (int i = 0; i < starCount; i++) {
    stars[i].x = random(maxX);
    stars[i].y = random(maxY);
    stars[i].color =
        colorPallet[random(0, palletSize)];  // Random color from the pallet
    stars[i].audioBand = random(8, maxX - 1);
  }
  calibrateAccelerometer();
}

void starWrapper(int maxX, int maxY) {
  // Check if the star is out of bounds
  for (int i = 0; i < starCount; i++) {
    if (stars[i].x < 0) {
      stars[i].x = maxX - 1;
    } else if (stars[i].x > maxX) {
      stars[i].x = 0;
    }

    if (stars[i].y < 0) {
      stars[i].y = maxY - 1;
    } else if (stars[i].y > maxY) {
      stars[i].y = 0;
    }
  }
}

void updateStarField(int maxX, int maxY) {
  if (stars == nullptr) {
    initializeStars(maxX, maxY);
  }

  // Read current accelerometer values
  readAccelerometer();
  readGyro();

  float scale = 0.01;
  float deltaX = -gx * scale;
  float deltaY = -gy * scale;

  // Update star positions based on computed angles
  for (int i = 0; i < starCount; i++) {
    stars[i].x += deltaX;
    // stars[i].y += deltaY;
  }
  starWrapper(maxX, maxY);
}

void updateStartPulse(int maxX, int maxY) {
  if (stars == nullptr) {
    initializeStars(maxX, maxY);
  }
  for (int i = 0; i < starCount; i++) {
    stars[i].x += spectralData[stars[i].audioBand];
  }

  starWrapper(maxX, maxY);
}
#endif  // STARS_H