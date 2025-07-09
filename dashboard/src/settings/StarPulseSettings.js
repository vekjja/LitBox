import React from "react";
import BrightnessSlider from "../sliders/BrightnessSlider";
import SensitivitySlider from "../sliders/SensitivitySlider";
import ColorPalletSelector from "../selectors/ColorPalletSelector";
import { LBSettings } from "./LBSettings";

function StarPulseSettings({ config, updateConfig, visualizationDropdown }) {
  return (
    <LBSettings
      label="Star Pulse Settings"
      config={config}
      updateConfig={updateConfig}
      visualization="starPulse"
    >
      {visualizationDropdown}
      <BrightnessSlider config={config} updateConfig={updateConfig} />
      <SensitivitySlider config={config} updateConfig={updateConfig} />
      <ColorPalletSelector config={config} updateConfig={updateConfig} />
    </LBSettings>
  );
}
export default StarPulseSettings;
