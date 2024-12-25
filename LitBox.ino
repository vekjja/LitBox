#include <ESPWiFi.h>

#include "Birds.h"
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
  initializeFromConfig();
  initializeSpectralAnalyzer();
  // randomSeed(analogRead(A0));
  Serial.println("Lit Box Initialized");
}

void setBrightness(int newBrightness) {
  wifi.config["brightness"] =
      constrain(newBrightness, minBrightness, maxBrightness);
  FastLED.setBrightness(wifi.config["brightness"]);
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

void initializeFromConfig() {
  setBrightness(wifi.config["brightness"].as<int>());
  setSensitivity(wifi.config["sensitivity"].as<int>());
  setFramerate(wifi.config["frameRate"].as<unsigned int>());

  visualization = wifi.config["visualization"].as<String>();
  // temperatureUnit = wifi.config["temperatureUnit"].as<String>();

  // JsonArray colorArray = wifi.config["colorPallet"];
  // for (int i = 0; i < palletSize; i++) {
  //   colorPallet[i] = colorArray[i];
  // }
  // pixelColor = wifi.config["pixelColor"];
  // pixelBgColor = wifi.config["pixelBgColor"];

  // text = wifi.config["text"]["content"].as<String>();
  // textSpeed = wifi.config["text"]["speed"].as<int>();
}

void loop() {
  if (visualization == "gameOfLife") {
    runAtFrameRate(drawGameOfLife, frameRate);
  } else if (visualization == "birds") {
    runAtFrameRate(drawBirds, frameRate);
  } else if (visualization == "circles") {
    // runAtFrameRate(drawCircles, frameRate);
    drawCircles();
  } else {
    drawBars();
  }
}

void drawBars() {
  // Analyze audio and populate spectralData
  spectralAnalyzer(LED_WIDTH, LED_HEIGHT);
  FastLED.clear();
  for (int x = 0; x < LED_WIDTH; x++) {
    for (int y = 0; y < spectralData[x]; y++) {
      CRGB color = colorPallet[0];
      if (y > 1) color = colorPallet[1];
      if (y > 4) color = colorPallet[2];
      if (y > 6) color = colorPallet[3];
      drawPixel(x, y, color);
    }
  }
  FastLED.show();
}

void drawBirds() {
  updateFlock(LED_WIDTH, LED_HEIGHT);
  FastLED.clear();
  for (int i = 0; i < birdCount; i++) {
    drawPixel(birds[i].x, birds[i].y, birds[i].color);
  }
  FastLED.show();
}

void drawCircles() {
  spectralAnalyzer(LED_WIDTH, LED_HEIGHT);
  FastLED.clear();
  for (int x = 0; x < LED_WIDTH; x++) {
    int circleRadius = spectralData[x] / 2;
    CRGB circleColor = colorPallet[0];
    if (circleRadius > 1) circleColor = colorPallet[1];
    if (circleRadius > 4) circleColor = colorPallet[2];
    if (circleRadius > 6) circleColor = colorPallet[3];
    if (circleRadius > 0) {
      drawCircle(x, 4, circleRadius, circleColor);
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
  wifi.webServer.on("/config", HTTP_GET, []() {
    wifi.webServer.send(200, "application/json", wifi.config.as<String>());
  });

  wifi.webServer.on("/config", HTTP_POST, []() {
    String body = wifi.webServer.arg("plain");
    JsonDocument config;
    DeserializationError error = deserializeJson(config, body);
    if (error) {
      wifi.webServer.send(400, "text/plain", "Invalid JSON");
      return;
    } else {
      if (config.containsKey("brightness")) {
        setBrightness(config["brightness"]);
      }
      if (config.containsKey("sensitivity")) {
        setSensitivity(config["sensitivity"]);
      }
      if (config.containsKey("frameRate")) {
        setFramerate(config["frameRate"]);
      }
      if (config.containsKey("visualization")) {
        visualization = config["visualization"].as<String>();
      }

      birds = nullptr;
      wifi.webServer.send(200, "application/json", config.as<String>());
      wifi.config.set(config);
    }
  });

  wifi.webServer.on("/saveConfig", HTTP_GET, []() {
    wifi.saveConfig();
    wifi.webServer.send(200, "application/json", wifi.config.as<String>());
  });

  wifi.connectSubroutine = []() { testMatrix(); };
  wifi.start();
}