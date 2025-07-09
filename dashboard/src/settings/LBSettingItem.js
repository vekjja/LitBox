import React, { useState } from "react";
import Box from "@mui/material/Box";
import Typography from "@mui/material/Typography";
import Collapse from "@mui/material/Collapse";

const LBSettingItem = ({
  label,
  children,
  defaultOpen = false,
  width = "100%",
  maxWidth = 400,
  ...props
}) => {
  const [open, setOpen] = useState(defaultOpen);

  return (
    <Box sx={{ mb: 2, width, maxWidth, boxSizing: "border-box" }} {...props}>
      <Box
        onClick={() => setOpen((prev) => !prev)}
        sx={{
          cursor: "pointer",
          bgcolor: "grey.800",
          color: "white",
          borderTopLeftRadius: 16,
          borderTopRightRadius: 16,
          borderBottomLeftRadius: open ? 0 : 16,
          borderBottomRightRadius: open ? 0 : 16,
          px: 4,
          py: 2,
          textAlign: "center",
          fontWeight: 500,
          fontSize: 20,
          boxShadow: open ? 4 : 1,
          transition: "box-shadow 0.2s, border-radius 0.2s",
          "&:hover": { bgcolor: "grey.700" },
          width: "100%",
          boxSizing: "border-box",
        }}
      >
        <Typography
          variant="h6"
          sx={{ m: 0, p: 0, fontWeight: 500, fontSize: 20 }}
        >
          {label}
        </Typography>
      </Box>
      <Collapse in={open} timeout="auto" unmountOnExit>
        <Box
          sx={{
            mt: 0,
            width: "100%",
            bgcolor: "grey.900",
            borderTopLeftRadius: 0,
            borderTopRightRadius: 0,
            borderBottomLeftRadius: 16,
            borderBottomRightRadius: 16,
            py: 2,
            boxShadow: 2,
            display: "block",
            px: 0,
            boxSizing: "border-box",
          }}
        >
          <Box sx={{ px: 4, width: "100%", boxSizing: "border-box" }}>
            {children}
          </Box>
        </Box>
      </Collapse>
    </Box>
  );
};

export default LBSettingItem;
