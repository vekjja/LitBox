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
      <LBSettingItem label="Color Pallet" defaultOpen={true}>
        <Box sx={{ flexDirection: "column", gap: 2 }}>
          {[...colors].reverse().map((color, index) => (
            <LBColorSelector
              key={index}
              label={`Color ${colors.length - index}`}
              value={color}
              onChange={handleColorChange(colors.length - index - 1)}
              id={`color${colors.length - index}`}
            />
          ))}
        </Box>
      </LBSettingItem>
    </Box>
  );
}

export default ColorPalletSelector;
