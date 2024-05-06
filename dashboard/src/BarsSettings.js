import React, { useEffect } from 'react';
import BrightnessSlider from './BrightnessSlider';
import SensitivitySlider from './SensitivitySlider';

function BarsSettings({ config, updateConfig }) {

    useEffect(() => {
        const newConfig = { ...config, visualization: 'bars' };
        updateConfig(newConfig);
        return () => {
            // perform clean-up tasks here if needed
        };
    }, []);

    return (
        <div className="setting">
            <BrightnessSlider config={config} updateConfig={updateConfig} />
            <SensitivitySlider config={config} updateConfig={updateConfig} />
            {/* Add other components or controls */}
        </div>
    );
}

export default BarsSettings;
