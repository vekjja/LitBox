import React from "react";

const LBColorSelector = ({ label, value, onChange, id }) => (
  <input
    type="color"
    id={id}
    title={label}
    className="color-picker"
    value={value}
    onChange={onChange}
    style={{
      width: 48,
      height: 32,
      border: "none",
      background: "none",
      padding: 0,
    }}
  />
);

export default LBColorSelector;
