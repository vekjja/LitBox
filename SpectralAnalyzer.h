#ifndef SPECTRAL_ANALYZER_H
#define SPECTRAL_ANALYZER_H

#include <IOPin.h>
#include <arduinoFFT.h>

// Audio Config
IOPin audio(A0, INPUT);
arduinoFFT FFT = arduinoFFT();
const int maxInput = 81;
const int minSensitivity = 1;
const int maxSensitivity = 100;
const uint16_t audioSamples = 128;
const int usableSamples = (audioSamples / 2);
int* spectralData = nullptr;
bool scaling = true;
double vReal[audioSamples];
double vImage[audioSamples];
int sensitivity = 6;

void logarithmicScaling(int* spectralData, int maxWidth, int maxHeight) {
  for (int i = 0; i < maxWidth; i++) {
    if (spectralData[i] > 0) {
      // Apply logarithmic scaling
      spectralData[i] = log10(spectralData[i]) * (maxHeight / log10(maxInput));
    }
  }
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

  if (scaling) logarithmicScaling(peakData, maxWidth, maxHeight);
}

void spectralAnalyzer(int maxWidth, int maxHeight) {
  for (int i = 0; i < audioSamples; i++) {
    vReal[i] = audio.readA() / sensitivity;
    vImage[i] = 0;
  }

  FFT.Windowing(vReal, audioSamples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImage, audioSamples, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImage, audioSamples);

  if (spectralData == nullptr) {
    spectralData = new int[maxWidth];
  }
  peakDetection(spectralData, maxWidth, maxHeight);
}
#endif  // SPECTRAL_ANALYZER_H