import React, { useState } from 'react';
import SaveButton from '../buttons/SaveButton';

function WiFiSettings({ config, saveConfig }) {
    const [ssid, setSSID] = useState(config.client.ssid);
    const [password, setPassword] = useState(config.client.password);


    const restartAsClient = () => {
        const newConfig = { ...config, mode: 'client' };
        restart(newConfig);
    }
    const restartAsAccessPoint = () => {
        const newConfig = { ...config, mode: 'ap' };
        restart(newConfig);
    }

    const restart = (newConfig) => {
        saveConfig(newConfig);
        fetch('/restart', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(newConfig)
        })
            .then(response => {
                if (response.ok) {
                    alert('System is restarting as client...');
                } else {
                    alert('Failed to restart system.');
                }
            })
            .catch(error => {
                console.error('Error restarting system:', error);
                alert('Error restarting system');
            });
    };

    return (
        <div class="setting">
            <div className="setting" id="color-pallet-settings">
                <input id="toggleColorPallet" type="checkbox" className="toggle-button"></input>
                <label htmlFor="toggleColorPallet" className="clickable">WiFi</label>
                <div className="toggle-content">
                    <div className="setting">
                        <label>SSID</label>
                        <input value={ssid} onChange={e => setSSID(e.target.value)} />
                        <label>Password</label>
                        <input value={password} onChange={e => setPassword(e.target.value)} />
                    </div>
                </div>
            </div>
            <SaveButton config={config} saveConfig={saveConfig} />
            <button onClick={restartAsClient} style={{ backgroundColor: 'brown', marginTop: '10px' }}>Restart as Client</button>
            <button onClick={restartAsAccessPoint} style={{ backgroundColor: 'brown', marginTop: '10px' }}>Restart as Access Point</button>
        </div >
    );
}

export default WiFiSettings;