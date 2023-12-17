#ifndef MOTION_H
#define MOTION_H

#include <BMI160Gen.h>

#include "Colors.h"
#include "Vector2D.h"

// SDA/D2
// SCL/D1
int motionNumObjects = 3;
bool gravityEnabled = true;
float energyLossFactor = 0.99;
float gx = 0, gy = 0, gz = 0;
float ax = 0, ay = 0, az = 0;
bool BMI160Initialized = false;
Pixel* motionObjects = nullptr;
const int motion_i2c_addr = 0x69;
const float rawDataConversion = 32768.0;

void generateMotionObjects(int maxX, int maxY) {
  motionObjects = new Pixel[motionNumObjects];
  for (int i = 0; i < motionNumObjects; i++) {
    motionObjects[i].color = colorPallet[random(0, palletSize)];
    motionObjects[i].x = random(1, maxX - 1);
    motionObjects[i].y = random(1, maxY - 1);
    motionObjects[i].vx = 0;
    motionObjects[i].vy = 0;
  }
}

void initializeMotion(int maxX, int maxY) {
  if (checkI2CDevice(motion_i2c_addr)) {
    if (BMI160.begin(BMI160GenClass::I2C_MODE, motion_i2c_addr)) {
      generateMotionObjects(maxX, maxY);
      Serial.print("BMI160 Auto Calibration Starting");
      BMI160.autoCalibrateXAccelOffset(1);
      BMI160.autoCalibrateYAccelOffset(1);
      BMI160.autoCalibrateZAccelOffset(1);
      BMI160.autoCalibrateGyroOffset();
      delay(250);
      Serial.print("BMI160 Auto Calibration Complete");
      BMI160Initialized = true;
    } else {
      BMI160Initialized = false;
      Serial.print("BMI160 initialization failed!");
    }
  } else {
    BMI160Initialized = false;
    Serial.print("BMI160 sensor not detected at the specified I2C address:" +
                 String(motion_i2c_addr));
  }
}

float getTemperature(String unit) {
  if (!BMI160Initialized) {
    return 0;  // or some default value or error code
  }
  int16_t rawTemp = BMI160.getTemperature();  // returns a 16-bit integer
  // The temperature data is a signed 16-bit value where 0x0000 corresponds to
  // 23°C, and each least significant bit (LSB) represents approximately
  // 0.00195°C.
  float tempC = 23.0 + ((float)rawTemp) * 0.00195;
  if (unit == "F") {
    float tempF = tempC * 9.0 / 5.0 + 32.0;  // Convert to Fahrenheit
    return tempF;
  }
  return tempC;
}

float convertRawGyro(int gRaw) {
  // convert the raw gyro data to degrees/second
  // we are using 250 degrees/seconds range
  // -250 maps to a raw value of -32768
  // +250 maps to a raw value of 32767
  return (gRaw * 250.0) / rawDataConversion;
}

float convertRawAccel(int raw, int offset) {
  // Assuming the full scale range is ±2g
  // Scale Factor for ±2g = 32768/2 = 16384
  return (raw - offset) / (rawDataConversion / BMI160.getAccelerometerRange());
}

void readSensors() {
  int gxRaw, gyRaw, gzRaw;
  int axRaw, ayRaw, azRaw;
  BMI160.readGyro(gxRaw, gyRaw, gzRaw);
  BMI160.readAccelerometer(axRaw, ayRaw, azRaw);

  gx = convertRawGyro(gxRaw);
  gy = convertRawGyro(gyRaw);
  gz = convertRawGyro(gzRaw);

  ax = convertRawAccel(axRaw, BMI160.getXAccelOffset());
  ay = convertRawAccel(ayRaw, BMI160.getYAccelOffset());
  az = convertRawAccel(azRaw, BMI160.getZAccelOffset());

  // Serial.println("g: " + String(gx) + ", " + String(gy) + ", " + String(gz));
  Serial.println("a: " + String(ax) + ", " + String(ay) + ", " + String(az));
}

bool isCollision(Pixel& obj1, Pixel& obj2) {
  // Check horizontal overlap
  if (obj1.x + 1 < obj2.x || obj1.x > obj2.x + 1) return false;
  // Check vertical overlap
  if (obj1.y + 1 < obj2.y || obj1.y > obj2.y + 1) return false;
  // Overlapping in both directions
  return true;
}

void motionAnimation(int maxX, int maxY) {
  if (motionObjects == nullptr) {
    generateMotionObjects(maxX, maxY);
  }
  if (!BMI160Initialized) return;
  readSensors();
  float scale = 0.001;
  float gravityMagnitude = 1.1;
  float gravityX = -ay * gravityMagnitude;
  float gravityY = -ax * gravityMagnitude;
  for (int i = 0; i < motionNumObjects; i++) {
    if (gravityEnabled) {
      motionObjects[i].vx = gravityX;
      motionObjects[i].vy = gravityY;
    } else {
      motionObjects[i].vx += -gx * scale;
      motionObjects[i].vy += gy * scale;
      motionObjects[i].vx *= energyLossFactor;
      motionObjects[i].vy *= energyLossFactor;
    }

    for (int j = i + 1; j < motionNumObjects; j++) {
      if (motionObjects[i].x == motionObjects[j].x &&
          motionObjects[i].y == motionObjects[j].y) {
        // Collision detected

        // Convert velocities to vectors
        Vector2D v1 = makeVector(motionObjects[i].vx, motionObjects[i].vy);
        Vector2D v2 = makeVector(motionObjects[j].vx, motionObjects[j].vy);

        // Calculate new velocities
        // Simple bounce logic: swap velocities
        motionObjects[i].vx = v2.x;
        motionObjects[i].vy = v2.y;
        motionObjects[j].vx = v1.x;
        motionObjects[j].vy = v1.y;
      }
    }

    // Update position based on velocity and constrain to screen
    motionObjects[i].x =
        constrain(motionObjects[i].x += motionObjects[i].vx, 0, maxX - 1);
    motionObjects[i].y =
        constrain(motionObjects[i].y += motionObjects[i].vy, 0, maxY - 1);

    // for (int j = i + 1; j < motionNumObjects; j++) {
    //   if (isCollision(motionObjects[i], motionObjects[j])) {
    //     Vector2D pos1 = makeVector(motionObjects[i].x, motionObjects[i].y);
    //     Vector2D pos2 = makeVector(motionObjects[j].x, motionObjects[j].y);
    //     Vector2D vel1 = makeVector(motionObjects[i].vx, motionObjects[i].vy);
    //     Vector2D vel2 = makeVector(motionObjects[j].vx, motionObjects[j].vy);

    //     Vector2D collisionNormal = normalize(subtract(pos2, pos1));
    //     float v1n = dotProduct(collisionNormal, vel1);
    //     float v2n = dotProduct(collisionNormal, vel2);

    //     // Swap the normal components of the velocities
    //     motionObjects[i].vx += (v2n - v1n) * collisionNormal.x;
    //     motionObjects[i].vy += (v2n - v1n) * collisionNormal.y;
    //     motionObjects[j].vx += (v1n - v2n) * collisionNormal.x;
    //     motionObjects[j].vy += (v1n - v2n) * collisionNormal.y;
    //   }
    // }
  }
}
#endif  // MOTION_H