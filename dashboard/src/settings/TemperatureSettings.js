import React, { useState } from "react";
import BrightnessSlider from "../sliders/BrightnessSlider";
import FramerateSlider from "../sliders/FramerateSlider";
import ColorSelector from "../selectors/ColorSelector";
import { LBSettings } from "./LBSettings";

function TemperatureSettings({ config, updateConfig }) {
  var [temperatureUnit, setTemperatureUnit] = useState(config.temperatureUnit);

  const handleChange = (e) => {
    setTemperatureUnit(e.target.value);
    updateConfig({ ...config, temperatureUnit: e.target.value });
  };

  return (
    <LBSettings
      label="Temperature Settings"
      config={config}
      updateConfig={updateConfig}
      visualization="temperature"
    >
      <BrightnessSlider config={config} updateConfig={updateConfig} />
      <FramerateSlider config={config} updateConfig={updateConfig} />
      <ColorSelector config={config} updateConfig={updateConfig} />
      <div className="setting">
        <select
          id="settingSelect"
          className="clickable"
          value={temperatureUnit}
          onChange={handleChange}
        >
          <option value="C">Celsius</option>
          <option value="F">Fahrenheit</option>
        </select>
      </div>
    </LBSettings>
  );
}

export default TemperatureSettings;
