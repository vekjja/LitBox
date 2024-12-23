#ifndef SPECTRAL_ANALYZER_H
#define SPECTRAL_ANALYZER_H

#include <Arduino.h>
#include <arduinoFFT.h>

#define AUDIO_PIN A2

// Audio Config
const int maxInput = 4096;  // 12-bit ADC
const int minSensitivity = 1;
const int maxSensitivity = 100;
const uint16_t audioSamples = 128;  // This value MUST ALWAYS be a power of 2
const int usableSamples = (audioSamples / 2);
int* spectralData = nullptr;

// must be less than 10000 due to ADC
const double samplingFrequency = 8000;  // Hz
double vReal[audioSamples];
double vImage[audioSamples];
ArduinoFFT<double> FFT =
    ArduinoFFT<double>(vReal, vImage, audioSamples, samplingFrequency);
int sensitivity = 9;

void initializeSpectralAnalyzer() {
  pinMode(AUDIO_PIN, INPUT);  // Set pin as input
  analogReadResolution(12);   // Ensure 12-bit resolution
  Serial.println("Spectral Analyzer Initialized");
}

void peakDetection(int* peakData, int maxWidth, int maxHeight) {
  int avgRange = (usableSamples - 1) /
                 maxWidth;  // Adjust the range, skipping the first bin
  // Start the loop from 1 to skip the first bin
  for (int i = 1; i <= maxWidth; i++) {
    double peak = 0;
    int startFreqBin = (i)*avgRange + 1;  // Adjust start to skip first bin
    int endFreqBin = startFreqBin + avgRange;

    for (int j = startFreqBin; j < endFreqBin && j < usableSamples; j++) {
      if (vReal[j] > peak) {
        peak = vReal[j];
      }
    }
    // Map the peak value to a row on the LED matrix
    peakData[i - 1] = map(peak, 0, maxInput, 0, maxHeight);
  }
}

void spectralAnalyzer(int maxWidth, int maxHeight) {
  for (int i = 0; i < audioSamples; i++) {
    // Read and normalize raw value
    int rawValue = analogRead(AUDIO_PIN);
    double normalizedValue =
        rawValue / double(maxInput);  // Normalize to [0, 1]

    // Scale based on sensitivity
    vReal[i] = normalizedValue * sensitivity * maxInput / 10.0;
    vImage[i] = 0;  // Set imaginary part to 0

    // Serial.println("Raw Value: " + String(rawValue) +
    //                " Normalized Value: " + String(normalizedValue) +
    //                " Scaled Value: " + String(vReal[i]));
  }

  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward); /* Apply window */
  FFT.compute(FFTDirection::Forward);                       /* Compute FFT */
  FFT.complexToMagnitude(); /* Compute magnitudes */

  // Ensure `spectralData` is initialized
  if (spectralData == nullptr) {
    spectralData = new int[maxWidth];
  }

  // Detect peaks and store in `spectralData`
  peakDetection(spectralData, maxWidth, maxHeight);
}

#endif  // SPECTRAL_ANALYZER_H