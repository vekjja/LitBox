import React, { useState, useEffect } from 'react';
import WiFiSettings from './settings/WiFiSettings';
import BarsSettings from './settings/BarsSettings';
import GameOfLifeSettings from './settings/GameOfLifeSettings';
import MatrixSettings from './settings/MatrixSettings';
import WaveformSetting from './settings/WaveformSettings';
import BirdsSetting from './settings/BirdsSettings';
import CirclesSetting from './settings/CirclesSetting';
import StarPulseSetting from './settings/StarPulseSettings';
import TemperatureSetting from './settings/TemperatureSettings';
import MotionSettings from './settings/MotionSettings';
import TextSettings from './settings/TextSettings';

var defaultConfig = {
  "version": "1.0.0",
  "mode": "client",
  "mdns": "litbox",
  "client": {
    "ssid": "connectedness",
    "password": "ReallyLongPassword123!@#"
  },
  "ap": {
    "ssid": "LitBox",
    "password": "abcd1234"
  },
  "brightness": 9,
  "sensitivity": 9,
  "visualization": "bars",
  "frameRate": 30,
  "temperatureUnit": "C",
  "colorPallet": [31, 2047, 63514, 65535],
  "pixelColor": 65535,
  "pixelBgColor": 0,
  "text": {
    "content": "*.*. Lit Box .*.*",
    "animation": "scroll",
    "speed": "75",
    "size": "1"
  }
};

function App() {
  var [config, setConfig] = useState(null);
  const [selectedSetting, setSelectedSetting] = useState('bars');  // Default to 'about'

  useEffect(() => {
    if (process.env.NODE_ENV === 'development') {
      setConfig(defaultConfig);
      setSelectedSetting(defaultConfig.visualization);
    } else {
      fetch('/config')
        .then((response) => response.json())
        .then((data) => {
          setConfig(data);
          setSelectedSetting(data.visualization);
        })
        .catch((error) => console.error('Error loading configuration:', error));
    }
  }, []);

  const saveConfig = (newConfig) => {
    fetch('/saveConfig')
      .then(response => {
        if (!response.ok) throw new Error('Failed to save configuration');
        return response.json();
      }).then(data => {
        setConfig(newConfig);
        console.log('Configuration updated:', data);
        alert('Configuration Saved');
      }).catch(error => console.error('Error updating configuration:', error));
  };

  const updateConfig = (newConfig) => {
    fetch('/config', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(newConfig),
    }).then(response => {
      if (!response.ok) throw new Error('Failed to update configuration');
      return response.json();
    }).then(data => {
      setConfig(newConfig);
      console.log('Configuration updated:', data);
      // alert('Configuration updated');
    }).catch(error => console.error('Error updating configuration:', error));
  };

  if (!config) {
    return (
      <div className="container">
        <label className="header">Lit Box</label>
        <div>Loading configuration... ⚙️</div>
      </div>
    );
  }

  const renderSetting = () => {
    switch (selectedSetting) {
      case 'bars':
        return <BarsSettings config={config} updateConfig={updateConfig} />;
      case 'birds':
        return <BirdsSetting config={config} updateConfig={updateConfig} />;
      case 'circles':
        return <CirclesSetting config={config} updateConfig={updateConfig} />;
      case 'gameOfLife':
        return <GameOfLifeSettings config={config} updateConfig={updateConfig} />;
      case 'matrix':
        return <MatrixSettings config={config} updateConfig={updateConfig} />;
      case 'motion':
        return <MotionSettings config={config} updateConfig={updateConfig} />;
      case 'starPulse':
        return <StarPulseSetting config={config} updateConfig={updateConfig} />;
      case 'text':
        return <TextSettings config={config} updateConfig={updateConfig} />;
      case 'temperature':
        return <TemperatureSetting config={config} updateConfig={updateConfig} />;
      case 'waveform':
        return <WaveformSetting config={config} updateConfig={updateConfig} />;
      case 'wifi':
        return <WiFiSettings config={config} saveConfig={saveConfig} />;
      case 'about':
        return <div class="setting" id="about-settings">

          <div class="setting">
            <label>Version</label>
            <label id="version">{config.version}</label>
          </div>

          <div class="setting">
            <label>Designed and Developed by:</label>
            <a href="https://github.com/seemywingz/LitBox"
              target="_blank" id="SeeMyWingZ" rel="noreferrer">SeeMyWingZ</a>
          </div>

        </div>;
      default:
        return <div>🛠️ Under Construction 🛠️</div>;
    }
  };

  return (
    <div className="container">
      <label className="header">Lit Box</label>
      <div className="setting">
        <select
          id="settingSelect"
          className="clickable"
          value={selectedSetting}
          onChange={(e) => setSelectedSetting(e.target.value)}
        >
          <option value="bars">Bars</option>
          <option value="birds">Birds</option>
          <option value="circles">Circles</option>
          <option value="gameOfLife">Game of Life</option>
          <option value="matrix">Matrix</option>
          <option value="motion">Motion</option>
          <option value="starPulse">Star Pulse</option>
          <option value="text">Text</option>
          <option value="temperature">Temperature</option>
          <option value="waveform">Waveform</option>
          <option value="wifi">System</option>
          <option value="about">About</option>
        </select>
      </div>
      {renderSetting()}
    </div>
  );
}

export default App;