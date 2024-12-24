#include <ESPWiFi.h>

#include "GameOfLife.h"
#include "LEDMatrix.h"
#include "SpectralAnalyzer.h"
#include "Utils.h"
#include "colors.h"

// Web Server Config
ESPWiFi wifi;

// Visualization Config
const int maxFrameRate = 120;
unsigned int frameRate = 60;
String visualization = "bars";

void setup() {
  initializeMatrix();
  initializeWebServer();
  initializeSpectralAnalyzer();
  // randomSeed(analogRead(A0));
  Serial.println("Lit Box Initialized");
}

void loop() {
  if (visualization == "gameOfLife") {
    runAtFrameRate(drawGameOfLife, frameRate);
  } else {
    runAtFrameRate(drawBars, frameRate);
  }
}

void drawBars() {
  spectralAnalyzer(LED_WIDTH, LED_HEIGHT);
  FastLED.clear();
  for (int x = 0; x < LED_WIDTH; x++) {
    Serial.println("X: " + String(x) + " Y: " + String(spectralData[x]));
    for (int y = 0; y < spectralData[x]; y++) {
      // Map y to colorPallet index
      CRGB pixelColor = colorPallet[min(y / 2, 3)];
      drawPixel(x, y, pixelColor);
    }
  }
  FastLED.show();
}

void drawGameOfLife() {
  updateGameOfLife(LED_WIDTH, LED_HEIGHT, 231);
  FastLED.clear();
  for (int x = 0; x < LED_WIDTH; x++) {
    for (int y = 0; y < LED_HEIGHT; y++) {
      if (gol_Cells[x][y] == 1) {
        drawPixel(x, y, pixelColor);
      }
    }
  }
  FastLED.show();
}

void initializeWebServer() {
  wifi.connectSubroutine = []() { testMatrix(); };
  wifi.start();
}
