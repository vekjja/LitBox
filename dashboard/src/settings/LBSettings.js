import React, { useEffect } from "react";
import Box from "@mui/material/Box";

export const LBSettings = ({
  label,
  children,
  width = "100%",
  maxWidth = 400,
  config,
  updateConfig,
  visualization,
  ...props
}) => {
  useEffect(() => {
    if (visualization && config && updateConfig) {
      if (config.visualization !== visualization) {
        const newConfig = { ...config, visualization };
        updateConfig(newConfig);
      }
    }
    // No clean-up needed
  }, [visualization, config, updateConfig]);
  return (
    <Box
      sx={{ mb: 4, width, maxWidth, mx: "auto", boxSizing: "border-box" }}
      {...props}
    >
      <Box sx={{ width: "100%", boxSizing: "border-box" }}>{children}</Box>
    </Box>
  );
};
