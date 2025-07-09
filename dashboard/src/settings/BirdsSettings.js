import React from "react";
import BrightnessSlider from "../sliders/BrightnessSlider";
import FramerateSlider from "../sliders/FramerateSlider";
import ColorPalletSelector from "../selectors/ColorPalletSelector";
import { LBSettings } from "./LBSettings";

function BirdsSettings({ config, updateConfig, visualizationDropdown }) {
  return (
    <LBSettings
      label="Birds Settings"
      config={config}
      updateConfig={updateConfig}
      visualization="birds"
    >
      {visualizationDropdown}
      <BrightnessSlider config={config} updateConfig={updateConfig} />
      <FramerateSlider config={config} updateConfig={updateConfig} />
      <ColorPalletSelector config={config} updateConfig={updateConfig} />
    </LBSettings>
  );
}

export default BirdsSettings;
