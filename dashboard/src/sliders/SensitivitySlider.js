import React, { useState } from "react";
import Slider from "@mui/material/Slider";
import Box from "@mui/material/Box";
import LBSettingItem from "../settings/LBSettingItem";

function SensitivitySlider({ config, updateConfig }) {
  const [sensitivity, setSensitivity] = useState(config.sensitivity);

  const handleSliderRelease = () => {
    const newConfig = { ...config, sensitivity: parseInt(sensitivity) };
    updateConfig(newConfig);
  };

  return (
    <Box>
      <LBSettingItem label="Sensitivity">
        <Slider
          id="sensitivityValue"
          min={1}
          max={100}
          value={Number(sensitivity)}
          onChange={(e, value) => setSensitivity(value)}
          onChangeCommitted={() => handleSliderRelease()}
          sx={{ width: "100%", color: "#38ffb9" }}
        />
      </LBSettingItem>
    </Box>
  );
}

export default SensitivitySlider;
