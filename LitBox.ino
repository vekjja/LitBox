#include <ESPWiFi.h>

#include "Matrix.h"
#include "SpectralAnalyzer.h"

// Web Server Config
ESPWiFi wifi;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  initializeMatrix();
  initializeWebServer();
  Serial.println("Lit Box Initialized");
}

void loop() {
  wifi.handleClient();
  drawBars();
}

void drawBars() { spectralAnalyzer(LED_WIDTH, LED_HEIGHT); }

void initializeWebServer() {
  wifi.connectSubroutine = []() { testMatrix(); };
  wifi.start();
}
