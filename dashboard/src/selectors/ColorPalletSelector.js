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

function ColorPalletSelector({ config, updateConfig }) {
  // Ensure all initial colors are hex strings
  const [colors, setColors] = useState(
    (config.colorPallet || []).map(toHexColor)
  );

  const handleColorChange = (index) => (event) => {
    const newColors = [...colors];
    newColors[index] = toHexColor(event.target.value);
    setColors(newColors);
    updateConfig({
      ...config,
      colorPallet: newColors,
    });
  };

  return (
    <div className="setting" id="color-pallet-settings">
      <input
        id="toggleColorPallet"
        type="checkbox"
        className="toggle-button"
      ></input>
      <label htmlFor="toggleColorPallet" className="clickable">
        Color Pallet
      </label>
      <div className="toggle-content">
        {[...colors].reverse().map((color, index) => (
          <div className="setting" key={index}>
            <input
              type="color"
              id={`color${colors.length - index}`}
              title={`Choose Color ${colors.length - index}`}
              className="color-picker"
              value={color}
              onChange={handleColorChange(colors.length - index - 1)}
            ></input>
          </div>
        ))}
      </div>
    </div>
  );
}

export default ColorPalletSelector;
