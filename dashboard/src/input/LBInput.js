import React from "react";
import LBSettingItem from "../settings/LBSettingItem";

const LBInput = ({ label, value, onChange, placeholder, ...props }) => {
  return (
    <LBSettingItem label={label} defaultOpen={true}>
      <input
        type="text"
        value={value}
        onChange={onChange}
        placeholder={placeholder}
        style={{
          width: "100%",
          boxSizing: "border-box",
          display: "block",
          padding: "8px 12px",
          borderRadius: 8,
          border: "1px solid #444",
          background: "#222",
          color: "#fff",
          fontSize: 16,
          textAlign: "center",
        }}
        {...props}
      />
    </LBSettingItem>
  );
};

export default LBInput;
