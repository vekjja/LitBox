import React from "react";
import LBSettingItem from "../settings/LBSettingItem";

const LBDropdown = ({ label, value, onChange, options, ...props }) => {
  return (
    <LBSettingItem label={label} defaultOpen={true}>
      <select
        value={value}
        onChange={onChange}
        style={{
          width: "100%",
          boxSizing: "border-box",
          padding: "8px 12px",
          borderRadius: 8,
          border: "1px solid #444",
          background: "#222",
          color: "#fff",
          fontSize: 16,
          textAlign: "center",
        }}
        {...props}
      >
        {options.map((opt) => (
          <option key={opt.value} value={opt.value}>
            {opt.label}
          </option>
        ))}
      </select>
    </LBSettingItem>
  );
};

export default LBDropdown;
