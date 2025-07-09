import React from "react";
import LBSettingItem from "../settings/LBSettingItem";
import FormControl from "@mui/material/FormControl";
import Select from "@mui/material/Select";
import MenuItem from "@mui/material/MenuItem";

const LBDropdown = ({
  label,
  value,
  onChange,
  options,
  defaultOpen = false,
  ...props
}) => {
  return (
    <LBSettingItem label={label} defaultOpen={defaultOpen}>
      <FormControl fullWidth variant="outlined" size="small">
        <Select value={value} onChange={onChange} displayEmpty {...props}>
          {options.map((opt) => (
            <MenuItem key={opt.value} value={opt.value}>
              {opt.label}
            </MenuItem>
          ))}
        </Select>
      </FormControl>
    </LBSettingItem>
  );
};

export default LBDropdown;
