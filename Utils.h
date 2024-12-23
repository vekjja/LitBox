#ifndef UTILS_H
#define UTILS_H

void runAtFrameRate(void (*callback)(), unsigned int frameRate) {
  static unsigned long lastTime = 0;
  unsigned long currentTime = millis();
  if (currentTime - lastTime >= 1000 / frameRate) {
    callback();
    lastTime = currentTime;
  }
}

// bool checkI2CDevice(uint8_t address) {
//   Wire.begin();  // Initialize I2C communication
//   Wire.beginTransmission(address);
//   if (Wire.endTransmission() == 0) {
//     return true;  // Device found at this address
//   }
//   return false;  // No device found
// }

// void scanI2CDevices() {
//   Wire.begin();  // Initialize the I2C bus
//   byte error, address;
//   int nDevices;

//   Serial.println("Scanning for I2C devices...");

//   nDevices = 0;
//   for (address = 1; address < 127; address++) {
//     Wire.beginTransmission(address);
//     error = Wire.endTransmission();

//     if (error == 0) {
//       Serial.print("I2C device found at address 0x");
//       if (address < 16) Serial.print("0");
//       Serial.print(address, HEX);
//       Serial.println(" !");

//       nDevices++;
//     } else if (error == 4) {
//       Serial.print("Unknown error at address 0x");
//       if (address < 16) Serial.print("0");
//       Serial.println(address, HEX);
//     }
//   }
//   if (nDevices == 0)
//     Serial.println("No I2C devices found");
//   else
//     Serial.println("done\n");
// }
#endif