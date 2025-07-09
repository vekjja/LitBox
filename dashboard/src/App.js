import React, { useState, useEffect } from "react";
import SystemSettings from "./settings/SystemSettings";
import BarsSettings from "./settings/BarsSettings";
import GameOfLifeSettings from "./settings/GameOfLifeSettings";
import MatrixSettings from "./settings/MatrixSettings";
import WaveformSetting from "./settings/WaveformSettings";
import BirdsSetting from "./settings/BirdsSettings";
import CirclesSetting from "./settings/CirclesSetting";
import StarPulseSetting from "./settings/StarPulseSettings";
import TemperatureSetting from "./settings/TemperatureSettings";
import MotionSettings from "./settings/MotionSettings";
import TextSettings from "./settings/TextSettings";

// Material UI imports
import { ThemeProvider, createTheme } from "@mui/material/styles";
import Container from "@mui/material/Container";
import Typography from "@mui/material/Typography";
import Box from "@mui/material/Box";
import LBDropdown from "./selectors/LBDropdown";
import AppBar from "@mui/material/AppBar";

// Define the theme
const theme = createTheme({
  typography: {
    fontFamily: [
      "-apple-system",
      "BlinkMacSystemFont",
      '"Segoe UI"',
      "Roboto Slab",
      '"Helvetica Neue"',
      "Arial",
      "sans-serif",
      '"Apple Color Emoji"',
      '"Segoe UI Emoji"',
      '"Segoe UI Symbol"',
    ].join(","),
  },
  palette: {
    mode: "dark",
    primary: {
      main: "#47FFF0",
      alt: "#FF4186",
    },
    secondary: {
      main: "#333",
    },
    error: {
      main: "#FF5656",
    },
    success: {
      main: "#17EB9D",
    },
  },
  components: {},
});

function App() {
  const hostname = process.env.REACT_APP_API_HOST || "localhost";
  const port = process.env.REACT_APP_API_PORT || 80;
  const apiURL =
    process.env.NODE_ENV === "production" ? "" : `http://${hostname}:${port}`;

  var [config, setConfig] = useState(null);
  const [selectedSetting, setSelectedSetting] = useState("bars");

  useEffect(() => {
    fetch(apiURL + "/config")
      .then((response) => response.json())
      .then((data) => {
        console.log("Configuration loaded:", data);
        const configWithAPI = { ...data, apiURL };
        setConfig(configWithAPI);
        setSelectedSetting(data.visualization);
      })
      .catch((error) => console.error("Error loading configuration:", error));
  }, []);

  const saveConfig = (newConfig) => {
    fetch(apiURL + "/config", {
      method: "PUT",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(newConfig),
    })
      .then((response) => {
        if (!response.ok) throw new Error("Failed to save configuration");
        return response.json();
      })
      .then((data) => {
        setConfig(newConfig);
        console.log("Configuration Saved:", data);
        alert("Configuration Saved");
      })
      .catch((error) => console.error("Error updating configuration:", error));
  };

  const updateConfig = (newConfig) => {
    fetch(apiURL + "/config", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(newConfig),
    })
      .then((response) => {
        if (!response.ok) throw new Error("Failed to update configuration");
        return response.json();
      })
      .then((data) => {
        setConfig(newConfig);
        console.log("Configuration Updated:", data);
      })
      .catch((error) => console.error("Error updating configuration:", error));
  };

  if (!config) {
    return (
      <ThemeProvider theme={theme}>
        <Container maxWidth="sm" sx={{ mt: 6 }}>
          <Typography variant="h4" align="center" gutterBottom>
            Lit Box
          </Typography>
          <Typography align="center">Loading configuration... ⚙️</Typography>
        </Container>
      </ThemeProvider>
    );
  }

  const renderSetting = () => {
    const visualizationDropdown = (
      <LBDropdown
        label="Visualization"
        value={selectedSetting}
        onChange={(e) => setSelectedSetting(e.target.value)}
        defaultOpen={true}
        options={[
          { value: "bars", label: "Bars" },
          { value: "birds", label: "Birds" },
          { value: "circles", label: "Circles" },
          { value: "gameOfLife", label: "Game of Life" },
          { value: "matrix", label: "Matrix" },
          { value: "motion", label: "Motion" },
          { value: "starPulse", label: "Star Pulse" },
          { value: "text", label: "Text" },
          { value: "temperature", label: "Temperature" },
          { value: "waveform", label: "Waveform" },
          { value: "wifi", label: "System" },
          { value: "about", label: "About" },
        ]}
        // sx={{ backgroundColor: "background.paper", mb: 2 }}
      />
    );
    switch (selectedSetting) {
      case "bars":
        return (
          <BarsSettings
            config={config}
            updateConfig={updateConfig}
            visualizationDropdown={visualizationDropdown}
          />
        );
      case "birds":
        return (
          <BirdsSetting
            config={config}
            updateConfig={updateConfig}
            visualizationDropdown={visualizationDropdown}
          />
        );
      case "circles":
        return (
          <CirclesSetting
            config={config}
            updateConfig={updateConfig}
            visualizationDropdown={visualizationDropdown}
          />
        );
      case "gameOfLife":
        return (
          <GameOfLifeSettings
            config={config}
            updateConfig={updateConfig}
            visualizationDropdown={visualizationDropdown}
          />
        );
      case "matrix":
        return (
          <MatrixSettings
            config={config}
            updateConfig={updateConfig}
            visualizationDropdown={visualizationDropdown}
          />
        );
      case "motion":
        return (
          <MotionSettings
            config={config}
            updateConfig={updateConfig}
            visualizationDropdown={visualizationDropdown}
          />
        );
      case "starPulse":
        return (
          <StarPulseSetting
            config={config}
            updateConfig={updateConfig}
            visualizationDropdown={visualizationDropdown}
          />
        );
      case "text":
        return (
          <TextSettings
            config={config}
            updateConfig={updateConfig}
            visualizationDropdown={visualizationDropdown}
          />
        );
      case "temperature":
        return (
          <TemperatureSetting
            config={config}
            updateConfig={updateConfig}
            visualizationDropdown={visualizationDropdown}
          />
        );
      case "waveform":
        return (
          <WaveformSetting
            config={config}
            updateConfig={updateConfig}
            visualizationDropdown={visualizationDropdown}
          />
        );
      case "about":
        return (
          <div className="setting" id="about-settings">
            {visualizationDropdown}
            <div className="setting">
              <label htmlFor="version">Version</label>
              <label id="version">{config.version}</label>
            </div>
            <div className="setting">
              <label>Designed and Developed by:</label>
              <a
                href="https://github.com/seemywingz/LitBox"
                target="_blank"
                id="SeeMyWingZ"
                rel="noreferrer"
              >
                SeeMyWingZ
              </a>
            </div>
          </div>
        );
      default:
        return <div>Ho'd Yo Get Here? ��</div>;
    }
  };

  return (
    <ThemeProvider theme={theme}>
      <Container maxWidth="sm" sx={{}}>
        <AppBar position="fixed">
          <SystemSettings
            config={config}
            updateConfig={updateConfig}
            saveConfig={saveConfig}
          />
          <Typography
            variant="h4"
            align="center"
            gutterBottom
            color="primary"
            sx={{ mt: 3, mb: 3 }}
          >
            Lit Box
          </Typography>
        </AppBar>
        <Box sx={{ mt: 15 }}>{renderSetting()}</Box>
      </Container>
    </ThemeProvider>
  );
}

export default App;
