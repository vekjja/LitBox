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
struct Pixel {
  int x;
  int y;
  byte intensity;
};

// Birds
struct Bird {
  Pixel pixel;
  int vx, vy;      // Velocity
  uint32_t color;  // Color
};

const int NUM_BIRDS = 6;
uint32_t birdColor = 0;
Bird birds[NUM_BIRDS];

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

// void drawFirework(int x, int y) {
//   matrix.fillScreen(CYAN);
//   for (int i = 0; i < 7; i++) {
//     matrix.drawCircle(x, y, i, MAGENTA);
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
      matrix.drawPixel(x, ledRows - 1 - y, pixelColor);
      // if (spectralData[x] == ledRows - 1 && x == 13) {
      //   drawFirework(x, spectralData[x]);
      // }
    }
  }
  matrix.show();
}

void drawBirds() {
  if (birdColor == 0) {
    hatchBirds();
  }
  updateFlock();
  matrix.fillScreen(0);
  for (int i = 0; i < NUM_BIRDS; i++) {
    matrix.drawPixel(birds[i].pixel.x, birds[i].pixel.y, birds[i].color);
  }
  matrix.show();
}

void hatchBirds() {
  for (int i = 0; i < NUM_BIRDS; i++) {
    birdColor = colorPallets[currentPalette][random(0, 4)];
    birds[i].pixel.x = random(0, ledColumns);
    birds[i].pixel.y = random(0, ledRows);
    birds[i].pixel.intensity = random(100, 255);  // Random intensity
    birds[i].vx = random(0, 3);                   // Random velocity X
    birds[i].vy = random(0, 3);                   // Random velocity Y
    birds[i].color = birdColor;  // Function to generate a random color
  }
}

void updateFlock() {
  const float ALIGNMENT_FACTOR = 0.1;
  const float ALIGNMENT_THRESHOLD = 90;
  const float COHESION_FACTOR = 45;
  const float MAX_VELOCITY = 2;
  const float MIN_VELOCITY = 0;
  const float SEPARATION_THRESHOLD = 1;
  const int edgeBuffer = 1;  // Distance from edge to start avoiding
  const float randomVelocityChangeFactor = 1;  // Max random change in velocity
  const int randomChangeChance = 15;  // Chance of random change (in percentage)

  for (int i = 0; i < NUM_BIRDS; i++) {
    float avgVx = 0;
    float avgVy = 0;
    float avgX = 0;
    float avgY = 0;
    int count = 0;

    for (int j = 0; j < NUM_BIRDS; j++) {
      if (i != j) {
        int distanceX = birds[i].pixel.x - birds[j].pixel.x;
        int distanceY = birds[i].pixel.y - birds[j].pixel.y;
        int distanceSquared = distanceX * distanceX + distanceY * distanceY;

        if (distanceSquared < SEPARATION_THRESHOLD * SEPARATION_THRESHOLD) {
          avgVx -= distanceX;
          avgVy -= distanceY;
        } else if (distanceSquared <
                   ALIGNMENT_THRESHOLD * ALIGNMENT_THRESHOLD) {
          avgVx += birds[j].vx;
          avgVy += birds[j].vy;
          count++;
        }
      }
    }

    if (count > 0) {
      avgVx /= count;
      avgVy /= count;
      birds[i].vx += avgVx * ALIGNMENT_FACTOR;
      birds[i].vy += avgVy * ALIGNMENT_FACTOR;
    }

    // Edge Collision Avoidance for Top and Bottom Edges
    if (birds[i].pixel.y <= edgeBuffer) {
      birds[i].vy += 1;  // Steer down
    } else if (birds[i].pixel.y >= ledRows - edgeBuffer - 1) {
      birds[i].vy -= 1;  // Steer up
    }

    // Edge Collision Avoidance for Left and Right Edges
    if (birds[i].pixel.x <= edgeBuffer) {
      birds[i].vx += 1;  // Steer right
    } else if (birds[i].pixel.x >= ledColumns - edgeBuffer - 1) {
      birds[i].vx -= 1;  // Steer left
    }

    // Random Velocity Change
    if (random(100) < randomChangeChance) {
      birds[i].vx +=
          random(-randomVelocityChangeFactor, randomVelocityChangeFactor);
      birds[i].vy +=
          random(-randomVelocityChangeFactor, randomVelocityChangeFactor);
    }

    // Limiting velocity
    float speed = sqrt(birds[i].vx * birds[i].vx + birds[i].vy * birds[i].vy);
    if (speed > MAX_VELOCITY) {
      birds[i].vx = (birds[i].vx / speed) * MAX_VELOCITY;
      birds[i].vy = (birds[i].vy / speed) * MAX_VELOCITY;
    } else if (speed < MIN_VELOCITY) {
      birds[i].vx = (birds[i].vx / speed) * MIN_VELOCITY;
      birds[i].vy = (birds[i].vy / speed) * MIN_VELOCITY;
    }

    // Update position with vertical boundary check
    birds[i].pixel.y += birds[i].vy;
    if (birds[i].pixel.y < 0) {
      birds[i].pixel.y = 0;
    } else if (birds[i].pixel.y >= ledRows) {
      birds[i].pixel.y = ledRows - 1;
    }

    // Update position with horizontal boundary check
    birds[i].pixel.x += birds[i].vx;
    if (birds[i].pixel.x < 0) {
      birds[i].pixel.x = 0;
    } else if (birds[i].pixel.x >= ledColumns) {
      birds[i].pixel.x = ledColumns - 1;
    }
  }
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

uint32_t hexToColor(String hexColor) {
  // Remove the '#' character if present
  if (hexColor.startsWith("#")) {
    hexColor = hexColor.substring(1);
  }

  // Convert the hex string to a long integer
  unsigned long number = strtoul(hexColor.c_str(), NULL, 16);

  // Split into RGB components
  int r = (number >> 16) & 0xFF;
  int g = (number >> 8) & 0xFF;
  int b = number & 0xFF;

  // Reorder the components to match the GRB format for WS2812B LEDs
  return ((g & 0xFF) << 16) | ((r & 0xFF) << 8) | (b & 0xFF);
}

void testMatrix() {
  Serial.println("Begin Testing LED Matrix");
  int testDelay = 1000;
  int pixelColor = WHITE;

  // // bottom left
  // Serial.println("Bottom Left");
  // matrix.fillScreen(0);
  // matrix.drawPixel(0, 0, pixelColor);
  // matrix.show();
  // delay(testDelay);

  // // bottom right
  // Serial.println("Bottom Right");
  // matrix.fillScreen(0);
  // matrix.drawPixel(ledColumns - 1, 0, pixelColor);
  // matrix.show();
  // delay(testDelay);

  // // top right
  // Serial.println("Top Right");
  // matrix.fillScreen(0);
  // matrix.drawPixel(ledColumns - 1, ledRows - 1, pixelColor);
  // matrix.show();
  // delay(testDelay);

  // // top left
  // Serial.println("Top Left");
  // matrix.fillScreen(0);
  // matrix.drawPixel(0, ledRows - 1, pixelColor);
  // matrix.show();
  // delay(testDelay);

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

void runAtFrameRate(void (*functionToRun)(), unsigned int fps) {
  static unsigned long lastFrameTime = 0;
  static unsigned int frameDuration = 0;
  if (fps != 0) {
    frameDuration = 1000 / fps;
  }

  unsigned long currentTime = millis();
  if (currentTime - lastFrameTime >= frameDuration) {
    lastFrameTime = currentTime;
    functionToRun();  // Call the passed function
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
      if (newMode == 2) {
        hatchBirds();
      }
      setVisualization(newMode);
      wifi.webserver()->send(200, "text/plain",
                             "Visualization set to: " + String(visualization));
    } else {
      wifi.webserver()->send(400, "text/plain", "Missing visualization mode");
    }
  });

  wifi.webserver()->on("/scrollText", HTTP_POST, []() {
    if (wifi.webserver()->hasArg("text")) {
      String text = wifi.webserver()->arg("text");
      scrollText(text);  // Implement this function to scroll text
      wifi.webserver()->send(200, "text/plain", "Text updated");
    } else {
      wifi.webserver()->send(400, "text/plain", "Missing text");
    }
  });

  wifi.webserver()->on("/setTextColor", HTTP_POST, []() {
    if (wifi.webserver()->hasArg("color")) {
      String color = wifi.webserver()->arg("color");
      textColor = hexToColor(color);
      wifi.webserver()->send(200, "text/plain", "Color updated");
    } else {
      wifi.webserver()->send(400, "text/plain", "Missing color value");
    }
  });

  wifi.webserver()->on("/setSpeed", HTTP_POST, []() {
    if (wifi.webserver()->hasArg("speed")) {
      textSpeed = wifi.webserver()->arg("speed").toInt();
      wifi.webserver()->send(200, "text/plain", "Speed updated");
    } else {
      wifi.webserver()->send(400, "text/plain", "Missing speed value");
    }
  });

  wifi.setConnectSubroutine([]() { testMatrix(); });
  wifi.enableMDNS("spectral-analyzer");
  wifi.Start();
}
