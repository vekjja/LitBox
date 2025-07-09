import React from "react";
import BrightnessSlider from "../sliders/BrightnessSlider";
import SensitivitySlider from "../sliders/SensitivitySlider";
import ColorPalletSelector from "../selectors/ColorPalletSelector";
import { LBSettings } from "./LBSettings";

function WaveformSettings({ config, updateConfig }) {
  return (
    <LBSettings
      label="Waveform Settings"
      config={config}
      updateConfig={updateConfig}
      visualization="waveform"
    >
      <BrightnessSlider config={config} updateConfig={updateConfig} />
      <SensitivitySlider config={config} updateConfig={updateConfig} />
      <ColorPalletSelector config={config} updateConfig={updateConfig} />
    </LBSettings>
  );
}
export default WaveformSettings;
