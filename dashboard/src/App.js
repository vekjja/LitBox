import React, { useState, useEffect } from 'react';
import WiFiSettings from './WiFiSettings';
import APSettings from './APSettings';
import FileSettings from './FileSettings';
import BarsSettings from './BarsSettings';

var defaultConfig = {
  "client": {
    "ssid": "connectedness",
    "password": "ReallyLongPassword123!@#"
  },
  "ap": {
    "ssid": "LitBox-AP",
    "password": "abcd1234"
  },
  "mdns": "litbox",
  "mode": "client",
  "brightness": 18,
  "pixelBgColor": 0,
  "pixelColor": 65535,
  "colorPallet": [31, 2047, 63514, 65535],
};

function App() {
  var [config, setConfig] = useState(null);
  const [selectedSetting, setSelectedSetting] = useState('bars');  // Default to 'about'

  useEffect(() => {
    fetch('/config')
      .then(response => response.json())
      .then(data => setConfig(data))
      .catch(error => console.error('Error loading configuration:', error));
  }, []);

  const updateConfig = (newConfig) => {
    fetch('/config', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(newConfig),
    }).then(response => {
      if (!response.ok) throw new Error('Failed to save configuration');
      return response.json();
    }).then(data => {
      setConfig(newConfig);
      console.log('Configuration updated:', data);
      alert('Configuration updated');
    }).catch(error => console.error('Error updating configuration:', error));
  };

  // if (!config) return <div>Loading...</div>;

  if (!config) {
    config = defaultConfig;
  }

  const renderSetting = () => {
    switch (selectedSetting) {
      case 'bars':
        return <BarsSettings config={config} updateConfig={updateConfig} />;
      case 'wifi':
        return <WiFiSettings config={config} updateConfig={updateConfig} />;
      case 'ap':
        return <APSettings config={config} updateConfig={updateConfig} />;
      case 'files':
        return <FileSettings />;
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
        return <div>Not implemented</div>;
    }
  };

  return (
    <div className="container">
      <label className="header">Lit Box</label>
      <div className="setting">
        <select
          id="settingSelect"
          className="clickable"
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
          <option value="wifi">WiFi</option>
          <option value="ap">AP</option>
          <option value="files">Files</option>
          <option value="about">About</option>
        </select>
      </div>
      {renderSetting()}
    </div>
  );
}

export default App;