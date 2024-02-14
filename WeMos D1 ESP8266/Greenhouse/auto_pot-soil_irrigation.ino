#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>

/* WiFi credentials */
const char* ssid = "NodeMCU";
const char* password = "12345678";

/* IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

/* Relay Control Pins */
const int relayControlPin = 14;  // GPIO14 (D5)

/* LED Pin */
const int LEDpin = 2;  // GPIO2

/* Soil Moisture Sensor Pin */
const int moistureSensorPin = A0;  // Analog pin

/* Sensor and Relay Status */
bool relayStatus = LOW;
int LEDstatus = LOW;
int moistureLevel = 0;  // Variable to store moisture level

ESP8266WebServer server(80);
// Declare the SendHTML function
String SendHTML(uint8_t ledstat, int moisture);

void handle_OnConnect();
void handle_relayon();
void handle_relayoff();
void handle_NotFound();

void setup() {
  Serial.begin(9600); // Set baud rate to 9600

  pinMode(LEDpin, OUTPUT);
  pinMode(relayControlPin, OUTPUT);
  pinMode(moistureSensorPin, INPUT);

  /* WiFi Setup */
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  /* Web Server Setup */
  server.on("/", HTTP_GET, handle_OnConnect);
  server.on("/relayon", HTTP_GET, handle_relayon);
  server.on("/relayoff", HTTP_GET, handle_relayoff);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void handle_OnConnect() {
  String html = SendHTML(LEDstatus, moistureLevel);
  server.send(200, "text/html", html);
}

void handle_relayon() {
  relayStatus = HIGH;
  String html = SendHTML(LEDstatus, moistureLevel);
  server.send(200, "text/html", html);
}

void handle_relayoff() {
  relayStatus = LOW;
  String html = SendHTML(LEDstatus, moistureLevel);
  server.send(200, "text/html", html);
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

void loop() {
  server.handleClient();

  /* Update Relay Status */
  digitalWrite(relayControlPin, relayStatus);

  /* Update LED Status */
  digitalWrite(LEDpin, relayStatus);  // LED status based on relay

  /* Read Soil Moisture Level */
  int rawMoistureLevel = analogRead(moistureSensorPin);

  /* Map the raw reading to a scale of 1-5 */
  int moistureRating;
  if (rawMoistureLevel <= 333) {
    moistureRating = 1; // Super Moist
  } else if (rawMoistureLevel <= 700) {
    moistureRating = 2; // Lil Moist
  } else {
    moistureRating = 3; // Dry AF
  }

  /* Output Soil Moisture Level to Serial Monitor */
/*  Serial.print("Moisture Level (Raw): ");
  Serial.println(rawMoistureLevel); */
  Serial.print("Moisture Rating: ");
  switch (moistureRating) {
    case 1:
      Serial.println("Super Moist");
      break;
    case 2:
      Serial.println("Lil Moist");
      break;
    case 3:
      Serial.println("Dry AF");
      break;
  }

  /* Delay for 1 second */
  delay(1000);
}

String SendHTML(uint8_t ledstat, int moisture) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP8266 Web Server</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP8266 Web Server</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  
  ptr += "<p>Relay Status: ";
  if (relayStatus) {
    ptr += "ON</p><a class=\"button button-off\" href=\"/relayoff\">Turn Relay OFF</a>\n";
  } else {
    ptr += "OFF</p><a class=\"button button-on\" href=\"/relayon\">Turn Relay ON</a>\n";
  }

  // Display Moisture Level
  ptr += "<p>Moisture Level: " + String(moisture) + "</p>\n";

  // Debug/Information Section
  ptr +="<h3>Debug/Information Section</h3>\n";
  ptr +="<p><strong>CPU:</strong> " + String(ESP.getCpuFreqMHz()) + " MHz</p>\n";
  ptr +="<p><strong>Memory:</strong> " + String(system_get_free_heap_size()) + " bytes (free)</p>\n";
  ptr +="<p><strong>Storage Space:</strong> " + String(ESP.getFlashChipRealSize()) + " bytes (total), " + String(ESP.getFlashChipSize() - ESP.getSketchSize()) + " bytes (free)</p>\n";
  ptr +="<p><strong>Firmware:</strong> " + String(ESP.getSketchSize()) + " bytes (size), " + String(ESP.getFreeSketchSpace()) + " bytes (available)</p>\n";
  
  // Networking Information
  ptr +="<p><strong>Device IP:</strong> " + WiFi.softAPIP().toString() + "</p>\n";
  ptr +="<p><strong>MAC Address:</strong> " + WiFi.softAPmacAddress() + "</p>\n";
  
  // Additional Hardware Information
  ptr +="<p><strong>Chip ID:</strong> " + String(ESP.getChipId()) + "</p>\n";
  ptr +="<p><strong>Core Version:</strong> " + ESP.getCoreVersion() + "</p>\n";
  ptr +="<p><strong>SDK Version:</strong> " + String(ESP.getSdkVersion()) + "</p>\n";
  ptr +="<p><strong>Boot Version:</strong> " + String(ESP.getBootVersion()) + "</p>\n";
  
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
