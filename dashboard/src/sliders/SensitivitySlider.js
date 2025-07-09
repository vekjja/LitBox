import React, { useState } from "react";
import LBSlider from "./LBSlider";

function SensitivitySlider({ config, updateConfig }) {
  const [sensitivity, setSensitivity] = useState(config.sensitivity);

  const handleSliderRelease = () => {
    const newConfig = { ...config, sensitivity: parseInt(sensitivity) };
    updateConfig(newConfig);
  };

  return (
    <LBSlider
      label="Sensitivity"
      min={1}
      max={100}
      value={Number(sensitivity)}
      onChange={(e, value) => setSensitivity(value)}
      onChangeCommitted={handleSliderRelease}
    />
  );
}

export default SensitivitySlider;
