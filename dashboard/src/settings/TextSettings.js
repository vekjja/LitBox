import React, { useState } from 'react';
import BrightnessSlider from '../sliders/BrightnessSlider';
import ColorSelector from '../selectors/ColorSelector';

function TextSettings({ config, updateConfig }) {
    // Initialize local state for the text settings
    const [localText, setLocalText] = useState(config.text);

    const handleTextChange = (e) => {
        setLocalText({ ...localText, content: e.target.value });
    };

    const handleAnimationChange = (animation) => {
        var updatedText = { ...localText, animation };
        setLocalText(updatedText);

        if (animation === 'display') {
            const updatedConfig = { ...config, text: localText, visualization: 'text' };
            updateConfig(updatedConfig);
        }
    };

    const handleSliderChange = (event) => {
        setLocalText({ ...localText, speed: event.target.value });
    };

    const handleSendText = () => {
        const updatedConfig = { ...config, text: localText };
        updateConfig(updatedConfig);

        fetch('/text', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(updatedConfig),
        })
            .then((response) => {
                if (!response.ok) throw new Error('Failed to send text');
                return response.json();
            })
            .then((data) => {
                console.log('Config updated:', data);
            })
            .catch((error) => console.error('Error sending config:', error));
    };

    return (
        <div className="setting">
            <BrightnessSlider config={config} updateConfig={updateConfig} />
            <ColorSelector config={config} updateConfig={updateConfig} />
            <div className="setting" id="text-speed-settings">
                <input id="toggleTextSpeed" type="checkbox" class="toggle-button"></input>
                <label for="toggleTextSpeed" class="clickable">Text Speed</label>
                <div class="toggle-content">
                    <div class="setting">
                        <input
                            id="TextSpeedValue"
                            type="range"
                            min="1"
                            max="100"
                            value={localText.speed}
                            onChange={handleSliderChange}
                        />
                    </div>
                </div>
            </div>
            <div className="setting">
                <select
                    id="textAnimationSelect"
                    className="clickable"
                    value={localText.animation}
                    onChange={(e) => handleAnimationChange(e.target.value)}
                >
                    <option value="scroll">Scroll</option>
                    <option value="wave">Wave</option>
                    <option value="display">Display</option>
                </select>
            </div>
            <input
                value={localText.content}
                onChange={handleTextChange}
                placeholder="Enter text"
            />
            <button
                id='greenButton'
                onClick={handleSendText}
            >
                Send Text
            </button>
        </div>
    );
}

export default TextSettings;
