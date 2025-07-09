import React from "react";
import BrightnessSlider from "../sliders/BrightnessSlider";
import SensitivitySlider from "../sliders/SensitivitySlider";
import ColorPalletSelector from "../selectors/ColorPalletSelector";
import { LBSettings } from "./LBSettings";

function BarsSettings({ config, updateConfig }) {
  return (
    <LBSettings
      label="Bars Settings"
      config={config}
      updateConfig={updateConfig}
      visualization="bars"
    >
      <BrightnessSlider config={config} updateConfig={updateConfig} />
      <SensitivitySlider config={config} updateConfig={updateConfig} />
      <ColorPalletSelector config={config} updateConfig={updateConfig} />
    </LBSettings>
  );
}

export default BarsSettings;
