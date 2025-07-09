// Utility: Convert a number to a hex color string (e.g., 16711680 -> "#ff0000")
export function toHexColor(val) {
  if (typeof val === "string" && val.startsWith("#") && val.length === 7)
    return val;
  let num = typeof val === "number" ? val : parseInt(val, 10);
  if (isNaN(num)) return "#000000";
  let hex = num.toString(16).padStart(6, "0");
  return `#${hex}`;
}
