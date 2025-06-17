#ifndef BIRDS_H
#define BIRDS_H

#include "Colors.h"

// Birds Config
int birdCount = 18;
Pixel* birds = nullptr;
float birdAlignment = 9;
float birdSeparation = 1;
float birdCohesion = 999;
float birdMaxVelocity = 3;
float birdMinVelocity = 0;
bool birdVerticalBounds = true;
bool birdHorizontalBounds = true;
int birdEdgeBuffer = 1;  // Distance from edge to start avoiding
float birdRandomVelocityChangeFactor = 3;  // Max random change in velocity
int birdRandomChangeChance = 3;  // Chance of random change (in percentage)

void generateBirds(int maxX, int maxY) {
  birds = new Pixel[birdCount];
  for (int i = 0; i < birdCount; i++) {
    CRGB birdColor = colorPallet[random(0, palletSize - 1)];
    birds[i].x = random(0, maxX);
    birds[i].y = random(0, maxY);
    birds[i].vx = random(0, 3);  // Random velocity X
    birds[i].vy = random(0, 3);  // Random velocity Y
    birds[i].color = birdColor;  // Function to generate a random color
  }
}

void updateFlock(int maxX, int maxY) {
  if (birds == nullptr) {
    generateBirds(maxX, maxY);
  }
  for (int i = 0; i < birdCount; i++) {
    float avgVx = 0;
    float avgVy = 0;
    float avgX = 0;
    float avgY = 0;
    int count = 0;

    for (int j = 0; j < birdCount; j++) {
      if (i != j) {
        int distanceX = birds[i].x - birds[j].x;
        int distanceY = birds[i].y - birds[j].y;
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
      float cohesionVx = (avgX - birds[i].x) / birdCohesion;
      float cohesionVy = (avgY - birds[i].y) / birdCohesion;

      birds[i].vx += avgVx * 0.1 + cohesionVx;
      birds[i].vy += avgVy * 0.1 + cohesionVy;
    }

    // Edge Collision Avoidance for Top and Bottom Edges
    if (birdVerticalBounds) {
      if (birds[i].y <= birdEdgeBuffer) {
        birds[i].vy += 1;  // Steer down
      } else if (birds[i].y >= maxY - birdEdgeBuffer) {
        // birds[i].vy -= 1;  // Steer up
        birds[i].vy = 0;  // land on the ground
      }
    }

    // Edge Collision Avoidance for Left and Right Edges
    if (birdHorizontalBounds) {
      if (birds[i].x <= birdEdgeBuffer) {
        birds[i].vx += 1;  // Steer right
      } else if (birds[i].x >= maxX - birdEdgeBuffer - 1) {
        birds[i].vx -= 1;  // Steer left
      }
    }

    // Random Velocity Change
    if (random(100) < birdRandomChangeChance) {
      birds[i].vx += random(-birdRandomVelocityChangeFactor,
                            birdRandomVelocityChangeFactor);
      birds[i].vy += random(-birdRandomVelocityChangeFactor,
                            birdRandomVelocityChangeFactor);
    }

    // Create the rules for the birds
    float velocity =
        sqrt(birds[i].vx * birds[i].vx + birds[i].vy * birds[i].vy);
    if (velocity > birdMaxVelocity) {  // Limit velocity
      birds[i].vx = (birds[i].vx / velocity) * birdMaxVelocity;
      birds[i].vy = (birds[i].vy / velocity) * birdMaxVelocity;
    } else if (birds[i].y < maxY - 1 && velocity == birdMinVelocity) {
      // keep flying if not on the ground
      birds[i].vx += random(-birdRandomVelocityChangeFactor,
                            birdRandomVelocityChangeFactor);
      birds[i].vy += random(-birdRandomVelocityChangeFactor,
                            birdRandomVelocityChangeFactor);
    }

    // Update position with vertical boundary check or wrap
    birds[i].y += birds[i].vy;
    if (birdVerticalBounds) {
      if (birds[i].y < 0) {  // Vertical boundary check
        birds[i].y = 0;
      } else if (birds[i].y >= maxY) {
        birds[i].y = maxY - 1;
      }
    } else {
      if (birds[i].y < 0) {  // Vertical wrap
        birds[i].y = maxY - 1;
      } else if (birds[i].y >= maxY) {
        birds[i].y = 0;
      }
    }

    // Update position
    birds[i].x += birds[i].vx;
    if (birdHorizontalBounds) {
      if (birds[i].x < 0) {  // Horizontal boundary check
        birds[i].x = 0;
      } else if (birds[i].x >= maxX) {
        birds[i].x = maxX - 1;
      }
    } else {
      if (birds[i].x < 0) {  // Horizontal wrap
        birds[i].x = maxX - 1;
      } else if (birds[i].x >= maxX) {
        birds[i].x = 0;
      }
    }
  }
}

#endif  // BIRDS_H