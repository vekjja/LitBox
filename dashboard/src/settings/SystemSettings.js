import React, { useState } from 'react';

function SystemSettings({ config, updateConfig, saveConfig }) {
    const [ssid, setSSID] = useState(config.client.ssid);
    const [password, setPassword] = useState(config.client.password);


    const handleSave = () => {
        const newConfig = {
            ...config,
            client: {
                ssid: ssid,
                password: password
            }
        };
        updateConfig(newConfig);
        saveConfig(newConfig);
    }

    const restartAs = (mode) => {
        const newConfig = {
            ...config,
            mode: mode,
            client: {
                ssid: ssid,
                password: password
            }
        };
        updateConfig(newConfig);
        saveConfig(newConfig);
        fetch('/restart')
            .then(response => {
                if (response.ok) {
                    alert("System is Restarting as WiFi " + (mode === 'client' ? 'Client' : 'Access Point'));
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
            <button onClick={handleSave} style={{ color: "black", backgroundColor: '#38ffb9' }}>Save Current Config</button>
            <button onClick={() => restartAs('client')} style={{ backgroundColor: 'brown', marginTop: '10px' }}>Restart as Client</button>
            <button onClick={() => restartAs('ap')} style={{ backgroundColor: 'brown', marginTop: '10px' }}>Restart as Access Point</button>
        </div >
    );
}

export default SystemSettings;