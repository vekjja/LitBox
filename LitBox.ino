#include <ESPWiFi.h>

#include "GameOfLife.h"
#include "LEDMatrix.h"
#include "SpectralAnalyzer.h"
#include "Utils.h"
#include "colors.h"

// Web Server Config
ESPWiFi wifi;

void setup() {
  initializeMatrix();
  initializeWebServer();
  initializeSpectralAnalyzer();
  randomSeed(analogRead(A0));
  Serial.println("Lit Box Initialized");
}

void loop() { runAtFrameRate(drawBars, 60); }

void drawBars() {
  spectralAnalyzer(LED_WIDTH, LED_HEIGHT);
  FastLED.clear();
  // Check for new spectral data
  // for (int x = 0; x < LED_WIDTH; x++) {
  //   for (int y = 0; y < spectralData[x]; y++) {
  //     RgbColor pixelColor = colorPallet[0];
  //     pixelColor = (y > 1) ? colorPallet[1] : pixelColor;
  //     pixelColor = (y > 3) ? colorPallet[2] : pixelColor;
  //     pixelColor = (y > 6) ? colorPallet[3] : pixelColor;
  //     drawPixel(x, y, pixelColor);
  //   }
  // }
  drawPixel(16, 4, RED);
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
