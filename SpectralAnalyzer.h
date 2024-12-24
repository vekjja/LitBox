#ifndef SPECTRAL_ANALYZER_H
#define SPECTRAL_ANALYZER_H

#include <arduinoFFT.h>

// Audio Config
#define AUDIO_PIN A0
const int maxInput = 4095;  // Adjust for 12-bit ADC on ESP32-S3
const int minSensitivity = 1;
const int maxSensitivity = 100;
const uint16_t audioSamples = 128;  // This value MUST ALWAYS be a power of 2
const int usableSamples = (audioSamples / 2);
int* spectralData = nullptr;

// must be less than 10000 due to ADC
const double samplingFrequency = 8000;                 // Hz
double samplingDelay = (1000000 / samplingFrequency);  // microseconds
double vReal[audioSamples];
double vImage[audioSamples];
ArduinoFFT<double> FFT =
    ArduinoFFT<double>(vReal, vImage, audioSamples, samplingFrequency);
int sensitivity = 9;

void initializeSpectralAnalyzer() {
  pinMode(AUDIO_PIN, INPUT);  // Set the audio pin as input
  analogReadResolution(12);   // 12-bit ADC resolution
  // analogSetAttenuation(ADC_11db);  // Use appropriate attenuation (0-3.3V
  // range)
  Serial.println("Spectral Analyzer Initialized");
}

void peakDetection(int* peakData, int maxWidth, int maxHeight) {
  int avgRange = (usableSamples - 1) /
                 maxWidth;  // Divide FFT bins across the LED matrix width
  for (int i = 0; i < maxWidth; i++) {
    double peak = 0;
    int startFreqBin = (i * avgRange) + 1;  // Skip the first FFT bin
    int endFreqBin = startFreqBin + avgRange;

    for (int j = startFreqBin; j < endFreqBin && j < usableSamples; j++) {
      if (vReal[j] > peak) {
        peak = vReal[j];
      }
    }
    peakData[i] = map(peak, 0, maxInput, 0, maxHeight);  // Map to matrix height
    peakData[i] = constrain(peakData[i], 0, maxHeight);  // Clamp to height
  }
}

void spectralAnalyzer(int maxWidth, int maxHeight) {
  for (int i = 0; i < audioSamples; i++) {
    vReal[i] = analogRead(AUDIO_PIN);  // Read raw ADC value
    vImage[i] = 0;                     // Set imaginary part to zero
    delayMicroseconds(samplingDelay);  // Ensure 8 kHz sampling rate
  }

  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward); /* Weigh data */
  FFT.compute(FFTDirection::Forward);                       /* Compute FFT */
  FFT.complexToMagnitude(); /* Compute magnitudes */

  if (spectralData == nullptr) {
    spectralData = new int[maxWidth];
  }
  peakDetection(spectralData, maxWidth, maxHeight);
}
#endif  // SPECTRAL_ANALYZER_H
