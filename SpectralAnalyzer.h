#ifndef SPECTRAL_ANALYZER_H
#define SPECTRAL_ANALYZER_H

#include <arduinoFFT.h>

// Audio Config
#define AUDIO_PIN A0
const int maxInput = 4095;  // 12-bit ADC on ESP32-S3
const int minSensitivity = 1;
const int maxSensitivity = 100;
const uint16_t audioSamples = 128;  // This value MUST ALWAYS be a power of 2
const int usableSamples = (audioSamples / 2);
int* spectralData = nullptr;  // data for visualization

// must be less than 10000 due to ADC
int sensitivity = 9;
double vReal[audioSamples];
double vImage[audioSamples];
const double samplingFrequency = 8000;                 // Hz
double samplingDelay = (1000000 / samplingFrequency);  // microseconds
ArduinoFFT<double> FFT =
    ArduinoFFT<double>(vReal, vImage, audioSamples, samplingFrequency);

void initializeSpectralAnalyzer() {
  pinMode(AUDIO_PIN, INPUT);       // Set the audio pin as input
  analogReadResolution(12);        // 12-bit ADC resolution
  analogSetAttenuation(ADC_11db);  // Use appropriate attenuation (0-3.3V)
  Serial.println("Spectral Analyzer Initialized");
}

void peakDetection(int* peakData, int maxWidth, int maxHeight) {
  // Divide FFT bins across the LED matrix width
  int avgRange = (usableSamples) / maxWidth;
  for (int i = 0; i < maxWidth; i++) {
    double peak = 0;
    int startFreqBin = (i * avgRange) + 2;  // Skip FFT bins
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
  if (spectralData == nullptr) {
    spectralData = new int[maxWidth];
  }

  for (int i = 0; i < audioSamples; i++) {
    // read audio input and scale it to the sensitivity
    vReal[i] = analogRead(AUDIO_PIN) * (sensitivity / 10.0);
    vImage[i] = 0;
    delayMicroseconds(samplingDelay);
  }

  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward); /* Weigh data */
  FFT.compute(FFTDirection::Forward);                       /* Compute FFT */
  FFT.complexToMagnitude(); /* Compute magnitudes */

  peakDetection(spectralData, maxWidth, maxHeight);
}

#endif  // SPECTRAL_ANALYZER_H
