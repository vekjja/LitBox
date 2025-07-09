import Box from "@mui/material/Box";
import React, { useState } from "react";
import LBColorSelector from "./LBColorSelector";
import LBSettingItem from "../settings/LBSettingItem";
import { toHexColor } from "../utils/colorUtils";

function ColorSelector({ config, updateConfig }) {
  const [pixelColor, setPixelColor] = useState(toHexColor(config.pixelColor));
  const [pixelBgColor, setPixelBgColor] = useState(config.pixelBgColor);

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
      <LBSettingItem label="Colors">
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
      </LBSettingItem>
    </Box>
  );
}

export default ColorSelector;
