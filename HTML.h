const char indexHTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>

<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>LED Matrix Web Controls</title>
    <style>
        body {
            background-color: #111;
            font-family: Arial, Helvetica, Sans-Serif;
            text-align: center;
            margin: 0;
            padding: 0;
            color: #fff;
        }

        .container {
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            height: 100vh;
        }

        .header {
            font-size: 36px;
            margin-bottom: 20px;
        }

        .button-container {
            background-color: #111;
            padding: 20px;
            border-radius: 30px;
            margin: 20px;
            transition: box-shadow 0.3s ease;
        }

        .button {
            background-color: #444;
            border: none;
            color: white;
            padding: 15px 32px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            margin: 4px 2px;
            cursor: pointer;
            border-radius: 12px;
            transition: background-color 0.3s ease;
        }

        .button:hover {
            background-color: #555;
        }

        input[type=range] {
            -webkit-appearance: none;
            margin: 18px 0;
            width: 100%;
        }

        input[type=range]:focus {
            outline: none;
        }

        input[type=range]::-webkit-slider-runnable-track {
            width: 100%;
            height: 8.4px;
            cursor: pointer;
            animate: 0.2s;
            box-shadow: 1px 1px 1px #000000;
            background: #3071a9;
            border-radius: 1.3px;
            border: 0.2px solid #010101;
        }

        input[type=range]::-webkit-slider-thumb {
            box-shadow: 1px 1px 1px #000000;
            border: 1px solid #000000;
            height: 36px;
            width: 16px;
            border-radius: 3px;
            background: #ffffff;
            cursor: pointer;
            -webkit-appearance: none;
            margin-top: -14px;
        }

        input[type=range]:focus::-webkit-slider-runnable-track {
            background: #367ebd;
        }

        input[type=range]::-moz-range-track {
            width: 100%;
            height: 8.4px;
            cursor: pointer;
            animate: 0.2s;
            box-shadow: 1px 1px 1px #000000;
            background: #3071a9;
            border-radius: 1.3px;
            border: 0.2px solid #010101;
        }

        input[type=range]::-moz-range-thumb {
            box-shadow: 1px 1px 1px #000000;
            border: 1px solid #000000;
            height: 36px;
            width: 16px;
            border-radius: 3px;
            background: #ffffff;
            cursor: pointer;
        }

        input[type=range]::-ms-track {
            width: 100%;
            height: 8.4px;
            cursor: pointer;
            animate: 0.2s;
            background: transparent;
            border-color: transparent;
            border-width: 16px 0;
            color: transparent;
        }

        input[type=range]::-ms-fill-lower {
            background: #2a6495;
            border: 0.2px solid #010101;
            border-radius: 2.6px;
            box-shadow: 1px 1px 1px #000000;
        }

        input[type=range]::-ms-fill-upper {
            background: #3071a9;
            border: 0.2px solid #010101;
            border-radius: 2.6px;
            box-shadow: 1px 1px 1px #000000;
        }

        input[type=range]::-ms-thumb {
            margin-top: 1px;
            box-shadow: 1px 1px 1px #000000;
            border: 1px solid #000000;
            height: 36px;
            width: 16px;
            border-radius: 3px;
            background: #ffffff;
            cursor: pointer;
        }

        input[type=range]:focus::-ms-fill-lower {
            background: #3071a9;
        }

        input[type=range]:focus::-ms-fill-upper {
            background: #367ebd;
        }

        .info-container {
            position: fixed;
            bottom: 20px;
            left: 0;
            right: 0;
            text-align: center;
        }

        .modal {
            display: none;
            position: fixed;
            z-index: 1;
            left: 0;
            top: 0;
            width: 100%;
            height: 100%;
            overflow: auto;
            background-color: rgb(0, 0, 0);
            background-color: rgba(0, 0, 0, 0.4);
            padding-top: 60px;
        }

        .modal-content {
            background-color: #fefefe;
            margin: 5% auto;
            padding: 20px;
            border: 1px solid #888;
            width: 80%;
            color: #000;
        }

        .close {
            color: #aaa;
            float: right;
            font-size: 28px;
            font-weight: bold;
        }

        .close:hover,
        .close:focus {
            color: black;
            text-decoration: none;
            cursor: pointer;
        }
        .wifi-container {
            position: fixed;
            bottom: 10px;
            left: 0;
            right: 0;
            text-align: center;
        }

        .wifi-button {
            background-color: #444;
            border: none;
            color: white;
            padding: 10px 20px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 14px;
            margin: 4px 2px;
            cursor: pointer;
            border-radius: 8px;
            transition: background-color 0.3s ease;
        }

        .wifi-button:hover {
            background-color: #555;
        }
    </style>
</head>

<body>
    <div class="container">
        <div class="header">Spectral Analyzer Controls</div>
        <div class="button-container">
            <label for="sensitivityRange">Sensitivity: <span id="sensitivityValue">6</span></label>
            <input type="range" id="sensitivityRange" min="1" max="100" step="1" value="5">
            <button id="setSensitivityButton" class="button">Set Sensitivity</button>
            <br>  
            <button id="visMode0" class="button">Lines</button>
            <button id="visMode1" class="button">Circles</button>
            <br>
            <br>
            <button id="toggleScalingButton" class="button">Toggle Scaling</button>
            <br>
        </div>
    </div>
    <div class="wifi-container">
        <button id="wifiSetup" class="wifi-button">WiFi Setup</button>
        <button id="wifiErase" class="wifi-button">WiFi Erase</button>
    </div>

    <script>
        var sensitivityRange = document.getElementById('sensitivityRange');
        var sensitivityValue = document.getElementById('sensitivityValue');
        var setSensitivityButton = document.getElementById('setSensitivityButton');

        sensitivityRange.addEventListener('input', function () {
            sensitivityValue.textContent = sensitivityRange.value;
        });

        setSensitivityButton.addEventListener('click', function () {
            var newSensitivity = sensitivityRange.value;
            fetch('/set/sensitivity?value=' + newSensitivity, { method: 'GET' })
                .then(response => response.text())
                .then(data => {
                    console.log(data);
                })
                .catch(error => {
                    console.error('Error setting sensitivity:', error);
                });
        });

        document.getElementById('visMode0').addEventListener('click', function () {
          setVisualization(0);
        });
        document.getElementById('visMode1').addEventListener('click', function () {
            setVisualization(1);
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
        function updateScalingButtonText(isScalingOn) {
            toggleScalingButton.textContent = isScalingOn ? "Toggle Scaling Off" : "Toggle Scaling On";
        }
        
        toggleScalingButton.addEventListener('click', function () {
            fetch('/toggle/scaling', { method: 'GET' })
                .then(response => response.text())
                .then(data => {
                    var isScalingOn = data.includes("ON");
                    updateScalingButtonText(isScalingOn);
                    console.log(data);
                })
                .catch(error => {
                    console.error('Error toggling scaling:', error);
                });
        });

        // Initial check for the current state of scaling
        fetch('/get/scaling', { method: 'GET' })
            .then(response => response.text())
            .then(data => {
                var isScalingOn = data.includes("ON");
                updateScalingButtonText(isScalingOn);
        })
        .catch(error => {
            console.error('Error fetching scaling state:', error);
        });
        
        document.getElementById('wifiSetup').addEventListener('click', function () {
            window.location.href = '/setup';
        });
            document.getElementById('wifiErase').addEventListener('click', function () {
            window.location.href = '/erase';
        });
    </script>
</body>

</html>
)rawliteral";