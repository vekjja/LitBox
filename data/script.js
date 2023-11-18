function showPopup(slider, popup, value) {
    var sliderRect = slider.getBoundingClientRect();
    var popupRect = popup.getBoundingClientRect();
    var thumbPosition = (slider.value / slider.max) * slider.offsetWidth;
    popup.style.left = (sliderRect.left + thumbPosition - popupRect.width / 2) + 'px';
    popup.style.top = (sliderRect.top - popupRect.height - 30) + 'px';
    popup.textContent = value;
    popup.style.display = 'block';
}

var sensitivityRange = document.getElementById('sensitivityRange');
var sensitivityValue = document.getElementById('sensitivityValue');
var sensitivityPopup = document.getElementById('sensitivityPopup');

sensitivityRange.addEventListener('input', function () {
    showPopup(sensitivityRange, sensitivityPopup, sensitivityRange.value);
    sensitivityValue.textContent = sensitivityRange.value;
});

sensitivityRange.addEventListener('change', function () {
    sensitivityPopup.style.display = 'none';
    sensitivityValue.textContent = sensitivityRange.value;
    fetch('/set/sensitivity?value=' + sensitivityRange.value, { method: 'GET' })
        .then(response => response.text())
        .then(data => {
            console.log(data);
        })
        .catch(error => {
            console.error('Error setting sensitivity:', error);
        });
});

var brightnessRange = document.getElementById('brightnessRange');
var brightnessValue = document.getElementById('brightnessValue');
var brightnessPopup = document.getElementById('brightnessPopup');

brightnessRange.addEventListener('input', function () {
    showPopup(brightnessRange, brightnessPopup, brightnessRange.value);
    brightnessValue.textContent = brightnessRange.value;
});

brightnessRange.addEventListener('change', function () {
    brightnessPopup.style.display = 'none';
    brightnessValue.textContent = brightnessRange.value;
    fetch('/set/brightness?value=' + brightnessRange.value, { method: 'GET' })
        .then(response => response.text())
        .then(data => {
            console.log(data);
        })
        .catch(error => {
            console.error('Error setting brightness:', error);
        });
});

var visualizationSelect = document.getElementById('visualizationSelect');

visualizationSelect.addEventListener('change', function () {
    var selectedValue = visualizationSelect.value;
    setVisualization(selectedValue);
});

function setVisualization(mode) {
    fetch('/set/visualization?mode=' + mode, { method: 'GET' })
        .then(response => response.text())
        .then(data => {
            console.log(data);
        })
        .catch(error => {
            console.error('Error setting visualization mode:', error);
        });
}

document.getElementById('wifiSetup').addEventListener('click', function () {
    window.location.href = '/setup';
});

document.getElementById('wifiErase').addEventListener('click', function () {
    window.location.href = '/erase';
});
