function fetchInitialSettings() {
    fetch('/sensitivity', { method: 'GET' })
        .then(response => response.text())
        .then(data => {
            sensitivityRange.value = data;
            sensitivityValue.textContent = data;
            showPopup(sensitivityRange, sensitivityPopup, data);
        })
        .catch(error => {
            console.error('Error fetching initial sensitivity:', error);
        });

    fetch('/brightness', { method: 'GET' })
        .then(response => response.text())
        .then(data => {
            brightnessRange.value = data;
            brightnessValue.textContent = data;
            showPopup(brightnessRange, brightnessPopup, data);
        })
        .catch(error => {
            console.error('Error fetching initial brightness:', error);
        });
}

window.onload = fetchInitialSettings;

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
    var formData = new FormData();
    formData.append('value', sensitivityRange.value);
    fetch('/sensitivity', { method: 'POST', body: formData })
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
    var formData = new FormData();
    formData.append('value', brightnessRange.value);
    fetch('/brightness', { method: 'POST', body: formData })
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
    fetch('/visualization?mode=' + mode, { method: 'GET' })
        .then(response => response.text())
        .then(data => {
            console.log(data);
        })
        .catch(error => {
            console.error('Error setting visualization mode:', error);
        });
}

document.getElementById('sendText').addEventListener('click', function () {
    var text = document.getElementById('customText').value;
    fetch('/scrollText', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'text=' + encodeURIComponent(text)
    })
        .then(response => response.text())
        .then(data => console.log(data))
        .catch(error => console.error('Error:', error));
});

document.getElementById('textColor').addEventListener('change', function () {
    var color = this.value;
    fetch('/setTextColor', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'color=' + encodeURIComponent(color)
    });
    console.log(color);
    console.log(encodeURIComponent(color));
});

document.getElementById('speedControl').addEventListener('change', function () {
    var speed = this.value;
    fetch('/setSpeed', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'speed=' + encodeURIComponent(speed)
    });
});

document.getElementById('toggleScroll').addEventListener('click', function () {
    var formData = new FormData();
    continuousScrolling = false;
    formData.append('enabled', continuousScrolling ? 'false' : 'true');
    fetch('/toggleContinuousScrolling', { method: 'POST', body: formData })
        .then(response => response.text())
        .then(data => {
            console.log(data);
            continuousScrolling = !continuousScrolling;
        })
        .catch(error => {
            console.error('Error:', error);
        });
});

document.getElementById('wifiSetup').addEventListener('click', function () {
    window.location.href = '/setup';
});

document.getElementById('wifiErase').addEventListener('click', function () {
    window.location.href = '/erase';
});


