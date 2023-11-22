#ifndef BIRDS_H
#define BIRDS_H
#include "Utils.h"

// Birds
struct Bird {
  Pixel pixel;
  int vx, vy;      // Velocity
  uint32_t color;  // Color
};

// Birds Config
int birdCount = 9;
Bird* birds = nullptr;
float birdAlignment = 9;
float birdCohesion = 999;
float birdMaxVelocity = 9;
float birdMinVelocity = 0;
float birdSeparation = 1;
int birdEdgeBuffer = 1;  // Distance from edge to start avoiding
float birdRandomVelocityChangeFactor = 1;  // Max random change in velocity
int birdRandomChangeChance = 3;  // Chance of random change (in percentage)

void updateFlock(int maxX, int maxY) {
  for (int i = 0; i < birdCount; i++) {
    float avgVx = 0;
    float avgVy = 0;
    float avgX = 0;
    float avgY = 0;
    int count = 0;

    for (int j = 0; j < birdCount; j++) {
      if (i != j) {
        int distanceX = birds[i].pixel.x - birds[j].pixel.x;
        int distanceY = birds[i].pixel.y - birds[j].pixel.y;
        int distanceSquared = distanceX * distanceX + distanceY * distanceY;

        if (distanceSquared < birdSeparation * birdSeparation) {
          avgVx -= distanceX;
          avgVy -= distanceY;
        } else if (distanceSquared < birdAlignment * birdAlignment) {
          avgVx += birds[j].vx;
          avgVy += birds[j].vy;
          count++;
        }
      }
    }

    if (count > 0) {
      avgVx /= count;
      avgVy /= count;
      avgX /= count;
      avgY /= count;

      // Add cohesion
      float cohesionVx = (avgX - birds[i].pixel.x) / birdCohesion;
      float cohesionVy = (avgY - birds[i].pixel.y) / birdCohesion;

      birds[i].vx += avgVx * 0.1 + cohesionVx;
      birds[i].vy += avgVy * 0.1 + cohesionVy;
    }

    // Edge Collision Avoidance for Top and Bottom Edges
    if (birds[i].pixel.y <= birdEdgeBuffer) {
      birds[i].vy += 1;  // Steer down
    } else if (birds[i].pixel.y >= maxY - birdEdgeBuffer) {
      // birds[i].vy -= 1;  // Steer up
      birds[i].vy = 0;  // land on the ground
    }

    // Edge Collision Avoidance for Left and Right Edges
    if (birds[i].pixel.x <= birdEdgeBuffer) {
      birds[i].vx += 1;  // Steer right
    } else if (birds[i].pixel.x >= maxX - birdEdgeBuffer - 1) {
      birds[i].vx -= 1;  // Steer left
    }

    // Random Velocity Change
    if (random(100) < birdRandomChangeChance) {
      birds[i].vx += random(-birdRandomVelocityChangeFactor,
                            birdRandomVelocityChangeFactor);
      birds[i].vy += random(-birdRandomVelocityChangeFactor,
                            birdRandomVelocityChangeFactor);
    }

    // Limiting velocity
    float speed = sqrt(birds[i].vx * birds[i].vx + birds[i].vy * birds[i].vy);
    if (speed > birdMaxVelocity) {
      birds[i].vx = (birds[i].vx / speed) * birdMaxVelocity;
      birds[i].vy = (birds[i].vy / speed) * birdMaxVelocity;
    } else if (speed < birdMinVelocity) {
      birds[i].vx = (birds[i].vx / speed) * birdMinVelocity;
      birds[i].vy = (birds[i].vy / speed) * birdMinVelocity;
    } else if (birds[i].pixel.y < maxY - 1 && speed == birdMinVelocity) {
      birds[i].vx += random(-birdRandomVelocityChangeFactor,
                            birdRandomVelocityChangeFactor);
      birds[i].vy += random(-birdRandomVelocityChangeFactor,
                            birdRandomVelocityChangeFactor);
    }

    // Update position with vertical boundary check
    birds[i].pixel.y += birds[i].vy;
    if (birds[i].pixel.y < 0) {
      birds[i].pixel.y = 0;
    } else if (birds[i].pixel.y >= maxY) {
      birds[i].pixel.y = maxY - 1;
    }

    // Update position with horizontal boundary check
    birds[i].pixel.x += birds[i].vx;
    if (birds[i].pixel.x < 0) {
      birds[i].pixel.x = 0;
    } else if (birds[i].pixel.x >= maxX) {
      birds[i].pixel.x = maxX - 1;
    }
  }
}

void hatchBirds(int maxX, int maxY, uint32_t colorPallet[]) {
  int palletSize = sizeof(colorPallet) / sizeof(colorPallet[0]);
  for (int i = 0; i < birdCount; i++) {
    uint32_t birdColor = colorPallet[random(0, palletSize - 1)];
    birds[i].pixel.x = random(0, maxX);
    birds[i].pixel.y = random(0, maxY);
    birds[i].pixel.intensity = random(100, 255);  // Random intensity
    birds[i].vx = random(0, 3);                   // Random velocity X
    birds[i].vy = random(0, 3);                   // Random velocity Y
    birds[i].color = birdColor;  // Function to generate a random color
  }
}

void generateBirds(int maxX, int maxY, uint32_t colorPallet[]) {
  birds = new Bird[birdCount];
  hatchBirds(maxX, maxY, colorPallet);
}
#endif