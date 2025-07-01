#include <ESPWiFi.h>

#include "Birds.h"
#include "Colors.h"
#include "GameOfLife.h"
#include "LEDMatrix.h"
#include "Matrix.h"
#include "SpectralAnalyzer.h"

// Web Server
ESPWiFi device;

// Visualization Config
const int maxFrameRate = 120;
unsigned int frameRate = 60;
String visualization = "bars";

void setBrightness(int newBrightness) {
  device.config["brightness"] =
      constrain(newBrightness, minBrightness, maxBrightness);
  FastLED.setBrightness(device.config["brightness"]);
}

void setSensitivity(int newSensitivity) {
  device.config["sensitivity"] =
      constrain(newSensitivity, minSensitivity, maxSensitivity);
  sensitivity = device.config["sensitivity"];
}

void setFramerate(unsigned int fps) {
  device.config["frameRate"] = constrain(fps, 1, maxFrameRate);
  frameRate = device.config["frameRate"];
}

void initializeFromConfig() {
  setBrightness(device.config["brightness"].as<int>());
  setSensitivity(device.config["sensitivity"].as<int>());
  setFramerate(device.config["frameRate"].as<unsigned int>());

  visualization = device.config["visualization"].as<String>();
  // temperatureUnit = device.config["temperatureUnit"].as<String>();

  // JsonArray colorArray = device.config["colorPallet"];
  // for (int i = 0; i < palletSize; i++) {
  //   colorPallet[i] = colorArray[i];
  // }
  // pixelColor = device.config["pixelColor"];
  // pixelBgColor = device.config["pixelBgColor"];

  // text = device.config["text"]["content"].as<String>();
  // textSpeed = device.config["text"]["speed"].as<int>();
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

void drawWaveform() {
  spectralAnalyzer(LED_WIDTH, LED_HEIGHT);
  FastLED.clear();

  int middleY = LED_HEIGHT / 2;  // Calculate the middle row of the matrix

  for (int x = 0; x < LED_WIDTH; x++) {
    int value = spectralData[x] / 2;
    for (int y = 0; y < value; y++) {
      // Draw upwards from the middle
      CRGB pixelColor = colorPallet[0];
      if (y > 1) {
        pixelColor = colorPallet[1];
      }
      if (y > 2) {
        pixelColor = colorPallet[2];
      }
      if (y > 3) {
        pixelColor = colorPallet[3];
      }
      drawPixel(x, middleY - y, pixelColor);
      // Draw downwards from the middle
      if (y > 0 && middleY + y < LED_HEIGHT) {
        drawPixel(x, middleY + y, pixelColor);
      }
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
  device.initWebServer();
  // device.addESPWiFiEndpoints();
  device.webServer->on(
      "/config", HTTP_POST, [](AsyncWebServerRequest* request) {
        if (request->hasParam("plain", true)) {
          String body = request->getParam("plain", true)->value();
          JsonDocument config;
          DeserializationError error = deserializeJson(config, body);
          if (error) {
            request->send(400, "text/plain", "Invalid JSON");
            return;
          } else {
            if (config["brightness"].is<int>()) {
              setBrightness(config["brightness"]);
            }
            if (config["sensitivity"].is<int>()) {
              setSensitivity(config["sensitivity"]);
            }
            if (config["frameRate"].is<unsigned int>()) {
              setFramerate(config["frameRate"]);
            }
            if (config["visualization"].is<String>()) {
              visualization = config["visualization"].as<String>();
            }

            request->send(200, "application/json", config.as<String>());
            device.config.set(config);
          }
        } else {
          request->send(400, "text/plain", "Missing body");
        }
      });

  device.webServer->on(
      "/saveConfig", HTTP_GET, [](AsyncWebServerRequest* request) {
        device.saveConfig();
        request->send(200, "application/json", device.config.as<String>());
      });

  // device.connectSubroutine = []() { testMatrix(); };
  device.startWebServer();
}

void runAtFrameRate(void (*callback)(), unsigned int frameRate) {
  static unsigned long lastTime = 0;
  unsigned long currentTime = millis();
  if (currentTime - lastTime >= 1000 / frameRate) {
    callback();
    lastTime = currentTime;
  }
}

void setup() {
  device.startLog();
  device.startWiFi();
  device.startMDNS();
  initializeMatrix();
  testMatrix();
  initializeWebServer();
  initializeFromConfig();
  initializeSpectralAnalyzer();
  Serial.println("🔥 Lit Box Initialized");
}

void loop() {
  yield();
  drawBars();
  // if (visualization == "circles") {
  //   drawCircles();
  // } else if (visualization == "birds") {
  //   runAtFrameRate(drawBirds, frameRate);
  // } else if (visualization == "gameOfLife") {
  //   runAtFrameRate(drawGameOfLife, frameRate);
  // } else if (visualization == "waveform") {
  //   runAtFrameRate(drawWaveform, frameRate);
  // } else if (visualization == "matrix") {
  //   runAtFrameRate([]() { matrixAnimation(LED_WIDTH, LED_HEIGHT); },
  //   frameRate);
  // } else {
  //   drawBars();
  // }
}