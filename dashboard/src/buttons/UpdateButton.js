import React, { useState } from 'react';

function UpdateButton({ label, updateConfig }) {

    return (
        <div className="setting">
            <button onClick={updateConfig} style={{ color: "black", backgroundColor: '#38ffb9' }}>${label}</button>
        </div>
    );
}
export default UpdateButton;
