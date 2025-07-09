import React, { useState } from "react";
import Dialog from "@mui/material/Dialog";
import DialogTitle from "@mui/material/DialogTitle";
import DialogContent from "@mui/material/DialogContent";
import DialogActions from "@mui/material/DialogActions";
import Fab from "@mui/material/Fab";
import Tabs from "@mui/material/Tabs";
import Tab from "@mui/material/Tab";
import TextField from "@mui/material/TextField";
import Button from "@mui/material/Button";
import Tooltip from "@mui/material/Tooltip";
import Alert from "@mui/material/Alert";
import NetworkCheckIcon from "@mui/icons-material/NetworkCheck";
import EditIcon from "@mui/icons-material/Edit";
import SaveIcon from "@mui/icons-material/Save";
import RestartAltIcon from "@mui/icons-material/RestartAlt";
import IconButton from "@mui/material/IconButton";

function SystemSettings({ config, updateConfig, saveConfig }) {
  const [open, setOpen] = useState(false);
  const [tab, setTab] = useState(0);
  const [ssid, setSSID] = useState(config.client.ssid);
  const [password, setPassword] = useState(config.client.password);
  const [jsonConfig, setJsonConfig] = useState(JSON.stringify(config, null, 2));
  const [jsonError, setJsonError] = useState("");
  const [isEditable, setIsEditable] = useState(false);

  const handleOpen = () => {
    setSSID(config.client.ssid);
    setPassword(config.client.password);
    setJsonConfig(JSON.stringify(config, null, 2));
    setJsonError("");
    setIsEditable(false);
    setOpen(true);
  };
  const handleClose = () => setOpen(false);

  const handleTabChange = (e, newValue) => setTab(newValue);

  const handleSaveNetwork = () => {
    const newConfig = {
      ...config,
      client: {
        ...config.client,
        ssid,
        password,
      },
    };
    updateConfig(newConfig);
    saveConfig(newConfig);
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
      updateConfig(parsed);
      saveConfig(parsed);
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
                label="SSID"
                value={ssid}
                onChange={(e) => setSSID(e.target.value)}
                fullWidth
                margin="normal"
                sx={{ mb: 2 }}
              />
              <TextField
                label="Password"
                value={password}
                onChange={(e) => setPassword(e.target.value)}
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
                  disabled={!isEditable}
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
