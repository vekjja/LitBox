```

                                                                                                                     
                                                                                                                     
LLLLLLLLLLL             IIIIIIIIIITTTTTTTTTTTTTTTTTTTTTTT     BBBBBBBBBBBBBBBBB                                      
L:::::::::L             I::::::::IT:::::::::::::::::::::T     B::::::::::::::::B                                     
L:::::::::L             I::::::::IT:::::::::::::::::::::T     B::::::BBBBBB:::::B                                    
LL:::::::LL             II::::::IIT:::::TT:::::::TT:::::T     BB:::::B     B:::::B                                   
  L:::::L                 I::::I  TTTTTT  T:::::T  TTTTTT       B::::B     B:::::B   ooooooooooo xxxxxxx      xxxxxxx
  L:::::L                 I::::I          T:::::T               B::::B     B:::::B oo:::::::::::oox:::::x    x:::::x 
  L:::::L                 I::::I          T:::::T               B::::BBBBBB:::::B o:::::::::::::::ox:::::x  x:::::x  
  L:::::L                 I::::I          T:::::T               B:::::::::::::BB  o:::::ooooo:::::o x:::::xx:::::x   
  L:::::L                 I::::I          T:::::T               B::::BBBBBB:::::B o::::o     o::::o  x::::::::::x    
  L:::::L                 I::::I          T:::::T               B::::B     B:::::Bo::::o     o::::o   x::::::::x     
  L:::::L                 I::::I          T:::::T               B::::B     B:::::Bo::::o     o::::o   x::::::::x     
  L:::::L         LLLLLL  I::::I          T:::::T               B::::B     B:::::Bo::::o     o::::o  x::::::::::x    
LL:::::::LLLLLLLLL:::::LII::::::II      TT:::::::TT           BB:::::BBBBBB::::::Bo:::::ooooo:::::o x:::::xx:::::x   
L::::::::::::::::::::::LI::::::::I      T:::::::::T           B:::::::::::::::::B o:::::::::::::::ox:::::x  x:::::x  
L::::::::::::::::::::::LI::::::::I      T:::::::::T           B::::::::::::::::B   oo:::::::::::oox:::::x    x:::::x 
LLLLLLLLLLLLLLLLLLLLLLLLIIIIIIIIII      TTTTTTTTTTT           BBBBBBBBBBBBBBBBB      ooooooooooo xxxxxxx      xxxxxxx
                                                                                                                     
                                                                                                                     
                                                                                                                     
                                                                                                                     
                                                                                                                     
                                                                                                                     
                                                                                                                     

```
------

## Overview


LitBox is an Arduino-based project for driving an LED matrix display. It features various visualizations including waveform, circles, motion detection, text scrolling, birds simulation, the Game of Life, and star pulses. The project is configurable through a web server interface, allowing users to adjust settings like brightness, color, and display mode.

## Hardware Requirements

1. **WeMos Board** with ESP8266 WiFi module.
2. **Microphone** connected to `MIC Pin A0`.
3. **LED Matrix Display**: 32x8 Pixels, connected via `LED Pin D6`.
4. **I2C Devices**: SCL connected to `Pin D1`, SDA connected to `Pin D2`.

## Software Requirements

- **Arduino IDE**: For compiling and uploading the firmware.
- **Adafruit NeoPixel Library**: For controlling the LED matrix.
- **Adafruit GFX Library**: For graphics functions.
- **ESPWiFi Library**: For handling WiFi and web server functionalities.
- **ArduinoFFT Library**: For Fourier Transform operations in spectral analysis.

## Features

- **Multiple Visualizations**: Waveform, circles, motion, text, birds, Game of Life, star pulse, and bars.
- **Web Server Control**: Configure settings like brightness, color, sensitivity, and display modes.
- **Motion Detection**: Using a BMI160 sensor, it can detect and visualize motion.
- **Spectral Analyzer**: Visualizes audio input in different formats.
- **Game of Life**: Simulates Conway's Game of Life on the LED matrix.
- **Temperature Sensing**: Displays temperature readings (Celsius or Fahrenheit).

## Setup and Installation

1. **Hardware Assembly**: Connect all hardware components as per the requirements.
2. **Software Setup**: Install the required libraries in the Arduino IDE.
3. **Firmware Upload**: Compile and upload the provided code to the WeMos board.
4. **Configuration**: Connect to the web server hosted by the WeMos board to configure the settings.

## Usage

Once powered on, the LED matrix will start displaying the default visualization. You can change the visualization and settings through the web server interface. The web server allows you to control the brightness, sensitivity, frame rate, and specific settings for each visualization mode.

## Contributing

Contributions to the project are welcome. You can contribute by improving the code, adding new features, fixing bugs, or improving this documentation.

## License

This project is open-source and is licensed under the [MIT License](https://opensource.org/licenses/MIT).

---

**Note**: This README is a brief overview of the project. For detailed implementation and customization, refer to the inline comments in the source code.
