import React, { useState } from "react";

function FramerateSlider({ config, updateConfig }) {
  const [frameRate, setFrameRate] = useState(config.frameRate);

  const handleSliderChange = (event) => {
    setFrameRate(event.target.value);
  };

  const handleSliderRelease = () => {
    const newConfig = { ...config, frameRate: parseInt(frameRate) };
    updateConfig(newConfig);
  };

  return (
    <div className="setting" id="framerate-settings">
      <input
        id="toggleFramerate"
        type="checkbox"
        className="toggle-button"
      ></input>
      <label htmlFor="toggleFramerate" className="clickable">
        Frame Rate
      </label>
      <div className="toggle-content">
        <div className="setting">
          <input
            id="framerateValue"
            type="range"
            min="1"
            max="255"
            value={frameRate}
            onChange={handleSliderChange}
            onMouseUp={handleSliderRelease}
            onTouchEnd={handleSliderRelease}
          />
        </div>
      </div>
    </div>
  );
}

export default FramerateSlider;
