import React, { useState } from "react";
import LBSlider from "./LBSlider";

function BrightnessSlider({ config, updateConfig }) {
  const [brightness, setBrightness] = useState(config.brightness);

  const handleSliderRelease = () => {
    const newConfig = { ...config, brightness: parseInt(brightness) };
    updateConfig(newConfig);
  };

  return (
    <LBSlider
      label="Brightness"
      min={3}
      max={255}
      value={Number(brightness)}
      onChange={(e, value) => setBrightness(value)}
      onChangeCommitted={handleSliderRelease}
    />
  );
}

export default BrightnessSlider;
