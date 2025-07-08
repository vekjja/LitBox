```

                                                                 
                                                                 
`7MMF'      `7MMF'MMP""MM""YMM     `7MM"""Yp,                    
  MM          MM  P'   MM   `7       MM    Yb                    
  MM          MM       MM            MM    dP  ,pW"Wq.`7M'   `MF'
  MM          MM       MM            MM"""bg. 6W'   `Wb `VA ,V'  
  MM      ,   MM       MM            MM    `Y 8M     M8   XMX    
  MM     ,M   MM       MM            MM    ,9 YA.   ,A9 ,V' VA.  
.JMMmmmmMMM .JMML.   .JMML.        .JMMmmmd9   `Ybmd9'.AM.   .MA.
                                                                 
                                                                 

```
------

# LitBox: LED Matrix Visualizer & IoT Platform

## Overview

**LitBox** is a modular, open-source platform for driving LED matrix displays with real-time visualizations, web-based configuration, and IoT features. It is built on top of the [ESPWiFi](https://github.com/vekjja/ESPWiFi) library, which provides robust WiFi, web server, and configuration management for ESP8266 and ESP32 boards. LitBox also includes a React-based dashboard for easy control and customization from your browser.

LitBox is designed for:
- Audio-reactive LED visualizations (bars, circles, waveform, etc.)
- Motion, temperature, and sensor-based effects
- Web-based configuration and control (via ESPWiFi)
- Easy extensibility for new visualizations and hardware

## Hardware Requirements

- **Supported Boards:**
  - ESP8266 (e.g., WeMos D1 Mini)
  - ESP32 (e.g., Seeed XIAO ESP32-C3/S3, ESP32-CAM)
- **LED Matrix Display:** 32x8 WS2812B (or compatible, connected to `LED Pin D6` or board default)
- **Microphone:** Analog, connected to `MIC Pin A0`
- **I2C Devices:** (Optional) BMI160 motion sensor, SCL to `D1`, SDA to `D2`
- **Other Sensors:** (Optional) Camera (ESP32-CAM), temperature, etc.

## Software Requirements

- **Development Environment:**
  - [VSCode](https://code.visualstudio.com/) with [PlatformIO](https://platformio.org/)
- **Firmware Libraries:** (see `platformio.ini` for exact versions)
  - [FastLED](https://github.com/FastLED/FastLED)
  - [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
  - [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
  - [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
  - [Adafruit BusIO](https://github.com/adafruit/Adafruit_BusIO)
  - [arduinoFFT](https://github.com/kosme/arduinoFFT)
  - [ESPWiFi](https://github.com/vekjja/ESPWiFi) (included as a submodule/library)
- **Dashboard:**
  - React (see `dashboard/` for details)

## Features

- **Multiple Visualizations:** Bars, circles, waveform, birds, Game of Life, star pulse, matrix, motion, temperature, and text.
- **Web Server Control:** Configure brightness, color, sensitivity, frame rate, and visualization mode via browser (powered by ESPWiFi).
- **Motion Detection:** BMI160 sensor support for motion-reactive effects.
- **Spectral Analyzer:** Real-time audio visualization using FFT.
- **Game of Life:** Conway's Game of Life simulation.
- **Temperature Sensing:** Display temperature in Celsius or Fahrenheit.
- **Camera Support:** (ESP32-CAM) Live streaming and snapshots.
- **Modular Config:** All settings stored in `config.json` (see below).
- **Dashboard:** Modern web UI for configuration and control (see `dashboard/`).

## Project Structure

- `LitBox/` - Main firmware and source code for LED matrix and visualizations
- `ESPWiFi/` - WiFi, web server, config, and hardware abstraction library (used by LitBox)
- `dashboard/` - React web app for device configuration and control
- `examples/` - Example configuration files

## Setup and Installation

1. **Hardware Assembly:**
   - Connect all components as described above.
2. **Clone the Repository:**
   ```sh
   git clone --recurse-submodules <this-repo-url>
   cd LitBox
   ```
3. **Open in VSCode + PlatformIO:**
   - Open the project folder in VSCode.
   - Ensure PlatformIO extension is installed.
4. **Configure PlatformIO:**
   - Select your board in `platformio.ini` (`[env:esp8266]`, `[env:esp32-c3]`, etc.)
   - Dependencies are managed automatically via `platformio.ini`.
5. **Configure Device:**
   - Edit `examples/config.json` with your WiFi and preferences.
   - Copy to `data/config.json` if needed.
6. **Build and Upload Firmware:**
   - Use PlatformIO tasks or run:
     ```sh
     pio run --target upload
     pio run --target uploadfs  # Upload config and dashboard files
     ```
7. **Access the Web Dashboard:**
   - The device hosts a web server (default: `http://<device-ip>/`).
   - For advanced control, use the React dashboard in `dashboard/` (see its README for setup).

## Example Configuration (`config.json`)

```json
{
    "version": "v0.0.0",
    "mode": "client",
    "mdns": "LitBox",
    "client": {
        "ssid": "YOUR_SSID",
        "password": "YOUR_PASSWORD"
    },
    "ap": {
        "ssid": "LitBox",
        "password": "abcd1234"
    },
    "colorPallet": ["#1d54ff", "#532bff", "#fb62ff", "#ff3691"],
    "visualization": "bars",
    "temperatureUnit": "C",
    "pixelColor": "#ff3691",
    "pixelBgColor": "#000000",
    "sensitivity": 25,
    "brightness": 3,
    "frameRate": 60,
    "text": {
        "content": "*.*. Lit Box .*.*",
        "animation": "scroll",
        "speed": "75",
        "size": "1"
    }
}
```

- See [ESPWiFi config example](../ESPWiFi/examples/config.json) for advanced options (modules, power, etc).

## Usage

- Once powered on, the LED matrix displays the default visualization.
- Access the device's web server to change settings, visualizations, and more.
- Use the dashboard for a modern UI experience (see `dashboard/README.md`).
- All settings are saved to `config.json` and persist across reboots.

## ESPWiFi Integration

LitBox uses [ESPWiFi](https://github.com/vekjja/ESPWiFi) for:
- WiFi management (client/AP modes, mDNS, config)
- Web server and REST API
- File system (LittleFS)
- Power management
- Sensor and peripheral abstraction (I2C, GPIO, camera, etc.)

For advanced networking, configuration, or hardware features, see the [ESPWiFi README](../ESPWiFi/.github/README.md).

## Dashboard

- The `dashboard/` directory contains a React app for device configuration and control.
- See `dashboard/README.md` for setup and usage instructions.

## Contributing

Contributions are welcome! Improve the code, add features, fix bugs, or enhance documentation.
- For firmware: open issues/PRs in this repo.
- For ESPWiFi: see [ESPWiFi repo](https://github.com/vekjja/ESPWiFi).
- For dashboard: see `dashboard/` subdirectory.

## License

This project is open-source and licensed under the [MIT License](https://opensource.org/licenses/MIT).

---

**Note:** For detailed implementation and customization, refer to inline comments in the source code and the ESPWiFi documentation.
