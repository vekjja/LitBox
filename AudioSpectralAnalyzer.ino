#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <ButtonManager.h>
#include <IOPin.h>
#include <arduinoFFT.h>

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define ORANGE 0xFD20
#define TEAL 0x0410
#define VIOLET 0xEC1D
#define OLIVE 0x5345
#define GOLD 0xFEA0
#define SILVER 0xC618
#define DARK_GREEN 0x0320
#define FOREST_GREEN 0x2444
#define CORAL 0xFBEA
#define SALMON 0xFC0E
#define ROSE 0xF8A6
#define PEACH 0xFEA6

// IOPins
IOPin ledData(6);
IOPin audio(A3, INPUT);
IOPin btn1Pin(3, INPUT_PULLUP);
IOPin btn2Pin(9, INPUT_PULLUP);

// LED Matrix Config
int ledRows = 8;
int ledColumns = 32;
uint8_t matrixType =
    NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG;
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
    ledColumns, ledRows, ledData.pin(), matrixType, NEO_GRB + NEO_KHZ800);

// Brightness and Color Config
const int minBrightness = 3;
const int brightnessStep = 81;
const int maxBrightness = 255;
const int colorPalletCount = 7;
volatile int currentPalette = 0;
volatile int brightness = minBrightness;
uint32_t colorPallets[colorPalletCount][4] = {
    {GREEN, YELLOW, ORANGE, RED},       {BLUE, CYAN, CYAN, VIOLET},
    {MAGENTA, MAGENTA, VIOLET, VIOLET}, {CORAL, SALMON, SALMON, ROSE},
    {ROSE, ROSE, ROSE, ROSE},           {GREEN, GREEN, GREEN, GREEN},
    {WHITE, WHITE, WHITE, WHITE},
};

// Audio Config
arduinoFFT FFT = arduinoFFT();
const int minSensitivity = 3;
const int maxSensitivity = 100;
const int sensitivityStep = 20;
const uint16_t audioSamples = 32;
const int usableSamples = (audioSamples / 2);
double vReal[audioSamples];
double vImage[audioSamples];
volatile int sensitivity = minSensitivity;

// Visualization Config
volatile int visualization = 0;
int maxVisualization = 3;

// Button Config
ButtonManager btn1(btn1Pin.pin());

void setup() {
  matrix.begin();
  Serial.begin(115200);
  initButtonHandlers();
  matrix.setTextWrap(false);
  matrix.setBrightness(brightness);
}

void loop() {
  // testMatrix();
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

  int spectralData[ledColumns] = {};
  int spectralIndex = 0, maxInput = 80;

  for (int i = 2; i < usableSamples; i++) {
    vReal[i] =
        constrain(vReal[i], 0, maxInput);  // set max value for input data

    vReal[i] = map(vReal[i], 0, maxInput, 0,
                   ledRows + 1);  // map data to fit our display

    spectralData[spectralIndex] = vReal[i];
    spectralData[spectralIndex + 1] = vReal[i];
    spectralIndex += 2;
  }

  matrix.fillScreen(0);
  switch (visualization) {
    case 1:
      drawCircles(spectralData);
      break;
    case 2:
      drawText();
      break;
    default:
      drawBars(spectralData);
      break;
  }
  matrix.show();
}

void drawText() {
  matrix.fillScreen(0);
  matrix.setCursor(0, 0);
  matrix.setTextColor(colorPallets[currentPalette][0]);
  matrix.print(F("INIKO"));
  matrix.show();
}

void drawCircles(int *spectralData) {
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

void drawBars(int *spectralData) {
  matrix.fillScreen(0);

  for (int x = 0; x < ledColumns; x++) {
    for (int y = ledRows - 1; y >= ledRows - spectralData[x]; y--) {
      uint32_t pixelColor = colorPallets[currentPalette][3];
      pixelColor =
          (y > ledRows - 6) ? colorPallets[currentPalette][2] : pixelColor;
      pixelColor =
          (y > ledRows - 7) ? colorPallets[currentPalette][1] : pixelColor;
      pixelColor =
          (y > ledRows - 3) ? colorPallets[currentPalette][0] : pixelColor;
      matrix.drawPixel(x + 1, y, pixelColor);
    }
  }
  matrix.show();
}

void initButtonHandlers() {
  attachInterrupt(
      digitalPinToInterrupt(btn1.pin()), []() { btn1.isr(); }, CHANGE);
  // btn1.setVerbose(true);
  btn1.setSingleClickCallback([]() {
    if (btn2Pin.readD() == LOW) {
      Serial.println("Changing color pallet");
      currentPalette = (currentPalette + 1) % colorPalletCount;
    } else {
      Serial.println("Changing brightness");
      brightness = (brightness + brightnessStep > maxBrightness)
                       ? minBrightness
                       : brightness + brightnessStep;
      matrix.setBrightness(brightness);
    }
  });
  btn1.setDoubleClickCallback([]() { Serial.println("Double Click"); });
  btn1.setLongClickCallback([]() {
    if (btn2Pin.readD() == LOW) {
      Serial.println("Changing sensitivity");
      sensitivity = (sensitivity + sensitivityStep > maxSensitivity)
                        ? minSensitivity
                        : sensitivity + sensitivityStep;
    } else {
      Serial.println("Changing Visualizer");
      visualization =
          (visualization + 1 > maxVisualization) ? 0 : visualization + 1;
    }
  });
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
      delay(50);
    }
  }
}
