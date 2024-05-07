import React, { useState, useCallback } from 'react';
import _ from 'lodash';

function ColorSelector({ config, updateConfig }) {
    const [pixelColor, setPixelColor] = useState(config.pixelColor);
    const [pixelBgColor, setPixelBgColor] = useState(config.pixelBgColor);

    const debouncedUpdateConfig = useCallback(_.debounce(() => {
        const newConfig = {
            ...config,
            pixelColor: pixelColor,
            pixelBgColor: pixelBgColor
        };
        updateConfig(newConfig);
    }, 333), [pixelColor, pixelBgColor, config, updateConfig]);

    const handlePixelColorChange = (event) => {
        setPixelColor(event.target.value);
        debouncedUpdateConfig();
    };

    const handleBgColorChange = (event) => {
        setPixelBgColor(event.target.value);
        debouncedUpdateConfig();
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
