import React from "react";
import Button from "@mui/material/Button";

function LBButton({ label, onClick, sx = {} }) {
  return (
    <Button
      onClick={onClick}
      variant="contained"
      sx={{
        height: "63px",
        width: "100%",
        color: "black",
        borderTopLeftRadius: 16,
        borderTopRightRadius: 16,
        borderBottomRightRadius: 16,
        borderBottomLeftRadius: 16,
        backgroundColor: "#primary",
        fontWeight: "bold",
        boxShadow: 2,
        ...sx,
      }}
    >
      {label}
    </Button>
  );
}

export default LBButton;
