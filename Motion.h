#ifndef MOTION_H
#define MOTION_H

#include <BMI160Gen.h>

#include "Colors.h"
#include "Vector2D.h"

struct Object {
  float x, y;
  float vx, vy;
  uint32_t color;
};

const int motion_i2c_addr = 0x69;
Object* motionObjects = nullptr;
int motionNumObjects = 9;

float pitch = 0, roll = 0, yaw = 0;
int gx = 0, gy = 0, gz = 0;

void generateMotionObjects(int maxX, int maxY) {
  motionObjects = new Object[motionNumObjects];
  for (int i = 0; i < motionNumObjects; i++) {
    motionObjects[i].color = colorPallet[random(0, palletSize)];
    motionObjects[i].x = random(1, maxX - 1);
    motionObjects[i].y = random(1, maxY - 1);
    motionObjects[i].vx = 0;
    motionObjects[i].vy = 0;
  }
}

void initializeMotion(int maxX, int maxY) {
  BMI160.begin(BMI160GenClass::I2C_MODE, motion_i2c_addr);
  generateMotionObjects(maxX, maxY);
}

float convertRawGyro(int gRaw) {
  // convert the raw gyro data to degrees/second
  // we are using 250 degrees/seconds range
  // -250 maps to a raw value of -32768
  // +250 maps to a raw value of 32767
  float g = (gRaw * 250.0) / 32768.0;
  return g;
}

float getTemperature(String unit) {
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

void readSensors() {
  int gxRaw, gyRaw, gzRaw;
  BMI160.readGyro(gxRaw, gyRaw, gzRaw);

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

bool isCollision(Object& obj1, Object& obj2) {
  float distance = sqrt(pow(obj1.x - obj2.x, 2) + pow(obj1.y - obj2.y, 2));
  return distance < 1.0;  // collisionThreshold is the minimum
                          // distance for collision
}

void motionAnimation(int maxX, int maxY) {
  readSensors();
  float scale = 0.001;
  float damping = 0.99;
  for (int i = 0; i < motionNumObjects; i++) {
    motionObjects[i].vx += -gx * scale;
    motionObjects[i].vy += gy * scale;

    motionObjects[i].vx *= damping;
    motionObjects[i].vy *= damping;

    motionObjects[i].x += motionObjects[i].vx;
    motionObjects[i].y += motionObjects[i].vy;

    if (motionObjects[i].x < 0) {
      motionObjects[i].x = 0;
      motionObjects[i].vx = -motionObjects[i].vx;
    }
    if (motionObjects[i].x >= maxX) {
      motionObjects[i].x = maxX - 1;
      motionObjects[i].vx = -motionObjects[i].vx;
    }
    if (motionObjects[i].y < 0) {
      motionObjects[i].y = 0;
      motionObjects[i].vy = -motionObjects[i].vy;
    }
    if (motionObjects[i].y >= maxY) {
      motionObjects[i].y = maxY - 1;
      motionObjects[i].vy = -motionObjects[i].vy;
    }

    for (int j = i + 1; j < motionNumObjects; j++) {
      if (isCollision(motionObjects[i], motionObjects[j])) {
        Vector2D pos1 = makeVector(motionObjects[i].x, motionObjects[i].y);
        Vector2D pos2 = makeVector(motionObjects[j].x, motionObjects[j].y);
        Vector2D vel1 = makeVector(motionObjects[i].vx, motionObjects[i].vy);
        Vector2D vel2 = makeVector(motionObjects[j].vx, motionObjects[j].vy);

        Vector2D collisionNormal = normalize(subtract(pos2, pos1));
        float v1n = dotProduct(collisionNormal, vel1);
        float v2n = dotProduct(collisionNormal, vel2);

        // Swap the normal components of the velocities
        motionObjects[i].vx += (v2n - v1n) * collisionNormal.x;
        motionObjects[i].vy += (v2n - v1n) * collisionNormal.y;
        motionObjects[j].vx += (v1n - v2n) * collisionNormal.x;
        motionObjects[j].vy += (v1n - v2n) * collisionNormal.y;
      }
    }
  }
}
#endif  // MOTION_H