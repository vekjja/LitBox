import React, { useState } from "react";
import LBSlider from "./LBSlider";

function TextSpeedSlider({ config, updateConfig }) {
  const [sensitivity, setSensitivity] = useState(config.sensitivity);

  const handleSliderRelease = () => {
    const newConfig = { ...config, sensitivity: parseInt(sensitivity) };
    updateConfig(newConfig);
  };

  return (
    <LBSlider
      label="Text Speed"
      min={1}
      max={100}
      value={Number(sensitivity)}
      onChange={(e, value) => setSensitivity(value)}
      onChangeCommitted={handleSliderRelease}
    />
  );
}

export default TextSpeedSlider;
