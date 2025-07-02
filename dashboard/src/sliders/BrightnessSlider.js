import React, { useState } from "react";

function BrightnessSlider({ config, updateConfig }) {
  const [brightness, setBrightness] = useState(config.brightness);

  const handleSliderChange = (event) => {
    setBrightness(event.target.value);
  };

  const handleSliderRelease = () => {
    const newConfig = { ...config, brightness: parseInt(brightness) };
    updateConfig(newConfig);
  };

  return (
    <div className="setting" id="brightness-settings">
      <input
        id="toggleBrightness"
        type="checkbox"
        className="toggle-button"
      ></input>
      <label htmlFor="toggleBrightness" className="clickable">
        Brightness
      </label>
      <div className="toggle-content">
        <div className="setting">
          <input
            id="brightnessValue"
            type="range"
            min="1"
            max="255"
            value={brightness}
            onChange={handleSliderChange}
            onMouseUp={handleSliderRelease}
            onTouchEnd={handleSliderRelease}
          />
        </div>
      </div>
    </div>
  );
}

export default BrightnessSlider;
