import BrightnessSlider from './BrightnessSlider';

function BarsSettings({ config, updateConfig }) {

    return (
        <div className="setting">
            <BrightnessSlider config={config} updateConfig={updateConfig} />
        </div>
    );
}

export default BarsSettings;
