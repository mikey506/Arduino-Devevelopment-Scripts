#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";

// Web server and ISS API
ESP8266WebServer server(80);
const char* issApi = "api.open-notify.org";

// ISS coordinates
float latitude = 0.0, longitude = 0.0;
unsigned long lastApiCall = 0;

// Fetch ISS location from Open Notify API
void fetchISSLocation() {
  WiFiClient http;
  if (!http.connect(issApi, 80)) {
    Serial.println("Failed to connect to ISS API!");
    return;
  }

  // Send HTTP GET request
  http.print("GET /iss-now.json HTTP/1.1\r\nHost: api.open-notify.org\r\nConnection: close\r\n\r\n");
  while (!http.available()) delay(10);

  // Skip HTTP headers
  if (http.find("\r\n\r\n")) {
    // Parse JSON response
    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, http);
    if (!error) {
      latitude = doc["iss_position"]["latitude"];
      longitude = doc["iss_position"]["longitude"];
      Serial.printf("ISS Position: Latitude %.4f, Longitude %.4f\n", latitude, longitude);
    } else {
      Serial.println("Failed to parse ISS API response!");
    }
  } else {
    Serial.println("Failed to find ISS API response headers!");
  }
  http.stop();
}

// Serve the HTML dashboard
void handleRoot() {
  server.send(200, "text/html", R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ISS Tracker</title>
  <style>
    html, body {
      margin: 0;
      padding: 0;
      height: 100%;
    }
    #map {
      height: 100%;
    }
  </style>
  <script>
    let map, marker;

    // Fetch ISS coordinates from the ESP8266's /iss-data endpoint
    const fetchISSData = () => {
      fetch('/iss-data')
        .then(response => response.json())
        .then(data => {
          const { lat, lon } = data;

          // Update marker position and re-center the map
          if (marker) {
            marker.setLatLng([lat, lon]);
            map.panTo([lat, lon]);
          } else {
            // Create marker if it doesn't exist
            marker = L.marker([lat, lon]).addTo(map);
            map.setView([lat, lon], 3); // Set initial view
          }

          console.log(`Updated ISS Position: Latitude ${lat}, Longitude ${lon}`);
        })
        .catch(err => console.error('Error fetching ISS data:', err));
    };

    window.onload = () => {
      // Initialize the map, center at [0, 0] with zoom level 2
      map = L.map('map').setView([0, 0], 2);

      // Add OpenStreetMap tile layer (HTTPS for modern browsers)
      L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
        maxZoom: 18,
        attribution: 'Map data © <a href="https://www.openstreetmap.org/">OpenStreetMap</a> contributors'
      }).addTo(map);

      // Fetch ISS data and update the map every 10 seconds
      setInterval(fetchISSData, 10000); // Fetch every 10 seconds
      fetchISSData(); // Initial fetch
    };
  </script>
  <!-- Leaflet.js styles and scripts -->
  <link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css" />
  <script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"></script>
</head>
<body>
  <div id="map"></div>
</body>
</html>
)rawliteral");
}

// Serve ISS coordinates as JSON
void handleISSData() {
  String jsonResponse = "{\"lat\": " + String(latitude, 6) + ", \"lon\": " + String(longitude, 6) + "}";
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", jsonResponse);
  Serial.println("Serving /iss-data: " + jsonResponse);
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected! IP: " + WiFi.localIP().toString());

  // Set up server routes
  server.on("/", handleRoot);
  server.on("/iss-data", handleISSData);
  server.begin();
}

void loop() {
  // Fetch ISS location every 10 seconds
  if (millis() - lastApiCall > 10000) {
    fetchISSLocation();
    lastApiCall = millis();
  }

  // Handle client requests
  server.handleClient();
}
