#ifndef MOTION_H
#define MOTION_H

#include <BMI160Gen.h>

const int select_pin = 10;
const int i2c_addr = 0x69;
int gx, gy, gz;

void initializeMotion() { BMI160.begin(BMI160GenClass::I2C_MODE, i2c_addr); }

void updateMotion() {
  BMI160.readGyro(gx, gy, gz);
  Serial.print("Gyro: ");
  Serial.print("g:\t");
  Serial.print(gx);
  Serial.print("\t");
  Serial.print(gy);
  Serial.print("\t");
  Serial.print(gz);
  Serial.println();
}

#endif  // MOTION_H