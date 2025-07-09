import React, { useState } from "react";
import BrightnessSlider from "../sliders/BrightnessSlider";
import FramerateSlider from "../sliders/FramerateSlider";
import ColorSelector from "../selectors/ColorSelector";
import { LBSettings } from "./LBSettings";
import LBDropdown from "../selectors/LBDropdown";

function TemperatureSettings({ config, updateConfig, visualizationDropdown }) {
  var [temperatureUnit, setTemperatureUnit] = useState(config.temperatureUnit);

  return (
    <LBSettings
      label="Temperature Settings"
      config={config}
      updateConfig={updateConfig}
      visualization="temperature"
    >
      {visualizationDropdown}
      <BrightnessSlider config={config} updateConfig={updateConfig} />
      <FramerateSlider config={config} updateConfig={updateConfig} />
      <ColorSelector config={config} updateConfig={updateConfig} />
      <LBDropdown
        label="Temperature Unit"
        value={temperatureUnit}
        defaultOpen="true"
        onChange={(e) => {
          setTemperatureUnit(e.target.value);
          updateConfig({ ...config, temperatureUnit: e.target.value });
        }}
        options={[
          { value: "C", label: "Celsius" },
          { value: "F", label: "Fahrenheit" },
        ]}
      />
    </LBSettings>
  );
}

export default TemperatureSettings;
