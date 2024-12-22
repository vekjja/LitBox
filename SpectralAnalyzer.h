#ifndef SPECTRAL_ANALYZER_H
#define SPECTRAL_ANALYZER_H

#include <arduinoFFT.h>

#include "IOPin.h"

// Audio Config
IOPin audioPin(9, INPUT);
const int maxInput = 1023;
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
    // Scale the audio input according to sensitivity
    vReal[i] = audioPin.readA() * (sensitivity / 10.0);
    vImage[i] = 0;
    Serial.println(vReal[i]);
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