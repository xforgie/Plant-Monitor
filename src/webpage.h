
const char* webpage = R"======(
<!DOCTYPE html>
<html>
<head>
    <title>Plant Monitor</title>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f2f2f2;
            color: #333;
            /* transition: background-color 0.3s, color 0.3s; */
        }

        body.dark-mode {
            background-color: #333;
            color: #f2f2f2;
        }

        h1 {
            text-align: center;
        }

        .container {
            margin: auto;
            width: 75%;
            padding: 10px;
            background-color: white;
            border-radius: 10px;
            box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.3);
            /* transition: background-color 0.3s, box-shadow 0.3s; */
        }

        .container.dark-mode {
            background-color: #222;
            box-shadow: 0px 0px 10px rgba(255, 255, 255, 0.3);
        }

        .row {
            display: flex;
            align-items: center;
            justify-content: space-between;
            margin-bottom: 10px;
            box-shadow: 0px 0px 0px;
            transition: background-color 0.3s, box-shadow 0.3s;
        }

        .row:hover {
            box-shadow: 0px 0px 10px;
            transition: background-color 0.3s, box-shadow 0.3s;
        }

        .label {
            font-weight: bold;
            margin-right: 10px;
        }

        .value {
            font-size: 24px;
            font-weight: bold;
        }

        .btn {
            background-color: #4c81af;
            color: white;
            padding: 8px 16px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            float: right;
        }

        .btn:hover {
            background-color: #345877;
        }
    </style>
</head>

<body onload="loadDarkMode()">
    <!-- <button class="btn" onclick="toggleDarkMode()">Toggle Dark Mode</button> -->
    <div class="container">
        <h1 style="text-justify: center">Plant Monitor v0.0.1</h1>
        <div class="row">
            <div class="label">Temperature:</div>
            <div class="value" id="temperature">--</div>
        </div>
        <div class="row">
            <div class="label">Humidity:</div>
            <div class="value" id="humidity">--</div>
        </div>
        <div class="row">
            <div class="label">Soil Moisture:</div>
            <div class="value" id="moisture">--</div>
        </div>
        <div class="row">
            <div class="label">Water Level:</div>
            <div class="value" id="water_level">--</div>
        </div>
        <button class="btn" id="hold-me" style="float:left">Hold to water</button>
    </div>
    <script>

        setInterval(loadDoc, 1000);

        const b = document.getElementById("hold-me");
        let time;
        b.onpointerdown = function() {
            time = Date.now();
        }
        b.onpointerup = function() {
            console.log(`you held me down for ${Date.now() - time} milliseconds`);
        }

        function loadDoc() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    updateValues();
                    //document.getElementsByClassName("container").innerHTML = this.responseText;
                }
            };
            xhttp.open("GET", "/", true);
            xhttp.send();
        }

        // Toggle between light and dark mode
        function toggleDarkMode() {
            document.body.classList.toggle('dark-mode');
            document.querySelector('.container').classList.toggle('dark-mode');
            setDarkModeCookie();
        }

        // Set the dark mode cookie
        function setDarkModeCookie() {
            const darkMode = document.body.classList.contains('dark-mode');
            document.cookie = `darkMode=${darkMode}; SameSite=Lax; path=/; max-age=${60 * 60 * 24 * 365}`; // Set cookie to expire after 1 year
        }

        // Get the dark mode cookie value
        function getDarkModeCookie() {
            const cookie = document.cookie.split(';').find(c => c.trim().startsWith('darkMode='));
            if (cookie) {
                return cookie.split('=')[1] === 'true';
            }
            return false;
        }

        function loadDarkMode() {
            const darkModeEnabled = true;// getDarkModeCookie();
            if (darkModeEnabled) {
                document.body.classList.add('dark-mode');
                document.querySelector('.container').classList.add('dark-mode');
            }
        }

        function updateValues() {
            // Update the temperature and humidity values on the website
            let temperature = Math.floor(Math.random() * 41); // Generate a random temperature between 0 and 50 degrees Celsius
            const temperatureValue = document.getElementById('temperature');
            temperatureValue.textContent = `${temperature} °C`; // update the temperature value

            let humidity = Math.floor(Math.random() * 101);
            document.getElementById('humidity').textContent = `${humidity} %`;

            let moisture = Math.floor(Math.random() * 101);
            document.getElementById('moisture').textContent = `${moisture} %`;

            let water_level = 20.0;
            document.getElementById('water_level').textContent = `${water_level.toFixed(2)} L`;
        }


    </script>
</body>

</html>
     )======";