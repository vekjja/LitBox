#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <ESPWiFi.h>
#include <IOPin.h>
#include <arduinoFFT.h>

#include "Birds.h"
#include "colors.h"

// IOPins
IOPin ledData(12);
IOPin audio(A0, INPUT);

// LED Matrix Config
int ledRows = 8;
int ledColumns = 32;
uint8_t matrixType =
    NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG;
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
    ledColumns, ledRows, ledData.pin(), matrixType, NEO_GRB + NEO_KHZ800);

// Brightness and Color Config
const int minBrightness = 1;
const int maxBrightness = 255;
const int colorPalletCount = 7;
int brightness = 6;
int currentPalette = 1;
uint32_t colorPallets[colorPalletCount][4] = {
    {GREEN, YELLOW, ORANGE, RED},       {BLUE, CYAN, VIOLET, WHITE},
    {MAGENTA, MAGENTA, VIOLET, VIOLET}, {CORAL, SALMON, SALMON, ROSE},
    {ROSE, ROSE, ROSE, ROSE},           {GREEN, GREEN, GREEN, GREEN},
    {WHITE, WHITE, WHITE, WHITE},
};

// Audio Config
arduinoFFT FFT = arduinoFFT();
const int maxInput = 81;
const int minSensitivity = 1;
const int maxSensitivity = 100;
const uint16_t audioSamples = 128;
const int usableSamples = (audioSamples / 2);
bool scaling = true;
double vReal[audioSamples];
double vImage[audioSamples];
int sensitivity = 6;

// Visualization Config
int visualization = 0;
int maxVisualization = 2;

// text color and speed
uint32_t textColor = WHITE;  // Default color
int textSpeed = 50;          // Default speed

// Web Server Config
ESPWiFi wifi = ESPWiFi("SpectralAnalyzer", "12345678");

void setup() {
  matrix.begin();
  Serial.begin(115200);
  matrix.setTextWrap(false);
  ledData.setPinMode(OUTPUT);
  matrix.setBrightness(brightness);
  // testMatrix(matrix, ledColumns, ledRows);
  testMatrix();
  initializeWebServer();
}

void loop() {
  wifi.handleClient();
  spectralAnalyzer();
}

void spectralAnalyzer() {
  for (int i = 0; i < audioSamples; i++) {
    vReal[i] = audio.readA() / sensitivity;
    vImage[i] = 0;
  }

  FFT.Windowing(vReal, audioSamples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImage, audioSamples, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImage, audioSamples);

  int spectralData[ledColumns] = {0};
  peakDetection(spectralData);

  switch (visualization) {
    case 1:
      drawCircles(spectralData);
      break;
    case 2:
      // drawBirds();
      runAtFrameRate(drawBirds, 60);
      break;
    default:
      drawBars(spectralData);
      break;
  }
}

void peakDetection(int* peakData) {
  int avgRange = usableSamples / ledColumns;

  // Start from 1 instead of 0 to skip the first bin
  // The first bin (bin 0) usually contains the DC component of the signal,
  // which is often not useful for audio visualization.
  for (int i = 1; i < ledColumns; i++) {
    double peak = 0;
    for (int j = i * avgRange; j < (i + 1) * avgRange && j < usableSamples;
         j++) {
      if (vReal[j] > peak) {
        peak = vReal[j];
      }
    }
    // Map the peak value to a row on the LED matrix
    peakData[i - 1] = map(peak, 0, maxInput, 0, ledRows);
  }
  if (scaling) logarithmicScaling(peakData);
}

void logarithmicScaling(int* spectralData) {
  for (int i = 0; i < ledColumns; i++) {
    if (spectralData[i] > 0) {
      // Apply logarithmic scaling
      spectralData[i] = log10(spectralData[i]) * (ledRows / log10(maxInput));
    }
  }
}

void drawCircles(int* spectralData) {
  matrix.fillScreen(0);
  for (int x = 0; x < ledColumns; x++) {
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

void drawBars(int* spectralData) {
  matrix.fillScreen(0);
  for (int x = 0; x < ledColumns; x++) {
    for (int y = 0; y < spectralData[x]; y++) {
      uint32_t pixelColor = colorPallets[currentPalette][0];
      pixelColor = (y > 1) ? colorPallets[currentPalette][1] : pixelColor;
      pixelColor = (y > 3) ? colorPallets[currentPalette][2] : pixelColor;
      pixelColor = (y > 6) ? colorPallets[currentPalette][3] : pixelColor;
      matrix.drawPixel(x, ledRows - 1 - y, pixelColor);
      // if (spectralData[x] == ledRows - 1 && x == 13) {
      //   drawFirework(x, spectralData[x]);
      // }
    }
  }
  matrix.show();
}

void drawBirds() {
  if (birds == nullptr) {
    generateBirds(ledColumns, ledRows, colorPallets[currentPalette]);
  }
  updateFlock(ledColumns, ledRows);
  matrix.fillScreen(0);
  for (int i = 0; i < birdCount; i++) {
    matrix.drawPixel(birds[i].pixel.x, birds[i].pixel.y, birds[i].color);
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

void setVisualization(int newMode) {
  visualization = constrain(newMode, 0, maxVisualization);
}

void scrollText(String text) {
  matrix.setTextColor(textColor);  // Set the text color
  matrix.fillScreen(0);
  int startX = matrix.width();
  int len = text.length() * 6;  // Approx width of a character
  for (int x = startX; x > -len; x--) {
    matrix.fillScreen(0);
    matrix.setCursor(x, 0);
    matrix.print(text);
    matrix.show();
    delay(100 - textSpeed);  // Adjust speed based on textSpeed
  }
}

void testMatrix() {
  Serial.println("Begin Testing LED Matrix");
  int pixelColor = WHITE;
  // loop through each pixel from bottom left to top right
  Serial.println("Looping through each pixel from bottom left to top right");
  for (int x = 0; x < ledColumns; x++) {
    for (int y = 0; y < ledRows; y++) {
      matrix.fillScreen(0);
      matrix.drawPixel(x, y, pixelColor);
      matrix.show();
      delay(1);
    }
  }
}

void initializeWebServer() {
  // Sensitivity endpoint
  wifi.webServer.on("/sensitivity", HTTP_GET, []() {
    wifi.webServer.send(200, "text/plain", String(sensitivity));
  });
  wifi.webServer.on("/sensitivity", HTTP_POST, []() {
    if (wifi.webServer.hasArg("value")) {
      int newSensitivity = wifi.webServer.arg("value").toInt();
      setSensitivity(newSensitivity);
      wifi.webServer.send(200, "text/plain", "Sensitivity updated");
    } else {
      wifi.webServer.send(400, "text/plain", "Missing sensitivity value");
    }
  });

  // Brightness endpoint
  wifi.webServer.on("/brightness", HTTP_GET, []() {
    wifi.webServer.send(200, "text/plain", String(brightness));
  });
  wifi.webServer.on("/brightness", HTTP_POST, []() {
    if (wifi.webServer.hasArg("value")) {
      int newBrightness = wifi.webServer.arg("value").toInt();
      setBrightness(newBrightness);
      wifi.webServer.send(200, "text/plain", "Brightness updated");
    } else {
      wifi.webServer.send(400, "text/plain", "Missing brightness value");
    }
  });

  wifi.webServer.on("/visualization", HTTP_GET, []() {
    if (wifi.webServer.hasArg("mode")) {
      int newMode = wifi.webServer.arg("mode").toInt();
      setVisualization(newMode);
      wifi.webServer.send(200, "text/plain",
                          "Visualization set to: " + String(visualization));
    } else {
      wifi.webServer.send(400, "text/plain", "Missing visualization mode");
    }
  });

  wifi.webServer.on("/scrollText", HTTP_POST, []() {
    if (wifi.webServer.hasArg("text")) {
      String text = wifi.webServer.arg("text");
      scrollText(text);  // Implement this function to scroll text
      wifi.webServer.send(200, "text/plain", "Text updated");
    } else {
      wifi.webServer.send(400, "text/plain", "Missing text");
    }
  });

  wifi.webServer.on("/setTextColor", HTTP_POST, []() {
    if (wifi.webServer.hasArg("color")) {
      String color = wifi.webServer.arg("color");
      textColor = hexToColor(color);
      wifi.webServer.send(200, "text/plain", "Color updated");
    } else {
      wifi.webServer.send(400, "text/plain", "Missing color value");
    }
  });

  wifi.webServer.on("/setSpeed", HTTP_POST, []() {
    if (wifi.webServer.hasArg("speed")) {
      textSpeed = wifi.webServer.arg("speed").toInt();
      wifi.webServer.send(200, "text/plain", "Speed updated");
    } else {
      wifi.webServer.send(400, "text/plain", "Missing speed value");
    }
  });

  wifi.webServer.on("/birds", HTTP_GET, []() {
    String response = "";
    response += "max_velocity=" + String(birdMaxVelocity) + "\n";
    response += "min_velocity=" + String(birdMinVelocity) + "\n";
    response += "num_birds=" + String(birdCount) + "\n";
    response += "alignment=" + String(birdAlignment) + "\n";
    response += "cohesion=" + String(birdCohesion) + "\n";
    response += "separation=" + String(birdSeparation) + "\n";
    wifi.webServer.send(200, "text/plain", response);
  });

  wifi.webServer.on("/birds", HTTP_POST, []() {
    if (wifi.webServer.hasArg("max_velocity")) {
      birdMaxVelocity = wifi.webServer.arg("max_velocity").toFloat();
    }
    if (wifi.webServer.hasArg("min_velocity")) {
      birdMinVelocity = wifi.webServer.arg("min_velocity").toFloat();
    }
    if (wifi.webServer.hasArg("num_birds")) {
      birdCount = wifi.webServer.arg("num_birds").toInt();
    }
    if (wifi.webServer.hasArg("alignment")) {
      birdAlignment = wifi.webServer.arg("alignment").toFloat();
    }
    if (wifi.webServer.hasArg("cohesion")) {
      birdCohesion = wifi.webServer.arg("cohesion").toFloat();
    }
    if (wifi.webServer.hasArg("separation")) {
      birdSeparation = wifi.webServer.arg("separation").toFloat();
    }
    generateBirds(ledColumns, ledRows, colorPallets[currentPalette]);
    wifi.webServer.send(200, "text/plain", "Bird settings updated");
  });

  // wifi.setConnectSubroutine([]() { testMatrix(matrix, ledColumns, ledRows);
  // });
  wifi.setConnectSubroutine([]() { testMatrix(); });
  wifi.enableMDNS("spectral-analyzer");
  wifi.Start();
}
