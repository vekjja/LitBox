function fetchInitialSettings() {
    fetch('/sensitivity', { method: 'GET' })
        .then(response => response.text())
        .then(data => {
            var sensitivityValue = parseInt(data, 10);
            sensitivitySlider.value = sensitivityValue; // Update the slider's value
        })
        .catch(error => {
            console.error('Error fetching initial sensitivity:', error);
        });

    fetch('/brightness', { method: 'GET' })
        .then(response => response.text())
        .then(data => {
            var brightnessValue = parseInt(data, 10);
            brightnessSlider.value = brightnessValue; // Update the slider's value
        })
        .catch(error => {
            console.error('Error fetching initial brightness:', error);
        });

    fetch('/frameRate', { method: 'GET' })
        .then(response => response.text())
        .then(data => {
            var framerateValue = parseInt(data, 10);
            framerateSlider.value = framerateValue; // Update the slider's value
        })
        .catch(error => {
            console.error('Error fetching initial framerate:', error);
        });

}

function fetchBirdSettings() {
    fetch('/birds', { method: 'GET' })
        .then(response => response.text())
        .then(data => {
            var settings = data.split('\n');
            settings.forEach(setting => {
                var parts = setting.split('=');
                var key = parts[0];
                var value = parts[1];
                switch (key) {
                    case 'birdCount':
                        document.getElementById('birdCount').value = value;
                        break;
                    case 'birdAlignment':
                        document.getElementById('birdAlignment').value = value;
                        break;
                    case 'birdCohesion':
                        document.getElementById('birdCohesion').value = value;
                        break;
                    case 'birdSeparation':
                        document.getElementById('birdSeparation').value = value;
                        break;
                    case 'birdVerticalBounds':
                        document.querySelector('#birdVerticalBounds').checked = (value === 'true');
                        break;
                    case 'birdHorizontalBounds':
                        document.querySelector('#birdHorizontalBounds').checked = (value === 'true');
                        break;
                }
            });
        })
        .catch(error => {
            console.error('Error fetching bird settings:', error);
        });
}

window.onload = function () {
    fetchInitialSettings();
    fetchBirdSettings();
}

document.querySelectorAll('.toggle-button').forEach(function (toggle) {
    var contentId = toggle.id;
    // Load the saved state
    toggle.checked = localStorage.getItem(contentId) === 'true';
    // Save state on change
    toggle.addEventListener('click', function () {
        localStorage.setItem(contentId, toggle.checked);
    });
});

var sensitivitySlider = document.getElementById('sensitivityValue');
sensitivitySlider.addEventListener('change', function () {
    var formData = new FormData();
    formData.append('sensitivity', sensitivitySlider.value);
    fetch('/sensitivity', { method: 'POST', body: formData })
        .then(response => response.text())
        .then(data => {
            console.log(data);
        })
        .catch(error => {
            console.error('Error setting sensitivity:', error);
        });
});

var brightnessSlider = document.getElementById('brightnessValue');
brightnessSlider.addEventListener('change', function () {
    var formData = new FormData();
    formData.append('brightness', brightnessSlider.value);
    fetch('/brightness', { method: 'POST', body: formData })
        .then(response => response.text())
        .then(data => {
            console.log(data);
        })
        .catch(error => {
            console.error('Error setting brightness:', error);
        });
});

var frameRateSlider = document.getElementById('frameRateValue');
frameRateSlider.addEventListener('change', function () {
    var formData = new FormData();
    formData.append('frameRate', frameRateSlider.value);
    fetch('/frameRate', { method: 'POST', body: formData })
        .then(response => response.text())
        .then(data => {
            console.log("Framerate updated: " + data);
        })
        .catch(error => {
            console.error('Error setting frame rate:', error);
        });
});

var visualizationSelect = document.getElementById('visualizationSelect');
visualizationSelect.addEventListener('change', function () {
    var visualization = visualizationSelect.value;

    var birdSettings = document.getElementById('bird-settings');
    birdSettings.style.display = 'none';

    var leavesSettings = document.getElementById('leaves-settings');
    leavesSettings.style.display = 'none';

    var textSettings = document.getElementById('text-settings');
    textSettings.style.display = 'none';

    var audioSettings = document.getElementById('audio-settings');
    audioSettings.style.display = 'none';

    var animationSettings = document.getElementById('animation-settings');
    animationSettings.style.display = 'none';

    setVisualization(visualization);
    switch (visualization) {
        case 'bars':
            audioSettings.style.display = 'block';
            break;
        case 'birds':
            birdSettings.style.display = 'block';
            animationSettings.style.display = 'block';
            break;
        case 'circles':
            audioSettings.style.display = 'block';
            break;
        case 'leaves':
            leavesSettings.style.display = 'block';
            animationSettings.style.display = 'block';
            break;
        case 'gameOfLife':
            animationSettings.style.display = 'block';
            break;
        case 'text':
            textSettings.style.display = 'block';
            break;
    }
});

function setVisualization(visualization) {
    fetch('/visualization?visualization=' + visualization, { method: 'POST' })
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
    var textColor = document.getElementById('textColor').value;
    var textSpeed = document.getElementById('textSpeed').value;
    var textAnimation = document.getElementById('textAnimation').value;
    console.log('Sending text:', text, textColor, textSpeed);

    fetch('/text', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'text=' + encodeURIComponent(text)
            + '&textColor=' + encodeURIComponent(textColor)
            + '&textSpeed=' + encodeURIComponent(textSpeed)
            + '&textAnimation=' + encodeURIComponent(textAnimation)
    })
        .then(response => response.text())
        .then(data => console.log('Text and speed updated:', data))
        .catch(error => console.error('Error:', error));
});

document.getElementById('updateBirds').addEventListener('click', function () {
    var birdCount = document.getElementById('birdCount').value;
    var birdAlignment = document.getElementById('birdAlignment').value;
    var birdCohesion = document.getElementById('birdCohesion').value;
    var birdSeparation = document.getElementById('birdSeparation').value;
    var birdVerticalBounds = document.getElementById('birdVerticalBounds').checked;
    var birdHorizontalBounds = document.getElementById('birdHorizontalBounds').checked;

    fetch('/birds', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'birdCount=' + birdCount
            + '&birdAlignment=' + birdAlignment
            + '&birdCohesion=' + birdCohesion
            + '&separation=' + birdSeparation
            + '&birdVerticalBounds=' + birdVerticalBounds
            + '&birdHorizontalBounds=' + birdHorizontalBounds
    })
        .then(response => response.text())
        .then(data => console.log(data))
        .catch(error => console.error('Error:', error));
});

document.getElementById('wifiSetup').addEventListener('click', function () {
    window.location.href = '/setup';
});

document.getElementById('wifiErase').addEventListener('click', function () {
    window.location.href = '/erase';
});


