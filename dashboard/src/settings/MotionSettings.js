import React from "react";
import BrightnessSlider from "../sliders/BrightnessSlider";
import ColorPalletSelector from "../selectors/ColorPalletSelector";
import { LBSettings } from "./LBSettings";

function MotionSettings({ config, updateConfig, visualizationDropdown }) {
  return (
    <LBSettings
      label="Motion Settings"
      config={config}
      updateConfig={updateConfig}
      visualization="motion"
    >
      {visualizationDropdown}
      <BrightnessSlider config={config} updateConfig={updateConfig} />
      <ColorPalletSelector config={config} updateConfig={updateConfig} />
    </LBSettings>
  );
}

export default MotionSettings;
