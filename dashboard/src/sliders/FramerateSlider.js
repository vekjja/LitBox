import React, { useState } from "react";
import LBSlider from "./LBSlider";

function FramerateSlider({ config, updateConfig }) {
  const [frameRate, setFrameRate] = useState(config.frameRate);

  const handleSliderRelease = () => {
    const newConfig = { ...config, frameRate: parseInt(frameRate) };
    updateConfig(newConfig);
  };

  return (
    <LBSlider
      label="Frame Rate"
      min={1}
      max={255}
      value={Number(frameRate)}
      onChange={(e, value) => setFrameRate(value)}
      onChangeCommitted={handleSliderRelease}
    />
  );
}

export default FramerateSlider;
