
// setInterval(loadDoc, 1000);

// Get current sensor readings when the page loads
window.addEventListener('load', getReadings);

const b = document.getElementById("hold-me");
let time;
b.onpointerdown = function () {
    time = Date.now();
}
b.onpointerup = function () {
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

// Function to get current readings on the webpage when it loads for the first time
function getReadings() {
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var myObj = JSON.parse(this.responseText);
            console.log(myObj);
            document.getElementById('temperature').textContent = `${myObj.temperature} °C`; // update the temperature value
            document.getElementById('humidity').textContent = `${myObj.humidity} %`;
        }
    };
    xhr.open("GET", "/readings", true);
    xhr.send();
}

if (!!window.EventSource) {
    var source = new EventSource('/events');

    source.addEventListener('open', function (e) {
        console.log("Events Connected");
    }, false);

    source.addEventListener('error', function (e) {
        if (e.target.readyState != EventSource.OPEN) {
            console.log("Events Disconnected");
        }
    }, false);

    source.addEventListener('message', function (e) {
        console.log("message", e.data);
    }, false);

    source.addEventListener('new_readings', function (e) {
        console.log("new_readings", e.data);
        var myObj = JSON.parse(e.data);
        console.log(myObj);
        document.getElementById('temperature').textContent = `${myObj.temperature} °C`;
        document.getElementById('humidity').textContent = `${myObj.humidity} %`;
    }, false);
}
