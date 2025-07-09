import React, { useState } from "react";
import ColorSelector from "../selectors/ColorSelector";
import BrightnessSlider from "../sliders/BrightnessSlider";
// import TextSpeedSlider from "../sliders/TextSpeedSlider";
import LBDropdown from "../selectors/LBDropdown";
import LBInput from "../input/LBInput";
import { LBSettings } from "./LBSettings";

function TextSettings({ config, updateConfig, visualizationDropdown }) {
  // Initialize local state for the text settings
  const [localText, setLocalText] = useState(config.text);

  const handleTextChange = (e) => {
    setLocalText({ ...localText, content: e.target.value });
  };

  const handleAnimationChange = (animation) => {
    var updatedText = { ...localText, animation };
    setLocalText(updatedText);
    // Do NOT update visualization here, just update text state
  };

  const handleSendText = () => {
    // When sending text, update config and visualization if needed
    const updatedConfig = {
      ...config,
      text: localText,
      visualization: "text", // Only set visualization here
    };
    updateConfig(updatedConfig);

    fetch(`${config.apiURL}/text`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(updatedConfig),
    })
      .then((response) => {
        if (!response.ok) throw new Error("Failed to send text");
        return response.json();
      })
      .then((data) => {
        console.log("Text Sent:", data);
      })
      .catch((error) => console.error("Error sending config:", error));
  };

  return (
    <LBSettings
      label="Text Settings"
      config={config}
      updateConfig={updateConfig}
      // Do NOT pass visualization prop here
    >
      {visualizationDropdown}
      <BrightnessSlider config={config} updateConfig={updateConfig} />
      <ColorSelector config={config} updateConfig={updateConfig} />
      {/* <TextSpeedSlider config={config} updateConfig={updateConfig} /> */}

      <LBDropdown
        label="Text Animation"
        value={localText.animation}
        onChange={(e) => handleAnimationChange(e.target.value)}
        options={[
          { value: "scroll", label: "Scroll" },
          { value: "wave", label: "Wave" },
          { value: "display", label: "Display" },
        ]}
      />
      <LBInput
        label="Text Content"
        value={localText.content}
        onChange={handleTextChange}
        placeholder="Enter text"
      />
      <button id="greenButton" onClick={handleSendText}>
        Send Text
      </button>
    </LBSettings>
  );
}

export default TextSettings;
