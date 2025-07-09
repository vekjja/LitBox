import React from "react";
import BrightnessSlider from "../sliders/BrightnessSlider";
import FramerateSlider from "../sliders/FramerateSlider";
import ColorSelector from "../selectors/ColorSelector";
import { LBSettings } from "./LBSettings";

function GameOfLifeSettings({ config, updateConfig, visualizationDropdown }) {
  return (
    <LBSettings
      label="Game of Life Settings"
      config={config}
      updateConfig={updateConfig}
      visualization="gameOfLife"
    >
      {visualizationDropdown}
      <BrightnessSlider config={config} updateConfig={updateConfig} />
      <FramerateSlider config={config} updateConfig={updateConfig} />
      <ColorSelector config={config} updateConfig={updateConfig} />
    </LBSettings>
  );
}

export default GameOfLifeSettings;
