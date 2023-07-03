#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AM2320.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

const int HIGH_TEMPERATURE_THRESHOLD = 30;
const int HIGH_HUMIDITY_THRESHOLD = 70;
const int LOW_TEMPERATURE_THRESHOLD = 15;
const int LOW_HUMIDITY_THRESHOLD = 40;

// Relay Pins
const int RELAY_1_PIN = 2;
const int RELAY_2_PIN = 3;
const int RELAY_3_PIN = 4;

Adafruit_AM2320 am2320;
AsyncWebServer server(80);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  am2320.begin();

  // Set up relay pins
  pinMode(RELAY_1_PIN, OUTPUT);
  pinMode(RELAY_2_PIN, OUTPUT);
  pinMode(RELAY_3_PIN, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin("YOUR_WIFI_SSID", "YOUR_WIFI_PASSWORD");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Set up web server routes
  server.on("/", handleRoot);
  server.on("/setThresholds", handleSetThresholds);

  // Start the server
  server.begin();

  // Print initialization complete message
  Serial.println("Initialization complete");
}

void loop() {
  // Read temperature and humidity
  float temperature = readTemperature();
  float humidity = readHumidity();
  Serial.println("-----------------------------------");
  // Print temperature and humidity information with trend
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C");
  printTrend(getTemperatureTrend(temperature));
  Serial.print("\tHumidity: ");
  Serial.print(humidity);
  Serial.print(" %");
  printTrend(getHumidityTrend(humidity));
  Serial.println();

  // Print relay state information
  printRelayState();

  // Check for high temperature or humidity
  if (temperature > HIGH_TEMPERATURE_THRESHOLD || humidity > HIGH_HUMIDITY_THRESHOLD) {
    Serial.println("Last Relay Change: High temperature or humidity detected!");
  }

  delay(10000); // Delay for 10 seconds
}

void printTrend(float value) {
  if (value > 0) {
    Serial.print(" (+)");
  } else if (value < 0) {
    Serial.print(" (-)");
  } else {
    Serial.print(" (=)");
  }
}

float getTemperatureTrend(float currentTemperature) {
  // Code to calculate temperature trend goes here
  // Replace with actual temperature trend calculation logic
  return random(-1, 2); // Returns -1, 0, or 1 randomly
}

float getHumidityTrend(float currentHumidity) {
  // Code to calculate humidity trend goes here
  // Replace with actual humidity trend calculation logic
  return random(-1, 2); // Returns -1, 0, or 1 randomly
}

void printRelayState() {
  // Print relay state information
  Serial.print("Relay State: ");
  Serial.print("R1: ");
  Serial.print(digitalRead(RELAY_1_PIN) == HIGH ? "ON" : "OFF");
  Serial.print(" / R2: ");
  Serial.print(digitalRead(RELAY_2_PIN) == HIGH ? "ON" : "OFF");
  Serial.print(" / R3: ");
  Serial.print(digitalRead(RELAY_3_PIN) == HIGH ? "ON" : "OFF");
  Serial.println();
}

float readTemperature() {
  return am2320.readTemperature();
}

float readHumidity() {
  return am2320.readHumidity();
}

void handleRoot(AsyncWebServerRequest *request) {
  // Build the HTML response with sensor and relay information
  String html = "<html><body>";
  html += "<h1>Sensor and Relay Information</h1>";
  html += "<h2>Temperature: " + String(readTemperature()) + " °C</h2>";
  html += "<h2>Humidity: " + String(readHumidity()) + " %</h2>";
  html += "<h2>Relay State:</h2>";
  html += "<p>R1: " + String(digitalRead(RELAY_1_PIN) == HIGH ? "ON" : "OFF") + "</p>";
  html += "<p>R2: " + String(digitalRead(RELAY_2_PIN) == HIGH ? "ON" : "OFF") + "</p>";
  html += "<p>R3: " + String(digitalRead(RELAY_3_PIN) == HIGH ? "ON" : "OFF") + "</p>";
  html += "<h2>Thresholds:</h2>";
  html += "<p>High Temperature: " + String(HIGH_TEMPERATURE_THRESHOLD) + "</p>";
  html += "<p>High Humidity: " + String(HIGH_HUMIDITY_THRESHOLD) + "</p>";
  html += "<p>Low Temperature: " + String(LOW_TEMPERATURE_THRESHOLD) + "</p>";
  html += "<p>Low Humidity: " + String(LOW_HUMIDITY_THRESHOLD) + "</p>";
  html += "</body></html>";

  // Send the response
  request->send(200, "text/html", html);
}

void handleSetThresholds(AsyncWebServerRequest *request) {
  // Read the new threshold values from the request parameters
  if (request->hasParam("highTemp") && request->hasParam("highHumidity") &&
      request->hasParam("lowTemp") && request->hasParam("lowHumidity")) {
    HIGH_TEMPERATURE_THRESHOLD = request->getParam("highTemp")->value().toInt();
    HIGH_HUMIDITY_THRESHOLD = request->getParam("highHumidity")->value().toInt();
    LOW_TEMPERATURE_THRESHOLD = request->getParam("lowTemp")->value().toInt();
    LOW_HUMIDITY_THRESHOLD = request->getParam("lowHumidity")->value().toInt();

    // Send a success message
    request->send(200, "text/plain", "Thresholds updated successfully");
  } else {
    // Send an error message if the request parameters are missing
    request->send(400, "text/plain", "Invalid request parameters");
  }
}
