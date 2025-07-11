#ifndef MOTION_H
#define MOTION_H

#include <2D/Body.h>
#include <2D/World.h>
#include <math.h>

#include "LED/Colors.h"

struct MotionObject {
  Body *body;
  CRGB color;
  int colorPaletteIndex;
};

// Motion Animation
int motionNumObjects = 9;
MotionObject *motionObjects = nullptr;

// BMI160 sensor data
float gx = 0.0, gy = 0.0, gz = 0.0;
float ax = 0.0, ay = 0.0, az = 0.0;

// Physics
bool gravityEnabled = true;
World world(Vec2{0.0, 0.0}, 6);

void generateMotionObjects(int maxX, int maxY) {
  world.Clear();

  float wallFriction = 0.0f;
  Body *ground = new Body();
  ground->Set(Vec2{static_cast<float>(maxX + 3), 1.0f}, FLT_MAX);
  ground->position.Set(maxX / 2.0f, static_cast<float>(maxY));
  ground->friction = wallFriction;
  world.Add(ground);

  Body *ceiling = new Body();
  ceiling->Set(Vec2{static_cast<float>(maxX + 3), 1.0f}, FLT_MAX);
  ceiling->position.Set(maxX / 2.0f, -1.0f);
  ceiling->friction = wallFriction;
  world.Add(ceiling);

  Body *leftWall = new Body();
  leftWall->Set(Vec2{1.0f, static_cast<float>(maxY + 3)}, FLT_MAX);
  leftWall->position.Set(-1.0f, maxY / 2.0f);
  leftWall->friction = wallFriction;
  world.Add(leftWall);

  Body *rightWall = new Body();
  rightWall->Set(Vec2{1.0f, static_cast<float>(maxY + 3)}, FLT_MAX);
  rightWall->position.Set(static_cast<float>(maxX), maxY / 2.0f);
  rightWall->friction = wallFriction;
  world.Add(rightWall);

  motionObjects = new MotionObject[motionNumObjects];
  for (int i = 0; i < motionNumObjects; i++) {
    motionObjects[i].colorPaletteIndex = random(0, palletSize);
    Body *b = new Body();
    b->Set(Vec2{1.1f, 1.0f}, 10.0f);
    b->friction = 0.3f;

    // Try to place the body with minimum separation from others
    bool validPosition = false;
    int attempts = 0;
    const int maxAttempts = 50;
    const float minSeparation = 2.0f; // Minimum distance between body centers

    while (!validPosition && attempts < maxAttempts) {
      float x = static_cast<float>(random(0, maxX));
      float y = static_cast<float>(random(0, maxY));

      validPosition = true;
      // Check distance from existing bodies
      for (int j = 0; j < i; j++) {
        float dx = x - motionObjects[j].body->position.x;
        float dy = y - motionObjects[j].body->position.y;
        float distance = sqrt(dx * dx + dy * dy);
        if (distance < minSeparation) {
          validPosition = false;
          break;
        }
      }

      if (validPosition) {
        b->position.Set(x, y);
      }
      attempts++;
    }

    // If we couldn't find a good position, just place it randomly
    if (!validPosition) {
      b->position.Set(static_cast<float>(random(0, maxX)),
                      static_cast<float>(random(0, maxY)));
    }

    world.Add(motionObjects[i].body = b);
  }
}

void motionStep(int maxX, int maxY, ESPWiFi &device) {
  if (motionObjects == nullptr) {
    generateMotionObjects(maxX, maxY);
  }

  device.readAccelerometer(ax, ay, az);
  // device.readGyroscope(gx, gy, gz);

  // // Apply a force to each object based on the gyroscope or accelerometer
  // values for (int i = 0; i < motionNumObjects; i++) {
  //   Body *b = motionObjects[i].body;
  //   // Example: Add velocity based on gyroscope (shake) values
  //   b->velocity.x += gx * 0.1f; // scale as needed
  //   b->velocity.y += gy * 0.1f; // scale as needed
  // }

  // Serial.println("g: " + String(-ay) + ", " + String(-ax));
  world.gravity.Set(-ay, -ax);
  world.Step(0.3f);

  // Add repulsion between bodies that get too close
  for (int i = 0; i < motionNumObjects; i++) {
    Body *b1 = motionObjects[i].body;
    for (int j = i + 1; j < motionNumObjects; j++) {
      Body *b2 = motionObjects[j].body;
      float dx = b2->position.x - b1->position.x;
      float dy = b2->position.y - b1->position.y;
      float distance = sqrt(dx * dx + dy * dy);

      if (distance > 0.1f && distance < 2.0f) {
        // Add repulsion force
        float force = 0.5f / (distance * distance);
        Vec2 repulsion(dx * force / distance, dy * force / distance);
        b1->AddForce(-repulsion);
        b2->AddForce(repulsion);
      }
    }
  }

  for (int i = 0; i < motionNumObjects; i++) {
    Body *b = motionObjects[i].body;
    b->position.x = constrain(b->position.x, 0, maxX - 1);
    b->position.y = constrain(b->position.y, 0, maxY - 1);
    b->rotation = 0.0f;
  }
}

#endif // MOTION_H