import React, { useState, useEffect } from 'react';

function NewSettings() {
    const [newSetting, setNewSetting] = useState([]);

    useEffect(() => {
        fetch('/NEW ENDPOINT HERE')
            .then(response => response.json())
            .then(data => {
                setNewSetting(data);
            })
            .catch(error => console.error('Failed to load setting:', error));
    }, []);

    return (
        <div className="setting">
          // Add your setting here
            {newSetting}
        </div>
    );
}

export default NewSettings;
