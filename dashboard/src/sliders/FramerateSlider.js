import React, { useState } from "react";
import Slider from "@mui/material/Slider";
import Box from "@mui/material/Box";
import LBSettingItem from "../settings/LBSettingItem";

function FramerateSlider({ config, updateConfig }) {
  const [frameRate, setFrameRate] = useState(config.frameRate);

  const handleSliderRelease = () => {
    const newConfig = { ...config, frameRate: parseInt(frameRate) };
    updateConfig(newConfig);
  };

  return (
    <Box>
      <LBSettingItem label="Frame Rate">
        <Slider
          id="framerateValue"
          min={1}
          max={255}
          value={Number(frameRate)}
          onChange={(e, value) => setFrameRate(value)}
          onChangeCommitted={() => handleSliderRelease()}
          sx={{ width: "100%", color: "#38ffb9" }}
        />
      </LBSettingItem>
    </Box>
  );
}

export default FramerateSlider;
