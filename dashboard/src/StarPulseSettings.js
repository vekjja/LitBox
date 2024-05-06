import React, { useEffect } from 'react';
import BrightnessSlider from './BrightnessSlider';
import SensitivitySlider from './SensitivitySlider';

function StarPulseSetting({ config, updateConfig }) {

    useEffect(() => {
        const newConfig = { ...config, visualization: 'starPulse' };
        updateConfig(newConfig);
        return () => {
            // perform clean-up tasks here if needed
        };
    }, []);


    return (
        <div className="setting">
            <BrightnessSlider config={config} updateConfig={updateConfig} />
            <SensitivitySlider config={config} updateConfig={updateConfig} />
        </div>
    );
}
export default StarPulseSetting;
