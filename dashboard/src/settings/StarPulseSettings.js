import React, { useEffect } from 'react';
import BrightnessSlider from '../sliders/BrightnessSlider';
import SensitivitySlider from '../sliders/SensitivitySlider';
import ColorPalletSelector from '../selectors/ColorPalletSelector';

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
            <ColorPalletSelector config={config} updateConfig={updateConfig} />
        </div>
    );
}
export default StarPulseSetting;
