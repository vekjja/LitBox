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

// Text configuration
String textContent = "*.*. Lit Box .*.*";
String textAnimation = "scroll";
int textSpeed = 75;
int textSize = 1;
bool textMode = false;

// Function to convert CRGB to 16-bit color
uint16_t crgbTo16bit(CRGB color) {
  uint8_t r = color.r >> 3; // 5 bits
  uint8_t g = color.g >> 2; // 6 bits
  uint8_t b = color.b >> 3; // 5 bits
  return (r << 11) | (g << 5) | b;
}

void scrollText(String text, ESPWiFi *wifi, CRGB textColor, CRGB bgColor) {
  textMode = true;

  // Check for empty text
  if (text.length() == 0) {
    textMode = false;
    return;
  }

  matrix.setTextSize(textSize);
  matrix.setTextColor(crgbTo16bit(textColor));

  // Stream text character by character to avoid memory issues
  int charWidth = 6;
  int totalWidth = text.length() * charWidth;
  int startX = LED_WIDTH;

  Serial.print("Streaming text: ");
  Serial.print(text.length());
  Serial.println(" characters");

  // Scroll through the entire text width
  for (int x = startX; x > -totalWidth; x--) {
    // Clear screen
    FastLED.clear();

    // Fill background
    fill_solid(leds, LED_WIDTH * LED_HEIGHT, bgColor);

    // Calculate which characters are visible
    int firstChar = max(0, -x / charWidth);
    int lastChar =
        min((int)text.length(), (LED_WIDTH - x + charWidth - 1) / charWidth);

    // Only draw visible characters
    if (firstChar < lastChar) {
      String visibleText = text.substring(firstChar, lastChar);
      matrix.setCursor(x + (firstChar * charWidth), 0);
      matrix.print(visibleText);
    }

    matrix.show();
    delay(100 - textSpeed);
    yield();
  }

  textMode = false;
}

void staticText(String text, CRGB textColor, CRGB bgColor) {
  textMode = true;

  matrix.setTextSize(textSize);
  matrix.setTextColor(crgbTo16bit(textColor));

  // Use original proven approach: 6 pixels per character
  int textLength = text.length() * 6;
  int xStart = (LED_WIDTH - textLength) / 2;
  int yStart = (LED_HEIGHT - 8) / 2;

  // Debug: Print text info
  Serial.print("Static text length: ");
  Serial.print(text.length());
  Serial.print(" Text width: ");
  Serial.print(textLength);
  Serial.print(" Position: x=");
  Serial.print(xStart);
  Serial.print(" y=");
  Serial.println(yStart);

  // Clear and fill background
  FastLED.clear();
  fill_solid(leds, LED_WIDTH * LED_HEIGHT, bgColor);

  matrix.setCursor(xStart, yStart);
  matrix.print(text);
  FastLED.show();

  textMode = false;
}

void waveText(String text, CRGB textColor, CRGB bgColor) {
  textMode = true;

  matrix.setTextSize(textSize);
  matrix.setTextColor(crgbTo16bit(textColor));

  // Use simple character-based width calculation (4 pixels per character for
  // better fit)
  int textPixelSize = 4;
  int textLength = text.length() * textPixelSize;

  for (int x = 0; x < LED_WIDTH + textLength; x++) {
    matrix.clear();

    // Fill background
    for (int i = 0; i < LED_WIDTH * LED_HEIGHT; i++) {
      leds[i] = bgColor;
    }

    // Sine wave for vertical position
    int y = sin(x / 2.0) * 2;
    y = constrain(y, 0, LED_HEIGHT - 8); // 8 pixels for text height

    matrix.setCursor(LED_WIDTH - x, y);
    matrix.print(text);
    matrix.show();

    delay(100 - textSpeed);
  }

  textMode = false;
}

bool textFits(String text) {
  // Check for empty text
  if (text.length() == 0) {
    return false;
  }

  // Simple calculation like Adafruit example
  return (text.length() * 6) <= LED_WIDTH;
}

void displayOrScrollText(String text, ESPWiFi *wifi, CRGB textColor,
                         CRGB bgColor) {
  if (text.length() > 20) {
    scrollText(text, wifi, textColor, bgColor);
  } else if (textFits(text)) {
    staticText(text, textColor, bgColor);
  } else {
    scrollText(text, wifi, textColor, bgColor);
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
        String animation = textObj["animation"] | "scroll";
        int speed = textObj["speed"] | 75;
        int size = textObj["size"] | 1;

        if (content.length() == 0) {
          AsyncWebServerResponse *response = request->beginResponse(
              400, "application/json", "{\"error\":\"NoContent\"}");
          device->addCORS(response);
          request->send(response);
          device->logError("/text Error: No content provided");
          return;
        }

        // Set text parameters
        textContent = content;
        textSpeed = speed;
        textSize = size;

        // Serialize the JsonObject to a string before sending
        String responseStr;
        serializeJson(textObj, responseStr);
        AsyncWebServerResponse *response =
            request->beginResponse(200, "application/json", responseStr);
        device->addCORS(response);
        request->send(response);

        // Display text based on animation type
        if (animation == "static") {
          staticText(textContent, pixelColor, pixelBgColor);
        } else if (animation == "wave") {
          waveText(textContent, pixelColor, pixelBgColor);
        } else if (animation == "scroll") {
          scrollText(textContent, device, pixelColor, pixelBgColor);
        } else if (animation == "display") {
          displayOrScrollText(textContent, device, pixelColor, pixelBgColor);
        }
      }));
}

#endif