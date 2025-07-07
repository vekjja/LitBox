#ifndef MOTION_H
#define MOTION_H

#include <2D/Body.h>
#include <2D/World.h>
#include <DFRobot_BMI160.h>
#include <Wire.h>

#include "Colors.h"

const int motion_i2c_addr = 0x68;

struct MotionObject {
  Body *body;
  CRGB color;
};

// Motion Animation
int motionNumObjects = 9;
MotionObject *motionObjects = nullptr;

// BMI160 sensor
float gx = 0.0, gy = 0.0, gz = 0.0;
float ax = 0.0, ay = 0.0, az = 0.0;
bool BMI160Initialized = false;

// Physics
bool gravityEnabled = true;
World world(Vec2{0.0, 0.0}, 6);

// BMI160 instance - using DFRobot library
DFRobot_BMI160 bmi160;

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

bool checkI2CDevice(uint8_t address) {
  Wire.beginTransmission(address);
  if (Wire.endTransmission() == 0) {
    return true; // Device found at this address
  }
  return false; // No device found
}

void scanI2CDevices() {
  Wire.begin(6, 7); // Initialize the I2C bus with SDA=GPIO6, SCL=GPIO7
  byte error, address;
  int nDevices;

  Serial.println("Scanning for I2C devices...");

  nDevices = 0;
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");

      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found");
  else
    Serial.println("done\n");
}

void startMotionSensor(int maxX, int maxY) {
  scanI2CDevices();
  if (checkI2CDevice(motion_i2c_addr)) {
    // Initialize BMI160 using DFRobot library
    int8_t rslt = bmi160.I2cInit(motion_i2c_addr);

    if (rslt == BMI160_OK) {
      generateMotionObjects(maxX, maxY);
      Serial.println("BMI160 initialized successfully");
      BMI160Initialized = true;
    } else {
      BMI160Initialized = false;
      Serial.println("BMI160 initialization failed!");
    }
  } else {
    BMI160Initialized = false;
    Serial.println(
        "BMI160 sensor not detected at the specified I2C address: 0x" +
        String(motion_i2c_addr, HEX));
  }
}

// float getTemperature(String unit) {
//   if (!BMI160Initialized)
//     return 0;
//   int16_t rawTemp = BMI160.getTemperature(); // returns a 16-bit integer
//   // The temperature data is a signed 16-bit value where 0x0000 corresponds
//   // 23°C, and each least significant bit (LSB) represents approximately
//   // 0.00195°C.
//   float tempC = 23.0 + ((float)rawTemp) * 0.00195;
//   if (unit == "F") {
//     float tempF = tempC * 9.0 / 5.0 + 32.0; // Convert to Fahrenheit
//     return tempF;
//   }
//   return tempC;
// }

void readGyro() {
  if (!BMI160Initialized)
    return;

  int16_t gyroData[3];
  int8_t rslt = bmi160.getGyroData(gyroData);

  if (rslt == BMI160_OK) {
    // Convert raw values to degrees/second (default range is ±250°/s)
    gx = gyroData[0];
    gy = gyroData[1];
    gz = gyroData[2];
  }

  // Serial.println("g: " + String(gx) + ", " + String(gy) + ", " + String(gz));
}

void readAccelerometer() {
  if (!BMI160Initialized)
    return;

  int16_t accelData[3];
  int8_t rslt = bmi160.getAccelData(accelData);

  if (rslt == BMI160_OK) {
    // Convert raw values to g (default range is ±2g)
    ax = accelData[0];
    ay = accelData[1];
    az = accelData[2];
  }

  // Serial.println("a: " + String(ax) + ", " + String(ay) + ", " + String(az));
}

void motionStep(int maxX, int maxY) {
  if (motionObjects == nullptr) {
    generateMotionObjects(maxX, maxY);
  }

  readAccelerometer();

  Serial.println("g: " + String(-ay) + ", " + String(-ax));
  world.gravity.Set(-ay, -ax);
  world.Step(0.8f);

  for (int i = 0; i < motionNumObjects; i++) {
    Body *b = motionObjects[i].body;
    b->position.x = constrain(b->position.x, 0, maxX - 1);
    b->position.y = constrain(b->position.y, 0, maxY - 1);
    b->rotation = 0.0f;
  }
}

#endif // MOTION_H