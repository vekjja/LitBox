import React, { useState } from "react";
import Dialog from "@mui/material/Dialog";
import DialogTitle from "@mui/material/DialogTitle";
import DialogContent from "@mui/material/DialogContent";
import DialogActions from "@mui/material/DialogActions";
import Fab from "@mui/material/Fab";
import Tabs from "@mui/material/Tabs";
import Tab from "@mui/material/Tab";
import TextField from "@mui/material/TextField";
import Tooltip from "@mui/material/Tooltip";
import Alert from "@mui/material/Alert";
import NetworkCheckIcon from "@mui/icons-material/NetworkCheck";
import EditIcon from "@mui/icons-material/Edit";
import SaveIcon from "@mui/icons-material/Save";
import RestartAltIcon from "@mui/icons-material/RestartAlt";
import IconButton from "@mui/material/IconButton";
import Switch from "@mui/material/Switch";
import FormControlLabel from "@mui/material/FormControlLabel";

function SystemSettings({ config, updateConfig, saveConfig }) {
  const [open, setOpen] = useState(false);
  const [tab, setTab] = useState(0);
  const [ssid, setSSID] = useState(config.client.ssid);
  const [password, setPassword] = useState(config.client.password);
  const [jsonConfig, setJsonConfig] = useState(JSON.stringify(config, null, 2));
  const [jsonError, setJsonError] = useState("");
  const [isEditable, setIsEditable] = useState(false);
  const [mode, setMode] = useState(config.client.mode || "client");
  const [mdns, setMdns] = useState(config.mdns || "");
  const [mdnsError, setMdnsError] = useState("");

  // RFC 1123 hostname validation
  function isValidHostname(name) {
    if (!name) return false;
    if (name.length > 253) return false;
    const labels = name.split(".");
    return labels.every(
      (label) =>
        label.length >= 1 &&
        label.length <= 63 &&
        /^[a-zA-Z0-9]([a-zA-Z0-9-]*[a-zA-Z0-9])?$/.test(label)
    );
  }

  // Helper to omit apiURL and version from config
  function omitConfigValues(obj) {
    const { apiURL, version, ...rest } = obj;
    return rest;
  }

  const handleOpen = () => {
    setSSID(config.client.ssid);
    setPassword(config.client.password);
    setMode(config.client.mode || "client");
    setMdns(config.mdns || "");
    setMdnsError("");
    setJsonConfig(JSON.stringify(omitConfigValues(config), null, 2));
    setJsonError("");
    setIsEditable(false);
    setOpen(true);
  };
  const handleClose = () => setOpen(false);

  const handleTabChange = (e, newValue) => setTab(newValue);

  // Only update local state and jsonConfig when mode changes
  const handleModeChange = (_, checked) => {
    const newMode = checked ? "client" : "ap";
    setMode(newMode);
    // Get new ssid and password from config for the selected mode
    const newSsid = config[newMode]?.ssid || "";
    const newPassword = config[newMode]?.password || "";
    setSSID(newSsid);
    setPassword(newPassword);
    // Update the local JSON config preview as well
    setJsonConfig((prev) => {
      const newConfig = {
        ...config,
        client: {
          ...config.client,
          ...(newMode === "client"
            ? { ssid: newSsid, password: newPassword }
            : {}),
          mode: newMode,
        },
        ap: {
          ...config.ap,
          ...(newMode === "ap" ? { ssid: newSsid, password: newPassword } : {}),
        },
        mode: newMode,
        mdns,
      };
      return JSON.stringify(omitConfigValues(newConfig), null, 2);
    });
  };

  // When editing SSID or password, update the correct section in the JSON preview
  const handleSsidChange = (e) => {
    const value = e.target.value;
    setSSID(value);
    setJsonConfig((prev) => {
      const newConfig = {
        ...config,
        client: {
          ...config.client,
          ...(mode === "client" ? { ssid: value } : {}),
          mode,
        },
        ap: {
          ...config.ap,
          ...(mode === "ap" ? { ssid: value } : {}),
        },
        mode,
        mdns,
      };
      return JSON.stringify(omitConfigValues(newConfig), null, 2);
    });
  };

  const handlePasswordChange = (e) => {
    const value = e.target.value;
    setPassword(value);
    setJsonConfig((prev) => {
      const newConfig = {
        ...config,
        client: {
          ...config.client,
          ...(mode === "client" ? { password: value } : {}),
          mode,
        },
        ap: {
          ...config.ap,
          ...(mode === "ap" ? { password: value } : {}),
        },
        mode,
        mdns,
      };
      return JSON.stringify(omitConfigValues(newConfig), null, 2);
    });
  };

  // Validate mDNS on change
  const handleMdnsChange = (e) => {
    const value = e.target.value;
    setMdns(value);
    if (!isValidHostname(value)) {
      setMdnsError(
        "Invalid hostname. Use 1-63 characters, no spaces, letters, digits, hyphens, can not start/end with hyphen."
      );
    } else {
      setMdnsError("");
    }
    // Update the local JSON config preview as well
    const newConfig = {
      ...config,
      client: {
        ...config.client,
        ssid,
        password,
        mode,
      },
      mdns: value,
    };
    setJsonConfig(JSON.stringify(omitConfigValues(newConfig), null, 2));
  };

  const handleSaveNetwork = () => {
    const newConfig = {
      ...config,
      client: {
        ...config.client,
        ssid,
        password,
        mode,
      },
      mdns,
    };
    const configToSave = omitConfigValues(newConfig);
    updateConfig(configToSave);
    saveConfig(configToSave);
  };

  const handleRestart = () => {
    fetch("/restart")
      .then((response) => {
        if (response.ok) {
          alert("System is Restarting");
        } else {
          alert("Failed to restart system.");
        }
      })
      .catch((error) => {
        console.error("Error restarting system:", error);
        alert("Error restarting system");
      });
  };

  const handleJsonChange = (e) => {
    setJsonConfig(e.target.value);
    if (jsonError) setJsonError("");
  };

  const handleSaveJson = () => {
    try {
      const parsed = JSON.parse(jsonConfig);
      if (!parsed.mdns) {
        setJsonError("Configuration must include 'mdns' field");
        return;
      }
      const configToSave = omitConfigValues(parsed);
      updateConfig(configToSave);
      saveConfig(configToSave);
      setIsEditable(false);
      setJsonError("");
      setOpen(false);
    } catch (e) {
      setJsonError("Invalid JSON format. Please check your configuration.");
    }
  };

  const toggleEditability = () => setIsEditable((prev) => !prev);

  return (
    <>
      <Tooltip title="System Settings">
        <Fab
          size="small"
          color="primary"
          onClick={handleOpen}
          sx={{ position: "fixed", top: 20, left: 20, zIndex: 1301 }}
        >
          <NetworkCheckIcon />
        </Fab>
      </Tooltip>
      <Dialog open={open} onClose={handleClose} maxWidth="md" fullWidth>
        <DialogTitle>System Settings</DialogTitle>
        <Tabs value={tab} onChange={handleTabChange} sx={{ px: 2 }}>
          <Tab label="Network" />
          <Tab label="JSON" />
        </Tabs>
        <DialogContent dividers sx={{ minHeight: 400 }}>
          {tab === 0 && (
            <>
              <TextField
                label="Device Name/mDNS"
                value={mdns}
                onChange={handleMdnsChange}
                error={!!mdnsError}
                helperText={mdnsError}
                fullWidth
                margin="normal"
                sx={{ mb: 2 }}
              />
              <FormControlLabel
                control={
                  <Switch
                    checked={mode === "client"}
                    onChange={handleModeChange}
                    color="primary"
                  />
                }
                label={`${mode === "client" ? "Client" : "AP"}`}
                sx={{ mt: 1, mb: 2 }}
              />
              <TextField
                label="SSID"
                value={ssid}
                onChange={handleSsidChange}
                fullWidth
                margin="normal"
                sx={{ mb: 2 }}
              />
              <TextField
                label="Password"
                value={password}
                onChange={handlePasswordChange}
                fullWidth
                margin="normal"
                type="password"
                sx={{ mb: 2 }}
              />
            </>
          )}
          {tab === 1 && (
            <>
              {jsonError && (
                <Alert severity="error" sx={{ mb: 2 }}>
                  {jsonError}
                </Alert>
              )}
              <TextField
                label="Configuration JSON"
                value={jsonConfig}
                onChange={handleJsonChange}
                variant="outlined"
                fullWidth
                multiline
                rows={16}
                error={!!jsonError}
                disabled={!isEditable}
                sx={{
                  mt: 2,
                  fontFamily: "monospace",
                  "& .MuiInputBase-input": {
                    fontFamily: "monospace",
                    fontSize: "0.875rem",
                  },
                }}
              />
            </>
          )}
        </DialogContent>
        {/* Shared Icon Buttons */}
        {(() => {
          // Save button for Network tab
          const saveNetworkButton = (
            <Tooltip title="Save Network Settings" key="save-network">
              <IconButton
                color="primary"
                aria-label="save network settings"
                onClick={handleSaveNetwork}
                sx={{ mr: 1 }}
                disabled={!!mdnsError}
              >
                <SaveIcon />
              </IconButton>
            </Tooltip>
          );
          // Edit button for JSON tab
          const editJsonButton = (
            <Tooltip
              title={isEditable ? "Disable Editing" : "Enable Editing"}
              key="edit-json"
            >
              <IconButton
                color={isEditable ? "secondary" : "primary"}
                aria-label={isEditable ? "disable editing" : "enable editing"}
                onClick={toggleEditability}
                sx={{ mr: 1 }}
              >
                <EditIcon />
              </IconButton>
            </Tooltip>
          );
          // Save button for JSON tab
          const saveJsonButton = (
            <Tooltip title="Save to Device" key="save-json">
              <span>
                <IconButton
                  color="primary"
                  aria-label="save to device"
                  onClick={handleSaveJson}
                  disabled={!isEditable || !!mdnsError}
                  sx={{ mr: 1 }}
                >
                  <SaveIcon />
                </IconButton>
              </span>
            </Tooltip>
          );
          // Restart button (used in both tabs)
          const restartButton = (
            <Tooltip title="Restart System" key="restart">
              <IconButton
                color="white"
                aria-label="restart system"
                onClick={handleRestart}
                sx={{ mr: 1 }}
              >
                <RestartAltIcon />
              </IconButton>
            </Tooltip>
          );
          return (
            <DialogActions>
              {tab === 0 && (
                <>
                  {saveNetworkButton}
                  {restartButton}
                </>
              )}
              {tab === 1 && (
                <>
                  {editJsonButton}
                  {saveJsonButton}
                  {restartButton}
                </>
              )}
            </DialogActions>
          );
        })()}
      </Dialog>
    </>
  );
}

export default SystemSettings;
