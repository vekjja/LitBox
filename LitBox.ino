#include <ESPWiFi.h>
#include <FastLED.h>

// LED Matrix configuration
#define LED_PIN 6
#define LED_WIDTH 32
#define LED_HEIGHT 8
#define NUM_LEDS (LED_WIDTH * LED_HEIGHT)

CRGB leds[NUM_LEDS];

// Layout settings
const bool kMatrixSerpentineLayout = false;

// Brightness Config
const int maxBrightness = 255;
const int minBrightness = 1;
int brightness = 18;

// Web Server Config
ESPWiFi wifi;

void setup() {
  initializeMatrix();
  initializeWebServer();
}

void initializeMatrix() {
  // Initialize FastLED
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);

  // Clear the matrix
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

void testMatrix(CRGB* leds) {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(10);
    leds[i] = CRGB::Black;
  }

  // Clear and light up (0, 0)
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  leds[XY(0, 0)] = CRGB::Blue;  // Bottom-left corner
  FastLED.show();
  delay(1000);

  // Clear and light up (0, top)
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  leds[XY(0, LED_HEIGHT - 1)] = CRGB::Blue;  // Top-left corner
  FastLED.show();
  delay(1000);

  // Clear and light up (right, top)
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  leds[XY(LED_WIDTH - 1, LED_HEIGHT - 1)] = CRGB::Blue;  // Top-right corner
  FastLED.show();
  delay(1000);

  // Clear and light up (right, bottom)
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  leds[XY(LED_WIDTH - 1, 0)] = CRGB::Blue;  // Bottom-right corner
  FastLED.show();
  delay(1000);
}

// Helper function to map 2D coordinates to 1D array index
uint16_t XY(uint8_t x, uint8_t y) {
  if (kMatrixSerpentineLayout) {
    if (x % 2 == 0) {
      // Even columns go top to bottom
      return (x * LED_HEIGHT) + y;
    } else {
      // Odd columns go bottom to top
      return (x * LED_HEIGHT) + (LED_HEIGHT - 1 - y);
    }
  } else {
    // Standard column-major layout
    return (x * LED_HEIGHT) + y;
  }
}

void loop() {
  wifi.handleClient();
  testMatrix(leds);
}

void initializeWebServer() { wifi.start(); }
