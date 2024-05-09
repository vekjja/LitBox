
function SaveButton({ config, saveConfig }) {

    const handleSave = () => {
        saveConfig({ ...config });
    };

    return (
        <button onClick={handleSave} style={{ backgroundColor: 'brown' }}>Save</button>
    );
}
export default SaveButton;
