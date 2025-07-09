import React, { useEffect } from "react";
import BrightnessSlider from "../sliders/BrightnessSlider";
import FramerateSlider from "../sliders/FramerateSlider";
import ColorPalletSelector from "../selectors/ColorPalletSelector";
import Box from "@mui/material/Box";

function BirdsSetting({ config, updateConfig }) {
  useEffect(() => {
    const newConfig = { ...config, visualization: "birds" };
    updateConfig(newConfig);
    return () => {
      // perform clean-up tasks here if needed
    };
  }, []);

  return (
    <Box sx={{ width: "100%", maxWidth: 400, mx: "auto" }}>
      <BrightnessSlider config={config} updateConfig={updateConfig} />
      <FramerateSlider config={config} updateConfig={updateConfig} />
      <ColorPalletSelector config={config} updateConfig={updateConfig} />
    </Box>
  );
}

export default BirdsSetting;
