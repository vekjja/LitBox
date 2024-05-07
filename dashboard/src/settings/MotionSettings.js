import React, { useEffect } from 'react';
import BrightnessSlider from '../sliders/BrightnessSlider';
import ColorPalletSelector from '../selectors/ColorPalletSelector';

function MotionSettings({ config, updateConfig }) {

    useEffect(() => {
        const newConfig = { ...config, visualization: 'motion' };
        updateConfig(newConfig);
        return () => {
            // perform clean-up tasks here if needed
        };
    }, []);


    return (
        <div className="setting">
            <BrightnessSlider config={config} updateConfig={updateConfig} />
            <ColorPalletSelector config={config} updateConfig={updateConfig} />
            {/* Add other components or controls */}
        </div>
    );
}

export default MotionSettings;
