import React, { useEffect } from 'react';
import BrightnessSlider from '../sliders/BrightnessSlider';
import SensitivitySlider from '../sliders/SensitivitySlider';
import ColorPalletSelector from '../selectors/ColorPalletSelector';

function WaveformSetting({ config, updateConfig }) {

    useEffect(() => {
        const newConfig = { ...config, visualization: 'waveform' };
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
            {/* Add other components or controls */}
        </div>
    );
}
export default WaveformSetting;
