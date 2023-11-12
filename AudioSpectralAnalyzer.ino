#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <ESPWiFi.h>
#include <IOPin.h>
#include <arduinoFFT.h>

#include "HTML.h"
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
int currentPalette = 0;
int brightness = minBrightness;
uint32_t colorPallets[colorPalletCount][4] = {
    {GREEN, YELLOW, ORANGE, RED},       {BLUE, CYAN, CYAN, VIOLET},
    {MAGENTA, MAGENTA, VIOLET, VIOLET}, {CORAL, SALMON, SALMON, ROSE},
    {ROSE, ROSE, ROSE, ROSE},           {GREEN, GREEN, GREEN, GREEN},
    {WHITE, WHITE, WHITE, WHITE},
};

// Audio Config
arduinoFFT FFT = arduinoFFT();
const int minSensitivity = 1;
const int maxSensitivity = 100;
const uint16_t audioSamples = 128;
const int usableSamples = (audioSamples / 2);
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
  matrix.setBrightness(brightness);
  initializeWebServer();
  // testMatrix();
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

  int* spectralData = averageSamples();

  switch (visualization) {
    case 1:
      drawCircles(spectralData);
      break;
    default:
      drawBars(spectralData);
      break;
  }
}

int* averageSamples() {
  int* spectralData = new int[ledColumns]{0};

  int sum = 0;
  int sampleCount = 0;
  int spectralIndex = 0;
  int maxInput = 81;
  int avgRange = (usableSamples / ledColumns);

  // Process the FFT data
  for (int i = 6; i < usableSamples; i++) {
    vReal[i] = map(vReal[i], 0, maxInput, 0, ledRows);
    sum += vReal[i];
    sampleCount++;
    if (i % avgRange == 0) {
      int data = sum / sampleCount;
      spectralData[spectralIndex++] = data;
      sum = 0;
      sampleCount = 0;
    }
  }

  return spectralData;  // Returning the pointer to the array
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
    matrix.drawCircle(x, 4, circleRadius, circleColor);
  }
  matrix.show();
}

void drawBars(int* spectralData) {
  matrix.fillScreen(0);
  for (int x = 0; x < ledColumns; x++) {
    for (int y = 0; y < spectralData[x]; y++) {
      uint32_t pixelColor = colorPallets[currentPalette][3];
      pixelColor =
          (y > ledRows - 6) ? colorPallets[currentPalette][2] : pixelColor;
      pixelColor =
          (y > ledRows - 7) ? colorPallets[currentPalette][1] : pixelColor;
      pixelColor =
          (y > ledRows - 3) ? colorPallets[currentPalette][0] : pixelColor;
      matrix.drawPixel(x, y, pixelColor);
      // if (spectralData[x] == ledRows - 1 && x == 13) {
      //   drawFirework(x, spectralData[x]);
      // }
    }
  }
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
  wifi.setConnectSubroutine([]() { testMatrix(); });
  wifi.enableMDNS("spectral-analyzer");

  wifi.webserver()->on("/", HTTP_GET, []() {
    wifi.webserver()->send(200, "text/html", indexHTML);
  });

  wifi.webserver()->on("/set/sensitivity", HTTP_GET, []() {
    if (wifi.webserver()->hasArg("value")) {
      int newSensitivity = wifi.webserver()->arg("value").toInt();
      setSensitivity(newSensitivity);
      wifi.webserver()->send(200, "text/plain",
                             "Sensitivity set to: " + String(sensitivity));
    } else {
      wifi.webserver()->send(400, "text/plain", "Missing sensitivity value");
    }
  });

  wifi.webserver()->on("/set/visualization", HTTP_GET, []() {
    if (wifi.webserver()->hasArg("mode")) {
      int newMode = wifi.webserver()->arg("mode").toInt();
      setVisualization(newMode);
      wifi.webserver()->send(200, "text/plain",
                             "Visualization set to: " + String(visualization));
    } else {
      wifi.webserver()->send(400, "text/plain", "Missing visualization mode");
    }
  });

  wifi.APToClientMode();
}
