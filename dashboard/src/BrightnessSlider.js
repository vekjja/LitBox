import React, { useState, useEffect } from 'react';

function BrightnessSlider() {

    return (
        <div class="setting" id="brightness-settings">
            <input id="toggleBrightness" type="checkbox" class="toggle-button">
            </input>
            <label for="toggleBrightness" class="clickable">Brightness</label>
            <div class="toggle-content">
                <div class="setting">
                    <input id="brightnessValue" type="range"
                        min="1" max="255">
                    </input>
                </div>
            </div>
        </div>
    );
}

export default BrightnessSlider;
