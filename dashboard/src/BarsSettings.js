import BrightnessSlider from './BrightnessSlider';

function BarsSettings({ config, updateConfig }) {

    return (
        <div className="setting">
            <BrightnessSlider config={config} updateConfig={updateConfig} />
            // add slider for sensitivity
        // add button to save the configuration
        </div>
    );
}

export default BarsSettings;
