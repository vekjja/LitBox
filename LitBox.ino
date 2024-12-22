#include <ESPWiFi.h>

#include "Matrix.h"
#include "SpectralAnalyzer.h"

// Web Server Config
ESPWiFi wifi;

void setup() {
  Serial.begin(115200);
  initializeMatrix();
  initializeWebServer();
  Serial.println("Lit Box Initialized");
}

void loop() {
  wifi.handleClient();
  Serial.println("Drawing bars");
  drawBars();
}

void initializeWebServer() {
  wifi.connectSubroutine = []() { testMatrix(); };
  wifi.start();
}

void drawBars() {
  // spectralAnalyzer(LED_WIDTH, LED_HEIGHT);
  // for (int x = 0; x < LED_WIDTH; x++) {
  //   Serial.println(spectralData[x]);
  //   for (int y = 0; y < spectralData[x]; y++) {
  //     // uint32_t pixelColor = colorPallet[0];
  //     // pixelColor = (y > 1) ? colorPallet[1] : pixelColor;
  //     // pixelColor = (y > 3) ? colorPallet[2] : pixelColor;
  //     // pixelColor = (y > 6) ? colorPallet[3] : pixelColor;
  //     leds[XY(x, LED_HEIGHT)] = CRGB::White;
  //   }
  // }
}
