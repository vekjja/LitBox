#ifndef MOTION_H
#define MOTION_H

#include <BMI160Gen.h>

#include "Colors.h"

Pixel* motionPixels = nullptr;
int motionNumPixels = 3;

int gx = 0, gy = 0, gz = 0;
int ax = 0, ay = 0, az = 0;

const int motion_i2c_addr = 0x69;

void initializeMotion() {
  BMI160.begin(BMI160GenClass::I2C_MODE, motion_i2c_addr);
  motionPixels = new Pixel[motionNumPixels];
  for (int i = 0; i < motionNumPixels; i++) {
    motionPixels[i].color = colorPallet[random(0, palletSize)];
    motionPixels[i].x = random(0, 7);
    motionPixels[i].y = random(0, 7);
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
  int gxRaw, gyRaw, gzRaw;
  int axRaw, ayRaw, azRaw;
  BMI160.readGyro(gxRaw, gyRaw, gzRaw);

  // convert the raw gyro data to degrees/second
  gx = convertRawGyro(gxRaw);
  gy = convertRawGyro(gyRaw);
  gz = convertRawGyro(gzRaw);

  Serial.print("g:\t");
  Serial.print(gx);
  Serial.print("\t");
  Serial.print(gy);
  Serial.print("\t");
  Serial.print(gz);
  Serial.println();
}

void motionAnimation(int maxX, int maxY) {
  readSensors();
  for (int i = 0; i < motionNumPixels; i++) {
    motionPixels[i].x += gz;
    motionPixels[i].y += gy;

    // Boundary conditions
    if (motionPixels[i].x < 0) motionPixels[i].x = 0;
    if (motionPixels[i].x >= maxX) motionPixels[i].x = maxX - 1;
    if (motionPixels[i].y < 0) motionPixels[i].y = 0;
    if (motionPixels[i].y >= maxY) motionPixels[i].y = maxY - 1;
  }
}

#endif  // MOTION_H