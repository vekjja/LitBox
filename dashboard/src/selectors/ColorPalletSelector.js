import React, { useState } from 'react';

function ColorPalletSelector({ config, updateConfig }) {
    const [colors, setColors] = useState(config.colorPallet);

    const handleColorChange = (index) => (event) => {
        const newColors = [...colors];
        newColors[index] = event.target.value;
        setColors(newColors);
        updateConfig({
            ...config,
            colorPallet: newColors
        });
    };

    return (
        <div className="setting" id="color-pallet-settings">
            <input id="toggleColorPallet" type="checkbox" className="toggle-button"></input>
            <label htmlFor="toggleColorPallet" className="clickable">Color Pallet</label>
            <div className="toggle-content">
                {[...colors].reverse().map((color, index) => (
                    <div className="setting" key={index}>
                        <input type="color" id={`color${colors.length - index}`}
                            title={`Choose Color ${colors.length - index}`} className="color-picker"
                            value={color}
                            onChange={handleColorChange(colors.length - index - 1)}
                        ></input>
                    </div>
                ))}
            </div>
        </div>
    );
}

export default ColorPalletSelector;
