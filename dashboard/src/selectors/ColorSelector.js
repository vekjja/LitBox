import React, { useState } from "react";

// Utility: Convert a number to a hex color string (e.g., 16711680 -> "#ff0000")
function toHexColor(val) {
  if (typeof val === "string" && val.startsWith("#") && val.length === 7)
    return val;
  let num = typeof val === "number" ? val : parseInt(val, 10);
  if (isNaN(num)) return "#000000";
  let hex = num.toString(16).padStart(6, "0");
  return `#${hex}`;
}

function ColorSelector({ config, updateConfig }) {
  const [pixelColor, setPixelColor] = useState(toHexColor(config.pixelColor));
  const [pixelBgColor, setPixelBgColor] = useState(
    toHexColor(config.pixelBgColor)
  );

  const handlePixelColorChange = (event) => {
    const newPixelColor = toHexColor(event.target.value);
    setPixelColor(newPixelColor);
    updateConfig({
      ...config,
      pixelColor: newPixelColor,
    });
  };

  const handleBgColorChange = (event) => {
    const newBgColor = toHexColor(event.target.value);
    setPixelBgColor(newBgColor);
    updateConfig({
      ...config,
      pixelBgColor: newBgColor,
    });
  };

  return (
    <div className="setting" id="color-settings">
      <input id="toggleColor" type="checkbox" className="toggle-button"></input>
      <label htmlFor="toggleColor" className="clickable">
        Color
      </label>
      <div className="toggle-content">
        <div className="setting">
          <input
            type="color"
            id="pixelColor"
            title="Choose A Color"
            className="color-picker"
            value={pixelColor}
            onChange={handlePixelColorChange}
          ></input>
        </div>
        <div className="setting">
          <input
            type="color"
            id="pixelBgColor"
            title="Choose A Background Color"
            className="color-picker"
            value={pixelBgColor}
            onChange={handleBgColorChange}
          ></input>
        </div>
      </div>
    </div>
  );
}

export default ColorSelector;
