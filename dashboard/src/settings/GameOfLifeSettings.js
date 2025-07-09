import React from "react";
import BrightnessSlider from "../sliders/BrightnessSlider";
import FramerateSlider from "../sliders/FramerateSlider";
import ColorSelector from "../selectors/ColorSelector";
import { LBSettings } from "./LBSettings";

function GameOfLifeSettings({ config, updateConfig }) {
  return (
    <LBSettings
      label="Game of Life Settings"
      config={config}
      updateConfig={updateConfig}
      visualization="gameOfLife"
    >
      <BrightnessSlider config={config} updateConfig={updateConfig} />
      <FramerateSlider config={config} updateConfig={updateConfig} />
      <ColorSelector config={config} updateConfig={updateConfig} />
    </LBSettings>
  );
}

export default GameOfLifeSettings;
