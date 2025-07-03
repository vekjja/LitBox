#include <AsyncJson.h>
#include <ESPWiFi.h>
#include <LEDMatrix.h>
#include <SpectralAnalyzer.h>

#include "Birds.h"
#include "Colors.h"
#include "GameOfLife.h"
#include "MatrixAnimation.h"
#include "Stars.h"

// Web Server
ESPWiFi device;

// Visualization Config
const int maxFrameRate = 120;
unsigned int frameRate = 60;
String visualization = "bars";

// Helper: Convert a config value (int or string) to a hex color string
inline String configValueToHexString(const JsonVariant &val) {
  if (val.is<int>() || val.is<unsigned int>() || val.is<long>()) {
    char buf[8];
    snprintf(buf, sizeof(buf), "%06x", val.as<unsigned int>());
    return String("#") + buf;
  } else if (val.is<const char *>() || val.is<String>()) {
    String s = val.as<String>();
    if (!s.startsWith("#") && s.length() == 6)
      return String("#") + s;
    return s;
  }
  return "#000000";
}

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

void applyConfig() {
  setBrightness(device.config["brightness"].as<int>());
  setSensitivity(device.config["sensitivity"].as<int>());
  setFramerate(device.config["frameRate"].as<unsigned int>());
  visualization = device.config["visualization"].as<String>();

  if (!device.config["pixelColor"].isNull()) {
    pixelColor = hexToCRGB(configValueToHexString(device.config["pixelColor"]));
  }
  if (!device.config["pixelBgColor"].isNull()) {
    pixelBgColor =
        hexToCRGB(configValueToHexString(device.config["pixelBgColor"]));
  }
  if (!device.config["colorPallet"].isNull()) {
    JsonArray arr = device.config["colorPallet"].as<JsonArray>();
    for (int i = 0; i < palletSize && i < arr.size(); i++) {
      colorPallet[i] = hexToCRGB(configValueToHexString(arr[i]));
    }
  }
}

// Simplified drawBars function to directly use colorPallet
void drawBars() {
  // Analyze audio and populate spectralData
  spectralAnalyzer(LED_WIDTH, LED_HEIGHT);
  FastLED.clear();

  for (int x = 0; x < LED_WIDTH; x++) {
    int spectralValue = spectralData[x];
    for (int y = 0; y < spectralValue; y++) {
      // Use colorPallet directly based on y value
      CRGB color = colorPallet[min(y / 2, 3)];
      drawPixel(x, y, color);
    }
  }

  FastLED.show();
}

void drawCircles() {
  spectralAnalyzer(LED_WIDTH, LED_HEIGHT);
  FastLED.clear();
  int maxRadius = LED_HEIGHT / 2;
  for (int x = 0; x < LED_WIDTH; x++) {
    int circleRadius = spectralData[x] / 2;
    if (circleRadius > 0) {
      // Map radius proportionally to color index (0-3)
      int colorIdx = map(circleRadius, 1, maxRadius, 0, 3);
      colorIdx = constrain(colorIdx, 0, 3);
      CRGB circleColor = colorPallet[colorIdx];
      drawCircle(x, 3, circleRadius, circleColor);
    }
  }
  FastLED.show();
}

void drawWaveform() {
  spectralAnalyzer(LED_WIDTH, LED_HEIGHT);
  FastLED.clear();

  int middleY = LED_HEIGHT / 2; // Calculate the middle row of the matrix

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
    // Invert Y coordinate so birds appear right-side up
    int displayY = LED_HEIGHT - 1 - birds[i].y;
    drawPixel(birds[i].x, displayY, birds[i].color);
  }
  FastLED.show();
}

void drawGameOfLife() {
  updateGameOfLife(LED_WIDTH, LED_HEIGHT, 231);

  fillMatrix(pixelBgColor);
  for (int x = 0; x < LED_WIDTH; x++) {
    for (int y = 0; y < LED_HEIGHT; y++) {
      if (gol_Cells[x][y] == 1) {
        drawPixel(x, y, pixelColor);
      }
    }
  }
  FastLED.show();
}

void drawStarPulse() {
  spectralAnalyzer(LED_WIDTH, LED_HEIGHT);
  updateStartPulse(LED_WIDTH, LED_HEIGHT);
  FastLED.clear();
  for (int i = 0; i < starCount; i++) {
    drawPixel(stars[i].x, stars[i].y, stars[i].color);
  }
  FastLED.show();
}

void startWebServer() {
  device.srvLog();
  device.srvRoot();
  device.srvFiles();
  device.srvConfig();
  device.srvRestart();

  device.connectSubroutine = []() { testMatrix(); };

  applyConfig();
  device.startWiFi();
  device.startMDNS();
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
  device.readConfig();

  device.startLEDMatrix();
  testMatrix();
  startWebServer();
  device.startSpectralAnalyzer();
  device.log("🔥 Lit Box Initialized");
}

void loop() {
  yield();
  applyConfig();
  if (visualization == "circles") {
    drawCircles();
  } else if (visualization == "birds") {
    runAtFrameRate(drawBirds, frameRate);
  } else if (visualization == "gameOfLife") {
    runAtFrameRate(drawGameOfLife, frameRate);
  } else if (visualization == "waveform") {
    runAtFrameRate(drawWaveform, frameRate);
  } else if (visualization == "matrix") {
    runAtFrameRate([]() { matrixAnimation(LED_WIDTH, LED_HEIGHT); }, frameRate);
  } else if (visualization == "starPulse") {
    drawStarPulse();
  } else {
    drawBars();
  }
}