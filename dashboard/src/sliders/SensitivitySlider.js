import React, { useState } from "react";

function SensitivitySlider({ config, updateConfig }) {
  const [sensitivity, setSensitivity] = useState(config.sensitivity);

  const handleSliderChange = (event) => {
    setSensitivity(event.target.value);
  };

  const handleSliderRelease = () => {
    const newConfig = { ...config, sensitivity: parseInt(sensitivity) };
    updateConfig(newConfig);
  };

  return (
    <div className="setting" id="sensitivity-settings">
      <input
        id="toggleSensitivity"
        type="checkbox"
        className="toggle-button"
      ></input>
      <label htmlFor="toggleSensitivity" className="clickable">
        Sensitivity
      </label>
      <div className="toggle-content">
        <div className="setting">
          <input
            id="sensitivityValue"
            type="range"
            min="1"
            max="100"
            value={sensitivity}
            onChange={handleSliderChange}
            onMouseUp={handleSliderRelease}
            onTouchEnd={handleSliderRelease}
          />
        </div>
      </div>
    </div>
  );
}

export default SensitivitySlider;
