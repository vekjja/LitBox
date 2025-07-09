import React from "react";
import BrightnessSlider from "../sliders/BrightnessSlider";
import FramerateSlider from "../sliders/FramerateSlider";
import { LBSettings } from "./LBSettings";

function MatrixSettings({ config, updateConfig }) {
  return (
    <LBSettings
      label="Matrix Settings"
      config={config}
      updateConfig={updateConfig}
      visualization="matrix"
    >
      <BrightnessSlider config={config} updateConfig={updateConfig} />
      <FramerateSlider config={config} updateConfig={updateConfig} />
    </LBSettings>
  );
}

export default MatrixSettings;
