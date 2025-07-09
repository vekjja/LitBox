import React, { useState } from "react";
import Box from "@mui/material/Box";
import LBSettingItem from "../settings/LBSettingItem";
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
    <Box>
      <LBSettingItem label="Color Pallet" defaultOpen={true}>
        <Box sx={{ display: "flex", flexDirection: "column", gap: 2 }}>
          {[...colors].reverse().map((color, index) => (
            <Box key={index} sx={{ mb: 1 }}>
              <LBColorSelector
                label={`Color ${colors.length - index}`}
                value={color}
                onChange={handleColorChange(colors.length - index - 1)}
                id={`color${colors.length - index}`}
              />
            </Box>
          ))}
        </Box>
      </LBSettingItem>
    </Box>
  );
}

export default ColorPalletSelector;
