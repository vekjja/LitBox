import React, { useState } from "react";
import Box from "@mui/material/Box";
import LBColorSelector from "./LBColorSelector";

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
    <Box>
      <LBColorSelector
        label="Pixel Color"
        value={pixelColor}
        onChange={handlePixelColorChange}
        id="pixelColor"
      />
      <LBColorSelector
        label="Background Color"
        value={pixelBgColor}
        onChange={handleBgColorChange}
        id="pixelBgColor"
      />
    </Box>
  );
}

export default ColorSelector;
