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
        const updatedText = { ...localText, animation };
        setLocalText(updatedText);



        // Update the visualization only if animation is "display"
        const updatedConfig = {
            ...config,
            text: updatedText,
            visualization: animation === 'display' ? 'text' : config.visualization
        };

        updateConfig(updatedConfig);
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
                onClick={handleSendText}
                style={{ color: 'black', backgroundColor: '#38ffb9' }}
            >
                Send Text
            </button>
        </div>
    );
}

export default TextSettings;
