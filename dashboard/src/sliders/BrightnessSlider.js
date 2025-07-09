import React, { useState } from "react";
import LBSettingItem from "../settings/LBSettingItem";
import Box from "@mui/material/Box";
import Slider from "@mui/material/Slider";

function BrightnessSlider({ config, updateConfig }) {
  const [brightness, setBrightness] = useState(config.brightness);

  const handleSliderRelease = () => {
    const newConfig = { ...config, brightness: parseInt(brightness) };
    updateConfig(newConfig);
  };

  return (
    <Box>
      <LBSettingItem label="Brightness">
        <Slider
          id="brightnessValue"
          min={1}
          max={255}
          value={Number(brightness)}
          onChange={(e, value) => setBrightness(value)}
          onChangeCommitted={() => handleSliderRelease()}
          sx={{ width: "100%", color: "#38ffb9" }}
        />
      </LBSettingItem>
    </Box>
  );
}

export default BrightnessSlider;
