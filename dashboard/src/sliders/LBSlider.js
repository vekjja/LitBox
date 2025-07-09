import React from "react";
import Slider from "@mui/material/Slider";
import Box from "@mui/material/Box";
import LBSettingItem from "../settings/LBSettingItem";

function LBSlider({ label, min, max, value, onChange, onChangeCommitted, sx }) {
  return (
    <Box>
      <LBSettingItem label={label}>
        <Slider
          min={min}
          max={max}
          value={value}
          onChange={onChange}
          onChangeCommitted={onChangeCommitted}
          sx={{ color: "primary", width: "100%", ...sx }}
        />
      </LBSettingItem>
    </Box>
  );
}

export default LBSlider;
