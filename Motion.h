#ifndef MOTION_H
#define MOTION_H

#include <BMI160Gen.h>

#include "Colors.h"

struct Object {
  float x, y;
  float vx, vy;
  uint32_t color;
};

const int motion_i2c_addr = 0x69;
Object* motionObjects = nullptr;
int motionNumObjects = 3;

float pitch = 0, roll = 0, yaw = 0;
int gx = 0, gy = 0, gz = 0;

void initializeMotion(int maxX, int maxY) {
  BMI160.begin(BMI160GenClass::I2C_MODE, motion_i2c_addr);
  motionObjects = new Object[motionNumObjects];
  for (int i = 0; i < motionNumObjects; i++) {
    motionObjects[i].color = colorPallet[random(0, palletSize)];
    motionObjects[i].x = random(0, maxX);
    motionObjects[i].y = random(0, maxY);
  }
}

float convertRawGyro(int gRaw) {
  // since we are using 250 degrees/seconds range
  // -250 maps to a raw value of -32768
  // +250 maps to a raw value of 32767
  float g = (gRaw * 250.0) / 32768.0;
  return g;
}

void readSensors() {
  // Read current time
  static unsigned long lastUpdateTime = millis();
  unsigned long currentTime = millis();
  float deltaTime =
      (currentTime - lastUpdateTime) / 1000.0;  // Convert to seconds

  int gxRaw, gyRaw, gzRaw;
  BMI160.readGyro(gxRaw, gyRaw, gzRaw);

  // convert the raw gyro data to degrees/second
  gx = convertRawGyro(gxRaw);
  gy = convertRawGyro(gyRaw);
  gz = convertRawGyro(gzRaw);

  // Update rotation angles based on gyro data and time elapsed
  // map proper axis to proper angle
  pitch += gy * deltaTime;
  roll += gz * deltaTime;
  yaw += gx * deltaTime;

  // Update last update time
  lastUpdateTime = currentTime;

  Serial.print("g:\t");
  Serial.print(gx);
  Serial.print("\t");
  Serial.print(gy);
  Serial.print("\t");
  Serial.print(gz);
  Serial.println();

  // Optional: Print the rotation angles
  Serial.print("Pitch: ");
  Serial.print(pitch);
  Serial.print("\tRoll: ");
  Serial.print(roll);
  Serial.print("\tYaw: ");
  Serial.println(yaw);
}

void motionAnimation(int maxX, int maxY) {
  readSensors();
  float scale = 0.01;
  for (int i = 0; i < motionNumObjects; i++) {
    motionObjects[i].vx += gx * scale;
    motionObjects[i].vy += gy * scale;

    motionObjects[i].x += motionObjects[i].vx;
    motionObjects[i].y += motionObjects[i].vy;
    if (motionObjects[i].x < 0) motionObjects[i].x = 0;
    if (motionObjects[i].x >= maxX) motionObjects[i].x = maxX - 1;
    if (motionObjects[i].y < 0) motionObjects[i].y = 0;
    if (motionObjects[i].y >= maxY) motionObjects[i].y = maxY - 1;
  }
}

#endif  // MOTION_H