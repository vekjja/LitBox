import React, { useState, useEffect } from 'react';
import BrightnessSlider from '../sliders/BrightnessSlider';
import FramerateSlider from '../sliders/FramerateSlider';
import ColorSelector from '../selectors/ColorSelector';

function TemperatureSettings({ config, updateConfig }) {
    var [temperatureUnit, setTemperatureUnit] = useState(config.temperatureUnit);


    useEffect(() => {
        const newConfig = { ...config, visualization: 'temperature' };
        updateConfig(newConfig);
        return () => {
            // perform clean-up tasks here if needed
        };
    }, []);

    const handleChange = (e) => {
        setTemperatureUnit(e.target.value)
        updateConfig({ ...config, temperatureUnit: e.target.value });
    }


    return (
        <div className="setting">
            <BrightnessSlider config={config} updateConfig={updateConfig} />
            <FramerateSlider config={config} updateConfig={updateConfig} />
            <ColorSelector config={config} updateConfig={updateConfig} />
            <div className="setting">
                <select
                    id="settingSelect"
                    className="clickable"
                    value={temperatureUnit}
                    onChange={handleChange}
                >
                    <option value="C">Celsius</option>
                    <option value="F">Fahrenheit</option>
                </select>
            </div>
        </div>
    );
}

export default TemperatureSettings;
