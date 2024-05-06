import React, { useEffect } from 'react';
import BrightnessSlider from './BrightnessSlider';

function BirdsSetting({ config, updateConfig }) {

    useEffect(() => {
        const newConfig = { ...config, visualization: 'birds' };
        updateConfig(newConfig);
        return () => {
            // perform clean-up tasks here if needed
        };
    }, []);

    return (
        <div className="setting">
            <BrightnessSlider config={config} updateConfig={updateConfig} />
            {/* Add other components or controls */}
        </div>
    );
}
export default BirdsSetting;
