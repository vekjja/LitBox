import React, { useState } from "react";
import Box from "@mui/material/Box";
import LBSettingItem from "../settings/LBSettingItem";
import LBColorSelector from "./LBColorSelector";
import { toHexColor } from "../utils/colorUtils";

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
      <LBSettingItem label="Color Pallet">
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
