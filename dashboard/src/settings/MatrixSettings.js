import React from "react";
import BrightnessSlider from "../sliders/BrightnessSlider";
import FramerateSlider from "../sliders/FramerateSlider";
import { LBSettings } from "./LBSettings";

function MatrixSettings({ config, updateConfig, visualizationDropdown }) {
  return (
    <LBSettings
      label="Matrix Settings"
      config={config}
      updateConfig={updateConfig}
      visualization="matrix"
    >
      {visualizationDropdown}
      <BrightnessSlider config={config} updateConfig={updateConfig} />
      <FramerateSlider config={config} updateConfig={updateConfig} />
    </LBSettings>
  );
}

export default MatrixSettings;
