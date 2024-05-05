import React, { useState } from 'react';

function BrightnessSlider({ config, updateConfig }) {
    const [brightness, setBrightness] = useState(config.brightness);

    const handleSliderChange = (event) => {
        setBrightness(event.target.value);
    };

    const handleSliderRelease = () => {
        const newConfig = { ...config, brightness: parseInt(brightness) };
        updateConfig(newConfig);
    };

    return (
        <div className="setting" id="brightness-settings">
            <input id="toggleBrightness" type="checkbox" class="toggle-button"></input>
            <label for="toggleBrightness" class="clickable">Brightness</label>
            <div class="toggle-content">
                <div class="setting">
                    <input
                        id="brightnessValue"
                        type="range"
                        min="1"
                        max="255"
                        value={brightness}
                        onChange={handleSliderChange}
                        onMouseUp={handleSliderRelease}
                        onTouchEnd={handleSliderRelease}
                    />
                </div>
            </div>
        </div>
    );
}

export default BrightnessSlider;
