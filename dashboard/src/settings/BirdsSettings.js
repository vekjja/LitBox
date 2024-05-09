import React, { useEffect } from 'react';
import BrightnessSlider from '../sliders/BrightnessSlider';
import FramerateSlider from '../sliders/FramerateSlider';
import ColorPalletSelector from '../selectors/ColorPalletSelector';

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
            <FramerateSlider config={config} updateConfig={updateConfig} />
            <ColorPalletSelector config={config} updateConfig={updateConfig} />
            <div class="setting">
                <label>Number of Birds</label>
                <input type="number" id="birdCount"
                    placeholder="Number of Birds"></input>
            </div>
            <div class="setting">
                <label>Alignment</label>
                <input type="number" id="birdAlignment"
                    placeholder="Alignment"></input>
            </div>
            <div class="setting">
                <label>Cohesion</label>
                <input type="number" id="birdCohesion"
                    placeholder="Cohesion"></input>
            </div>
            <div class="setting">
                <label>Separation</label>
                <input type="number" id="birdSeparation"
                    placeholder="Separation"></input>
            </div>
            <div class="setting">
                <label for="birdVeticalBounds">Vertical Bounds</label>
                <input id="birdVerticalBounds" type="checkbox"></input>
            </div>
            <div class="setting">
                <label for="birdHorizontalBounds">Horizontal
                    Bounds</label>
                <input id="birdHorizontalBounds" type="checkbox"></input>
            </div>
            <button id="updateBirds"
                class="clickable button">Update
                Birds</button>
            {/* Add other components or controls */}
        </div>
    );
}
export default BirdsSetting;
