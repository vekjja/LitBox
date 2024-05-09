import React, { useState } from 'react';
import BrightnessSlider from '../sliders/BrightnessSlider';
import FramerateSlider from '../sliders/FramerateSlider';
import ColorSelector from '../selectors/ColorSelector';

function TextSettings({ config, updateConfig }) {
    const [localConfig, setLocalConfig] = useState({ ...config });

    const handleTextChange = (e) => {
        const updatedText = { ...localConfig.text, content: e.target.value };
        setLocalConfig({ ...localConfig, text: updatedText });
    };

    const handleSendText = () => {
        fetch('/text', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(localConfig),
        }).then(response => {
            if (!response.ok) throw new Error('Failed to send text');
            return response.json();
        }).then(data => {
            console.log('Config updated:', data);
            // alert('Config updated');
        }).catch(error => console.error('Error sending config:', error));
    }

    return (
        <div className="setting">
            <BrightnessSlider config={config} updateConfig={updateConfig} />
            <FramerateSlider config={config} updateConfig={updateConfig} />
            <ColorSelector config={config} updateConfig={updateConfig} />
            <input
                value={localConfig.text.content}
                onChange={handleTextChange}
                placeholder="Enter text"
            />
            <button onClick={handleSendText} style={{ color: "black", backgroundColor: '#38ffb9' }}>Send Text</button>
        </div>
    );
}

export default TextSettings;
