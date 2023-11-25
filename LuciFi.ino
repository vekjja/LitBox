#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <ESPWiFi.h>
#include <arduinoFFT.h>
#include <math.h>

#include "Birds.h"
#include "Colors.h"
#include "GameofLife.h"
#include "SpectralAnalyzer.h"
#include "Text.h"

// LED Matrix Config
int LEDWidth = 32;
int LEDHeight = 8;
int ledDataPin = 12;  // Board Pin D6
uint8_t matrixType =
    NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG;
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
    LEDWidth, LEDHeight, ledDataPin, matrixType, NEO_GRB + NEO_KHZ800);

// Brightness and Color Config
const int maxBrightness = 255;
const int minBrightness = 1;
uint32_t pixelColor = WHITE;  // Default color
int brightness = 6;

// Visualization Config
const int maxFrameRate = 120;
unsigned int frameRate = 60;
String visualization = "bars";

// Web Server Config
ESPWiFi wifi = ESPWiFi("LuciFi", "abcd1234");

void setup() {
  matrix.begin();
  Serial.begin(115200);
  matrix.setTextWrap(false);
  matrix.setBrightness(brightness);
  testMatrix(&matrix, LEDWidth, LEDHeight);
  initializeWebServer();
}

void loop() {
  wifi.handleClient();

  if (visualization == "circles") {
    drawCircles();
  } else if (visualization == "birds") {
    runAtFrameRate(drawBirds, frameRate);
  } else if (visualization == "gameOfLife") {
    runAtFrameRate(drawGameOfLife, frameRate);
  } else {
    drawBars();
  }
}
void drawBars() {
  spectralAnalyzer(LEDWidth, LEDHeight);
  matrix.fillScreen(0);
  for (int x = 0; x < LEDWidth; x++) {
    for (int y = 0; y < spectralData[x]; y++) {
      uint32_t pixelColor = colorPallets[currentPalette][0];
      pixelColor = (y > 1) ? colorPallets[currentPalette][1] : pixelColor;
      pixelColor = (y > 3) ? colorPallets[currentPalette][2] : pixelColor;
      pixelColor = (y > 6) ? colorPallets[currentPalette][3] : pixelColor;
      matrix.drawPixel(x, LEDHeight - 1 - y, pixelColor);
    }
  }
  matrix.show();
}

void drawBirds() {
  if (birds == nullptr) {
    generateBirds(LEDWidth, LEDHeight);
  }
  updateFlock(LEDWidth, LEDHeight);
  matrix.fillScreen(0);
  for (int i = 0; i < birdCount; i++) {
    matrix.drawPixel(birds[i].pixel.x, birds[i].pixel.y, birds[i].color);
  }
  matrix.show();
}

void drawCircles() {
  spectralAnalyzer(LEDWidth, LEDHeight);
  matrix.fillScreen(0);
  for (int x = 0; x < LEDWidth; x++) {
    int circleRadius = spectralData[x];
    int circleColor = colorPallets[currentPalette][0];
    circleColor =
        (circleRadius > 2) ? colorPallets[currentPalette][1] : circleColor;
    circleColor =
        (circleRadius > 3) ? colorPallets[currentPalette][2] : circleColor;
    circleColor =
        (circleRadius > 5) ? colorPallets[currentPalette][3] : circleColor;
    if (circleRadius > 0) {
      matrix.drawCircle(x, 4, circleRadius, circleColor);
    }
  }
  matrix.show();
}

void drawGameOfLife() {
  if (gol_Cells == nullptr) {
    startGameOfLife(LEDWidth, LEDHeight);
  }
  int cellColor = colorPallets[currentPalette][0];
  updateGameOfLife(LEDWidth, LEDHeight, 231);
  matrix.fillScreen(0);
  for (int x = 0; x < LEDWidth; x++) {
    for (int y = 0; y < LEDHeight; y++) {
      if (gol_Cells[x][y] == 1) {
        matrix.drawPixel(x, y, cellColor);
      }
    }
  }
  matrix.show();
}

void setBrightness(int newBrightness) {
  brightness = constrain(newBrightness, minBrightness, maxBrightness);
  matrix.setBrightness(brightness);
  matrix.show();
}

void setSensitivity(int newSensitivity) {
  constrain(newSensitivity, minSensitivity, maxSensitivity);
  sensitivity = newSensitivity;
}

void setFramerate(unsigned int fps) {
  frameRate = constrain(fps, 1, maxFrameRate);
}

void initializeWebServer() {
  wifi.webServer.on("/sensitivity", HTTP_GET, []() {
    wifi.webServer.send(200, "text/plain", String(sensitivity));
  });
  wifi.webServer.on("/sensitivity", HTTP_POST, []() {
    if (wifi.webServer.hasArg("sensitivity")) {
      int newSensitivity = wifi.webServer.arg("sensitivity").toInt();
      setSensitivity(newSensitivity);
      wifi.webServer.send(200, "text/plain", "Sensitivity updated");
    } else {
      wifi.webServer.send(400, "text/plain", "Missing Sensitivity value");
    }
  });

  wifi.webServer.on("/brightness", HTTP_GET, []() {
    wifi.webServer.send(200, "text/plain", String(brightness));
  });
  wifi.webServer.on("/brightness", HTTP_POST, []() {
    if (wifi.webServer.hasArg("brightness")) {
      int newBrightness = wifi.webServer.arg("brightness").toInt();
      setBrightness(newBrightness);
      wifi.webServer.send(200, "text/plain", "Brightness updated");
    } else {
      wifi.webServer.send(400, "text/plain", "Missing Brightness value");
    }
  });

  wifi.webServer.on("/frameRate", HTTP_GET, []() {
    wifi.webServer.send(200, "text/plain", String(frameRate));
  });
  wifi.webServer.on("/frameRate", HTTP_POST, []() {
    if (wifi.webServer.hasArg("frameRate")) {
      unsigned int newFrameRate = wifi.webServer.arg("frameRate").toInt();
      setFramerate(newFrameRate);  // Update the global framerate
      wifi.webServer.send(200, "text/plain", "Frame Rate updated");
    } else {
      wifi.webServer.send(400, "text/plain", "Missing Frame Rate value");
    }
  });

  wifi.webServer.on("/visualization", HTTP_POST, []() {
    wifi.webServer.send(200, "text/plain", String(visualization));
  });
  wifi.webServer.on("/visualization", HTTP_POST, []() {
    if (wifi.webServer.hasArg("visualization")) {
      // setVisualization(newMode);
      visualization = wifi.webServer.arg("visualization");
      wifi.webServer.send(200, "text/plain",
                          "Visualization set to: " + String(visualization));
    } else {
      wifi.webServer.send(400, "text/plain", "Missing visualization mode");
    }
  });

  wifi.webServer.on("/text", HTTP_POST, []() {
    if (wifi.webServer.hasArg("textColor")) {
      String color = wifi.webServer.arg("textColor");
      pixelColor = hexToColor(color);
    }
    if (wifi.webServer.hasArg("textSpeed")) {
      textSpeed = wifi.webServer.arg("textSpeed").toInt();
      wifi.webServer.send(200, "text/plain", "Speed updated");
    }
    if (wifi.webServer.hasArg("text")) {
      text = wifi.webServer.arg("text");
    }
    if (wifi.webServer.hasArg("textAnimation")) {
      String textAnimation = wifi.webServer.arg("textAnimation");
      if (textAnimation == "scroll") {
        scrollText(&matrix, text);
      } else if (textAnimation == "wave") {
        waveText(&matrix, text);
      } else if (textAnimation == "blinking") {
        blinkText(&matrix, text);
      }
    } else {
      scrollText(&matrix, text);
    }
    wifi.webServer.send(200, "text/plain", "Text updated");
  });

  wifi.webServer.on("/birds", HTTP_GET, []() {
    String response = "";
    response += "birdCount=" + String(birdCount) + "\n";
    response += "birdAlignment=" + String(birdAlignment) + "\n";
    response += "birdCohesion=" + String(birdCohesion) + "\n";
    response += "birdSeparation=" + String(birdSeparation) + "\n";
    response += "birdVerticalBounds=" + String(birdVerticalBounds) + "\n";
    response += "birdHorizontalBounds=" + String(birdHorizontalBounds) + "\n";
    wifi.webServer.send(200, "text/plain", response);
  });

  wifi.webServer.on("/birds", HTTP_POST, []() {
    if (wifi.webServer.hasArg("max_velocity")) {
      birdMaxVelocity = wifi.webServer.arg("max_velocity").toFloat();
    }
    if (wifi.webServer.hasArg("min_velocity")) {
      birdMinVelocity = wifi.webServer.arg("min_velocity").toFloat();
    }
    if (wifi.webServer.hasArg("birdCount")) {
      birdCount = wifi.webServer.arg("birdCount").toInt();
    }
    if (wifi.webServer.hasArg("birdAlignment")) {
      birdAlignment = wifi.webServer.arg("birdAlignment").toFloat();
    }
    if (wifi.webServer.hasArg("birdCohesion")) {
      birdCohesion = wifi.webServer.arg("birdCohesion").toFloat();
    }
    if (wifi.webServer.hasArg("birdSeparation")) {
      birdSeparation = wifi.webServer.arg("birdSeparation").toFloat();
    }
    if (wifi.webServer.hasArg("birdVerticalBounds")) {
      birdVerticalBounds =
          wifi.webServer.arg("birdVerticalBounds").compareTo("true") == 0;
    }
    if (wifi.webServer.hasArg("birdHorizontalBounds")) {
      birdHorizontalBounds =
          wifi.webServer.arg("birdHorizontalBounds").compareTo("true") == 0;
    }
    generateBirds(LEDWidth, LEDHeight);
    wifi.webServer.send(200, "text/plain", "Bird settings updated");
  });

  wifi.setConnectSubroutine([]() { testMatrix(&matrix, LEDWidth, LEDHeight); });
  wifi.enableMDNS("lucifi");
  wifi.Start();
}
