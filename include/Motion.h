#ifndef MOTION_H
#define MOTION_H

#include <2D/Body.h>
#include <2D/World.h>

#include "Colors.h"

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
    motionObjects[i].color = colorPallet[random(0, palletSize)];
    Body *b = new Body();
    float w = 1.0f;
    b->Set(Vec2{w, w}, 10.0f);
    b->position.Set(static_cast<float>(random(0, maxX)),
                    static_cast<float>(random(0, maxY)));
    b->friction = 0.3f;
    world.Add(motionObjects[i].body = b);
  }
}

void motionStep(int maxX, int maxY, ESPWiFi &device) {
  if (motionObjects == nullptr) {
    generateMotionObjects(maxX, maxY);
  }

  device.readAccelerometer(ax, ay, az);

  // Serial.println("g: " + String(-ay) + ", " + String(-ax));
  world.gravity.Set(-ay, -ax);
  world.Step(0.8f);

  for (int i = 0; i < motionNumObjects; i++) {
    Body *b = motionObjects[i].body;
    b->position.x = constrain(b->position.x, 0, maxX - 1);
    b->position.y = constrain(b->position.y, 0, maxY - 1);
    b->rotation = 0.0f;
  }
}

#endif  // MOTION_H