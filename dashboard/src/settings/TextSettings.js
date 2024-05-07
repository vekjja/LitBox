import React, { useEffect } from 'react';
import BrightnessSlider from '../sliders/BrightnessSlider';
import FramerateSlider from '../sliders/FramerateSlider';
import ColorSelector from '../selectors/ColorSelector';

function TextSettings({ config, updateConfig }) {

    useEffect(() => {
        const newConfig = { ...config, visualization: 'text' };
        updateConfig(newConfig);
        return () => {
            // perform clean-up tasks here if needed
        };
    }, []);


    return (
        <div className="setting">
            <BrightnessSlider config={config} updateConfig={updateConfig} />
            <FramerateSlider config={config} updateConfig={updateConfig} />
            <ColorSelector config={config} updateConfig={updateConfig} />
            {/* Add other components or controls */}
        </div>
    );
}

export default TextSettings;
