#ifndef STARS_H
#define STARS_H

#include "Colors.h"

struct Star : public Pixel {
  int audioBand;
};

int starCount = 32;
Star* stars = nullptr;

// Function to initialize stars with random values
void initializeStars(int maxX, int maxY) {
  stars = new Star[starCount];
  randomSeed(analogRead(A0));
  for (int i = 0; i < starCount; i++) {
    stars[i].x = random(maxX);
    stars[i].y = random(maxY);
    stars[i].color =
        colorPallet[random(0, palletSize)];  // Random color from the pallet
    stars[i].audioBand = random(8, maxX - 1);
  }
}

void updateStartPulse(int maxX, int maxY) {
  if (stars == nullptr) {
    initializeStars(maxX, maxY);
  }
  for (int i = 0; i < starCount; i++) {
    stars[i].x += spectralData[stars[i].audioBand] / 2;
  }

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

#endif