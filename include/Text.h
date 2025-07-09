#ifndef TEXT_H
#define TEXT_H

#include "Colors.h"
#include "LEDMatrix.h"
#include <Adafruit_GFX.h>

// Custom FastLED NeoMatrix class for our LED matrix
class FastLED_NeoMatrix : public Adafruit_GFX {
private:
  CRGB *leds;
  uint16_t numLeds;
  uint8_t width;
  uint8_t height;

  uint16_t XY(uint8_t x, uint8_t y) {
    if (x % 2 == 0) {
      return x * height + y; // Even columns top to bottom
    } else {
      return x * height + (height - 1 - y); // Odd columns bottom to top
    }
  }

public:
  FastLED_NeoMatrix(CRGB *ledArray, uint8_t w, uint8_t h)
      : Adafruit_GFX(w, h), leds(ledArray), width(w), height(h) {
    numLeds = w * h;
  }

  void drawPixel(int16_t x, int16_t y, uint16_t color) override {
    if (x < 0 || x >= width || y < 0 || y >= height)
      return;

    uint16_t index = XY(x, y);
    if (index < numLeds) {
      // Convert 16-bit color to CRGB
      uint8_t r = (color >> 11) << 3;         // 5 bits red
      uint8_t g = ((color >> 5) & 0x3F) << 2; // 6 bits green
      uint8_t b = (color & 0x1F) << 3;        // 5 bits blue
      leds[index] = CRGB(r, g, b);
    }
  }

  void drawPixel(int16_t x, int16_t y, CRGB color) {
    if (x < 0 || x >= width || y < 0 || y >= height)
      return;

    uint16_t index = XY(x, y);
    if (index < numLeds) {
      leds[index] = color;
    }
  }

  void clear() { FastLED.clear(); }

  void show() { FastLED.show(); }
};

// Global matrix instance
FastLED_NeoMatrix matrix(leds, LED_WIDTH, LED_HEIGHT);

const int maxLen = 200;

// Text configuration
char textContent[maxLen] = "*.*. Lit Box .*.*"; // 40 chars max + null
char textAnimation[16] = "scroll";
bool textRequested = false;
int textSpeed = 75;
int textSize = 1;

// Function to convert CRGB to 16-bit color
uint16_t crgbTo16bit(CRGB color) {
  uint8_t r = color.r >> 3; // 5 bits
  uint8_t g = color.g >> 2; // 6 bits
  uint8_t b = color.b >> 3; // 5 bits
  return (r << 11) | (g << 5) | b;
}

void scrollText(const char *text) {

  matrix.setTextSize(textSize);
  matrix.setTextColor(crgbTo16bit(pixelColor));

  const int charWidth = 6;
  int textPixelWidth = strlen(text) * charWidth;
  int scrollEnd = textPixelWidth + 1;

  for (int x = 0; x < scrollEnd; x++) {
    fill_solid(leds, LED_WIDTH * LED_HEIGHT, pixelBgColor);
    matrix.setCursor(-x, 0);
    matrix.print(text);
    matrix.show();
    delay(constrain(120 - textSpeed, 20, 200));
    yield();
  }
}

void staticText(const char *text) {
  if (!text || strlen(text) == 0) {
    return;
  }
  matrix.setTextSize(textSize);
  matrix.setTextColor(crgbTo16bit(pixelColor));
  int textLength = strlen(text) * 6;
  int xStart = (LED_WIDTH - textLength) / 2;
  int yStart = (LED_HEIGHT - 8) / 2;
  FastLED.clear();
  fill_solid(leds, LED_WIDTH * LED_HEIGHT, pixelBgColor);
  matrix.setCursor(xStart, yStart);
  matrix.print(text);
  FastLED.show();
}

void waveText(const char *text) {
  if (!text || strlen(text) == 0) {
    return;
  }
  matrix.setTextSize(textSize);
  matrix.setTextColor(crgbTo16bit(pixelColor));
  int textPixelSize = 4;
  int textLength = strlen(textContent) * textPixelSize;
  for (int x = 0; x < LED_WIDTH + textLength; x++) {
    matrix.clear();
    for (int i = 0; i < LED_WIDTH * LED_HEIGHT; i++) {
      leds[i] = pixelBgColor;
    }
    int y = sin(x / 2.0) * 2;
    y = constrain(y, 0, LED_HEIGHT - 8);
    matrix.setCursor(LED_WIDTH - x, y);
    matrix.print(textContent);
    matrix.show();
    delay(100 - textSpeed);
  }
}

bool textFits(const char *text) {
  if (!text || strlen(text) == 0) {
    return false;
  }
  return (strlen(text) * 6) <= LED_WIDTH;
}

void displayOrScrollText(const char *text) {
  if (!text || strlen(text) == 0) {
    return;
  }
  if (strlen(text) > 20) {
    scrollText(text);
  } else if (textFits(text)) {
    staticText(text);
  } else {
    scrollText(text);
  }
}

// Function to handle deferred text display from main loop
void handleText() {
  if (textRequested) {
    if (strcmp(textAnimation, "static") == 0) {
      staticText(textContent);
    } else if (strcmp(textAnimation, "wave") == 0) {
      waveText(textContent);
    } else if (strcmp(textAnimation, "scroll") == 0) {
      scrollText(textContent);
    } else if (strcmp(textAnimation, "display") == 0) {
      displayOrScrollText(textContent);
    }
    textRequested = false;
  }
}

void startText(ESPWiFi *device) {
  device->initWebServer();
  device->webServer->addHandler(new AsyncCallbackJsonWebHandler(
      "/text", [device](AsyncWebServerRequest *request, JsonVariant &json) {
        if (json.isNull() || !json.is<JsonObject>()) {
          AsyncWebServerResponse *response = request->beginResponse(
              400, "application/json", "{\"error\":\"EmptyInput\"}");
          device->addCORS(response);
          request->send(response);
          device->logError("/text Error parsing JSON: EmptyInput");
          return;
        }

        JsonObject jsonObject = json.as<JsonObject>();
        JsonObject textObj = jsonObject["text"];
        if (textObj.isNull()) {
          AsyncWebServerResponse *response = request->beginResponse(
              400, "application/json", "{\"error\":\"NoTextObject\"}");
          device->addCORS(response);
          request->send(response);
          device->logError("/text Error: No text object in JSON");
          return;
        }

        String content = textObj["content"] | "";
        content = "     " + content;
        String animation = textObj["animation"] | "scroll";
        int speed = textObj["speed"] | 75;

        if (content.length() == 0) {
          AsyncWebServerResponse *response = request->beginResponse(
              400, "application/json", "{\"error\":\"NoContent\"}");
          device->addCORS(response);
          request->send(response);
          device->logError("/text Error: No content provided");
          return;
        }

        // Copy to char arrays
        strncpy(textContent, content.c_str(), maxLen);
        textContent[maxLen] = '\0';
        strncpy(textAnimation, animation.c_str(), 15);
        textAnimation[15] = '\0';
        textSpeed = speed;

        // Defer all text display to main loop to avoid conflicts
        textRequested = true;

        String responseStr;
        serializeJson(textObj, responseStr);
        AsyncWebServerResponse *response =
            request->beginResponse(200, "application/json", responseStr);
        device->addCORS(response);
        request->send(response);
      }));
}

#endif