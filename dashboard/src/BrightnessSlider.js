import React, { useState } from 'react';

function BrightnessSlider({ config, updateConfig }) {
    const [brightness, setBrightness] = useState(config.brightness);

    const handleSliderChange = (event) => {
        setBrightness(event.target.value);
    };

    const handleSliderRelease = () => {
        // Update the brightness on the server or backend
        // console.log('Brightness set to:', brightness);
        // fetch('/brightness', {
        //     method: 'POST',
        //     headers: { 'Content-Type': 'text/plain' },
        //     body: brightness.toString()
        // })
        //     .then(response => {
        //         if (!response.ok) {
        //             throw new Error('Failed to update brightness');
        //         }
        //         return response.text();
        //     })
        //     .then(data => console.log('Update successful:', data))
        //     .catch(error => console.error('Failed to update brightness:', error));
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
