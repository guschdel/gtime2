

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locationOK(pos) {
  // URL bauen
  var url = "http://api.openweathermap.org/data/2.5/weather?lat=" + 
      pos.coords.latitude + "&lon=" + pos.coords.longitude;
  console.log("URL: " + url);
  
  xhrRequest(url, "GET",
              function(responseText) {
                var json = JSON.parse(responseText);
                // Convert Kelvin to °C
                var temperature = Math.round(json.main.temp - 273.15);
                console.log("Die Temperatur ist " + temperature);
                // Conditions
                //var conditions = json.waether[0].main;
                //console.log("Wetterbedingungen " + conditions);
                
                // Assemble dictionary using our keys
                var dictionary = {
                  "KEY_TEMPERATURE": temperature
                };
                // Send to Pebble
                Pebble.sendAppMessage(dictionary,
                                      function(e) {
                                        console.log("Weather info sent to Pebble successfully!");
                                      },
                                      function(e) {
                                        console.log("Error sending weather info to Pebble!");
                                      }
                                     );
              }
  );
}

function locationError(err) {
  console.log("Fehler beim Position bestimmen!");
}

function getWeather() {
  navigator.geolocation.getCurrentPosition(
    locationOK,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}

//Listen when the watchface is active
Pebble.addEventListener('ready',
                       function(e) {
                         console.log("Pebble JS ready!");
                         // Get the initial weather
                         getWeather();
                       }
);

// Listen when a message is received
Pebble.addEventListener('appmessage',
                       function(e) {
                         console.log("Message received!");
                         getWeather();
                       }
);