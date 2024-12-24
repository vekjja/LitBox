#ifndef SPECTRAL_ANALYZER_H
#define SPECTRAL_ANALYZER_H

#include <arduinoFFT.h>

// Audio Device Configuration
#define AUDIO_PIN A0
const int maxInput = 4095;          // 12-bit ADC on ESP32-S3
const uint16_t audioSamples = 256;  // This value MUST be a power of 2
const int usableSamples = (audioSamples / 2);
double vReal[audioSamples];
double vImaginary[audioSamples];
int* spectralData = nullptr;  // Array to store spectral data for matrix
const double samplingFrequency =
    16000.0;  // Set high enough to analyze up to 8 kHz

ArduinoFFT<double> FFT =
    ArduinoFFT<double>(vReal, vImaginary, audioSamples, samplingFrequency);

// Configurable Values
const int minSensitivity = 1;
const int maxSensitivity = 100;
int sensitivity = 9;

void initializeSpectralAnalyzer() {
  pinMode(AUDIO_PIN, INPUT);       // Set audio pin as input
  analogReadResolution(12);        // Set ADC resolution to 12-bit
  analogSetAttenuation(ADC_11db);  // 0-3.3V range for ADC
  Serial.println("Spectral Analyzer Initialized");
}

void spectralAnalyzer(int matrixWidth, int matrixHeight) {
  if (spectralData == nullptr) {
    spectralData =
        new int[matrixWidth];  // Dynamically allocate for matrix width
  }

  // Read audio samples
  for (int i = 0; i < audioSamples; i++) {
    vReal[i] = analogRead(AUDIO_PIN) * (sensitivity / 10.0);
    vImaginary[i] = 0;  // FFT requires imaginary part initialized to zero
    delayMicroseconds(1000000 / samplingFrequency);  // Maintain sampling rate
  }

  // Perform FFT
  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);
  FFT.compute(FFTDirection::Forward);
  FFT.complexToMagnitude();

  // Map FFT output to LED matrix
  int binsPerColumn = usableSamples / matrixWidth;
  for (int x = 0; x < matrixWidth; x++) {
    double peak = 0;
    int startBin = x * binsPerColumn + 3;  // Skip FFT bins
    int endBin = startBin + binsPerColumn;

    for (int bin = startBin; bin < endBin && bin < usableSamples; bin++) {
      peak = max(peak, vReal[bin]);  // Get the maximum magnitude in the range
    }

    // Map the peak value to the LED matrix height
    spectralData[x] = map(peak, 0, maxInput, 0, matrixHeight);
    spectralData[x] = constrain(spectralData[x], 0, matrixHeight);
  }
}

#endif  // SPECTRAL_ANALYZER_H
