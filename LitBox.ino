#include <ArduinoJson.h>
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

void setBrightness(int newBrightness) {
  wifi.config["brightness"] =
      constrain(newBrightness, minBrightness, maxBrightness);
  FastLED.setBrightness(wifi.config["brightness"]);
  FastLED.show();
}

void setSensitivity(int newSensitivity) {
  wifi.config["sensitivity"] =
      constrain(newSensitivity, minSensitivity, maxSensitivity);
  sensitivity = wifi.config["sensitivity"];
}

void setFramerate(unsigned int fps) {
  wifi.config["frameRate"] = constrain(fps, 1, maxFrameRate);
  frameRate = wifi.config["frameRate"];
}

void loop() {
  if (visualization == "gameOfLife") {
    runAtFrameRate(drawGameOfLife, frameRate);
  } else {
    // runAtFrameRate(drawBars, frameRate);
    drawBars();
  }
}

void drawBars() {
  // Analyze audio and populate spectralData
  spectralAnalyzer(LED_WIDTH, LED_HEIGHT);
  FastLED.clear();
  for (int x = 0; x < LED_WIDTH; x++) {
    for (int y = 0; y < spectralData[x]; y++) {
      // Map y to a color palette
      CRGB color = CHSV(map(y, 0, LED_HEIGHT, 0, 255), 255, 255);
      leds[XY(x, y)] = color;
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