import React, { useState, useEffect } from "react";
import SystemSettings from "./settings/SystemSettings";
import BarsSettings from "./settings/BarsSettings";
import GameOfLifeSettings from "./settings/GameOfLifeSettings";
import MatrixSettings from "./settings/MatrixSettings";
import WaveformSetting from "./settings/WaveformSettings";
import BirdsSetting from "./settings/BirdsSettings";
import CirclesSetting from "./settings/CirclesSetting";
import StarPulseSetting from "./settings/StarPulseSettings";
import TemperatureSetting from "./settings/TemperatureSettings";
import MotionSettings from "./settings/MotionSettings";
import TextSettings from "./settings/TextSettings";

function App() {
  const hostname = process.env.REACT_APP_API_HOST || "localhost";
  const port = process.env.REACT_APP_API_PORT || 80;
  const apiURL =
    process.env.NODE_ENV === "production" ? "" : `http://${hostname}:${port}`;

  var [config, setConfig] = useState(null);
  const [selectedSetting, setSelectedSetting] = useState("bars"); // Default to 'about'

  useEffect(() => {
    fetch(apiURL + "/config")
      .then((response) => response.json())
      .then((data) => {
        console.log("Configuration loaded:", data);
        const configWithAPI = { ...data, apiURL };
        setConfig(configWithAPI);
        setSelectedSetting(data.visualization);
      })
      .catch((error) => console.error("Error loading configuration:", error));
  }, []);

  const saveConfig = (newConfig) => {
    fetch(apiURL + "/config", {
      method: "PUT",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(newConfig),
    })
      .then((response) => {
        if (!response.ok) throw new Error("Failed to save configuration");
        return response.json();
      })
      .then((data) => {
        setConfig(newConfig);
        console.log("Configuration Saved:", data);
        alert("Configuration Saved");
      })
      .catch((error) => console.error("Error updating configuration:", error));
  };

  const updateConfig = (newConfig) => {
    fetch(apiURL + "/config", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(newConfig),
    })
      .then((response) => {
        if (!response.ok) throw new Error("Failed to update configuration");
        return response.json();
      })
      .then((data) => {
        setConfig(newConfig);
        console.log("Configuration Updated:", data);
      })
      .catch((error) => console.error("Error updating configuration:", error));
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
      case "bars":
        return <BarsSettings config={config} updateConfig={updateConfig} />;
      case "birds":
        return <BirdsSetting config={config} updateConfig={updateConfig} />;
      case "circles":
        return <CirclesSetting config={config} updateConfig={updateConfig} />;
      case "gameOfLife":
        return (
          <GameOfLifeSettings config={config} updateConfig={updateConfig} />
        );
      case "matrix":
        return <MatrixSettings config={config} updateConfig={updateConfig} />;
      case "motion":
        return <MotionSettings config={config} updateConfig={updateConfig} />;
      case "starPulse":
        return <StarPulseSetting config={config} updateConfig={updateConfig} />;
      case "text":
        return <TextSettings config={config} updateConfig={updateConfig} />;
      case "temperature":
        return (
          <TemperatureSetting config={config} updateConfig={updateConfig} />
        );
      case "waveform":
        return <WaveformSetting config={config} updateConfig={updateConfig} />;
      case "wifi":
        return (
          <SystemSettings
            config={config}
            updateConfig={updateConfig}
            saveConfig={saveConfig}
          />
        );
      case "about":
        return (
          <div className="setting" id="about-settings">
            <div className="setting">
              <label htmlFor="version">Version</label>
              <label id="version">{config.version}</label>
            </div>

            <div className="setting">
              <label>Designed and Developed by:</label>
              <a
                href="https://github.com/seemywingz/LitBox"
                target="_blank"
                id="SeeMyWingZ"
                rel="noreferrer"
              >
                SeeMyWingZ
              </a>
            </div>
          </div>
        );
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
