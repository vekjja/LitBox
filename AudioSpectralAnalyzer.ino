#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <ESPWiFi.h>
#include <IOPin.h>
#include <arduinoFFT.h>

#include "colors.h"

// IOPins
IOPin ledData(12);
IOPin audio(A0, INPUT);

// LED Matrix Config
int ledRows = 8;
int ledColumns = 32;
uint8_t matrixType = NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS +
                     NEO_MATRIX_ZIGZAG;
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

ESPWiFi wifi = ESPWiFi("SpectralAnalyzer", "12345678");

void setup() {
  matrix.begin();
  Serial.begin(115200);
  matrix.setTextWrap(false);
  ledData.setPinMode(OUTPUT);
  matrix.setBrightness(brightness);
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

// void drawFirework(int x, int y) {
//   matrix.fillScreen(CYAN);
//   for (int i = 0; i < 7; i++) {
//     matrix.drawCircle(x, y, i, MAGENTA);
//   }
// }

// void scrollText() {
//   int16_t x = 0;
//   for (x = ledColumns / 2; x >= -45; x--) {
//     matrix.fillScreen(0);
//     matrix.setCursor(x, 0);
//     matrix.print(F("Hi, Kayla"));
//     matrix.show();
//     delay(10);
//   }
// }

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
      matrix.drawPixel(x, y, pixelColor);
      // if (spectralData[x] == ledRows - 1 && x == 13) {
      //   drawFirework(x, spectralData[x]);
      // }
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

void setVisualization(int newMode) {
  visualization = constrain(newMode, 0, maxVisualization);
}

void testMatrix() {
  Serial.println("Begin Testing LED Matrix");
  int testDelay = 1000;
  int pixelColor = WHITE;

  // bottom left
  Serial.println("Bottom Left");
  matrix.fillScreen(0);
  matrix.drawPixel(0, 0, pixelColor);
  matrix.show();
  delay(testDelay);

  // bottom right
  Serial.println("Bottom Right");
  matrix.fillScreen(0);
  matrix.drawPixel(ledColumns - 1, 0, pixelColor);
  matrix.show();
  delay(testDelay);

  // top right
  Serial.println("Top Right");
  matrix.fillScreen(0);
  matrix.drawPixel(ledColumns - 1, ledRows - 1, pixelColor);
  matrix.show();
  delay(testDelay);

  // top left
  Serial.println("Top Left");
  matrix.fillScreen(0);
  matrix.drawPixel(0, ledRows - 1, pixelColor);
  matrix.show();
  delay(testDelay);

  Serial.println("Drawing middle rectangle animation");
  matrix.fillScreen(0);
  matrix.drawPixel(3, 4, pixelColor);
  matrix.drawPixel(3, 3, pixelColor);
  matrix.drawPixel(4, 4, pixelColor);
  matrix.drawPixel(4, 3, pixelColor);
  matrix.show();
  delay(testDelay);

  matrix.fillScreen(0);
  matrix.drawPixel(3, 4, pixelColor);
  matrix.drawPixel(2, 3, pixelColor);
  matrix.drawPixel(5, 4, pixelColor);
  matrix.drawPixel(4, 3, pixelColor);
  matrix.show();
  delay(testDelay);

  // loop through each pixel from bottom left to top right
  Serial.println("Looping through each pixel from bottom left to top right");
  for (int x = 0; x < ledColumns; x++) {
    for (int y = 0; y < ledRows; y++) {
      matrix.fillScreen(0);
      matrix.drawPixel(x, y, pixelColor);
      matrix.show();
      delay(9);
    }
  }
}

void initializeWebServer() {
  // Sensitivity endpoint
  wifi.webserver()->on("/sensitivity", HTTP_GET, []() {
    wifi.webserver()->send(200, "text/plain", String(sensitivity));
  });
  wifi.webserver()->on("/sensitivity", HTTP_POST, []() {
    if (wifi.webserver()->hasArg("value")) {
      int newSensitivity = wifi.webserver()->arg("value").toInt();
      setSensitivity(newSensitivity);
      wifi.webserver()->send(200, "text/plain", "Sensitivity updated");
    } else {
      wifi.webserver()->send(400, "text/plain", "Missing sensitivity value");
    }
  });

  // Brightness endpoint
  wifi.webserver()->on("/brightness", HTTP_GET, []() {
    wifi.webserver()->send(200, "text/plain", String(brightness));
  });
  wifi.webserver()->on("/brightness", HTTP_POST, []() {
    if (wifi.webserver()->hasArg("value")) {
      int newBrightness = wifi.webserver()->arg("value").toInt();
      setBrightness(newBrightness);
      wifi.webserver()->send(200, "text/plain", "Brightness updated");
    } else {
      wifi.webserver()->send(400, "text/plain", "Missing brightness value");
    }
  });

  wifi.webserver()->on("/visualization", HTTP_GET, []() {
    if (wifi.webserver()->hasArg("mode")) {
      int newMode = wifi.webserver()->arg("mode").toInt();
      setVisualization(newMode);
      wifi.webserver()->send(200, "text/plain",
                             "Visualization set to: " + String(visualization));
    } else {
      wifi.webserver()->send(400, "text/plain", "Missing visualization mode");
    }
  });

  // wifi.setConnectSubroutine([]() { testMatrix(); });
  wifi.enableMDNS("spectral-analyzer");
  wifi.Start();
}
