#ifndef SPECTRAL_ANALYZER_H
#define SPECTRAL_ANALYZER_H

#include <arduinoFFT.h>

// Audio Device Configuration
#define AUDIO_PIN A0
const int maxInput = 4095;          // 12-bit ADC on ESP32-S3
const uint16_t audioSamples = 512;  // This value MUST be a power of 2
const int usableSamples = (audioSamples / 2);
double vReal[audioSamples];
double vImaginary[audioSamples];
int* spectralData = nullptr;  // Array to store spectral data for matrix
const double samplingFrequency = 16000.0;

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
    spectralData = new int[matrixWidth];  // Allocate for matrix width
  }

  // Read audio samples
  for (int i = 0; i < audioSamples; i++) {
    vReal[i] = analogRead(AUDIO_PIN) * (sensitivity / 10.0);  // Read ADC value
    vImaginary[i] = 0;  // Initialize imaginary part
    // delayMicroseconds(1000000 / samplingFrequency);
  }

  // Perform FFT
  FFT.windowing(FFTWindow::Blackman_Harris,
                FFTDirection::Forward);  // Apply window
  FFT.compute(FFTDirection::Forward);    // Compute FFT
  FFT.complexToMagnitude();              // Get magnitude spectrum

  // Map FFT bins directly to LED matrix columns
  int binsPerColumn = usableSamples / matrixWidth;  // Divide bins among columns
  for (int x = 0; x < matrixWidth; x++) {
    double peak = 0;

    // Aggregate bins for this column, skip bin 0
    int startBin = x * binsPerColumn + 4;  // Skip bins
    int endBin = startBin + binsPerColumn;

    for (int bin = startBin; bin < endBin && bin < usableSamples; bin++) {
      peak = max(peak, vReal[bin]);
    }

    // Map peak value to matrix height
    spectralData[x] = map(peak, 0, maxInput, 0, matrixHeight);
    spectralData[x] =
        constrain(spectralData[x], 0, matrixHeight);  // Clamp to height
  }
}

#endif  // SPECTRAL_ANALYZER_H
