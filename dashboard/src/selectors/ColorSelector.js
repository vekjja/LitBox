import React, { useState } from 'react';

function ColorSelector({ config, updateConfig }) {
    const [pixelColor, setPixelColor] = useState(config.pixelColor);
    const [pixelBgColor, setPixelBgColor] = useState(config.pixelBgColor);

    const handlePixelColorChange = (event) => {
        const newPixelColor = event.target.value;
        setPixelColor(newPixelColor);
        updateConfig({
            ...config,
            pixelColor: newPixelColor,
        });
    };

    const handleBgColorChange = (event) => {
        const newBgColor = event.target.value;
        setPixelBgColor(newBgColor);
        updateConfig({
            ...config,
            pixelBgColor: newBgColor
        });
    };

    return (
        <div className="setting" id="color-settings">
            <input id="toggleColor" type="checkbox" className="toggle-button"></input>
            <label htmlFor="toggleColor" className="clickable">Color</label>
            <div className="toggle-content">
                <div className="setting">
                    <input type="color" id="pixelColor"
                        title="Choose A Color" className="color-picker"
                        value={pixelColor}
                        onChange={handlePixelColorChange}
                    ></input>
                </div>
                <div className="setting">
                    <input type="color" id="pixelBgColor"
                        title="Choose A Background Color" className="color-picker"
                        value={pixelBgColor}
                        onChange={handleBgColorChange}
                    ></input>
                </div>
            </div>
        </div>
    );
}

export default ColorSelector;
