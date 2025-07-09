import React, { useEffect } from "react";
import BrightnessSlider from "../sliders/BrightnessSlider";
import SensitivitySlider from "../sliders/SensitivitySlider";
import ColorPalletSelector from "../selectors/ColorPalletSelector";
import Box from "@mui/material/Box";

function CirclesSetting({ config, updateConfig }) {
  useEffect(() => {
    const newConfig = { ...config, visualization: "circles" };
    updateConfig(newConfig);
    return () => {
      // perform clean-up tasks here if needed
    };
  }, []);

  return (
    <Box sx={{ width: "100%", maxWidth: 400, mx: "auto" }}>
      <BrightnessSlider config={config} updateConfig={updateConfig} />
      <SensitivitySlider config={config} updateConfig={updateConfig} />
      <ColorPalletSelector config={config} updateConfig={updateConfig} />
    </Box>
  );
}
export default CirclesSetting;
