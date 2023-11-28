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

/* Ultrasonic Sensor Pins */
const int trigPin = 5;  // GPIO5
const int echoPin = 4;  // GPIO4

/* Relay Control Pins */
const int relay1Pin = 14;  // GPIO14
const int relay2Pin = 12;  // GPIO12
const int relay3Pin = 13;  // GPIO13
const int relay4Pin = 15;  // GPIO15

/* LED Pin */
const int LEDpin = 2;  // GPIO2

/* Sensor and Relay Status */
float distance = 0.0;
bool relay1Status = LOW;
bool relay2Status = LOW;
bool relay3Status = LOW;
bool relay4Status = LOW;
int LEDstatus = LOW; 

ESP8266WebServer server(80);

void handle_OnConnect();
void handle_ledon();
void handle_ledoff();
void handle_getdistance();
void handle_relay1on();
void handle_relay1off();
void handle_relay2on();
void handle_relay2off();
void handle_relay3on();
void handle_relay3off();
void handle_relay4on();
void handle_relay4off();
void handle_NotFound();

void setup() {
  Serial.begin(115200);
  pinMode(LEDpin, OUTPUT);

  /* Ultrasonic Sensor Setup */
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  /* Relay Control Setup */
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  pinMode(relay3Pin, OUTPUT);
  pinMode(relay4Pin, OUTPUT);

  /* WiFi Setup */
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  /* Web Server Setup */
  server.on("/", HTTP_GET, handle_OnConnect);
  server.on("/ledon", HTTP_GET, handle_ledon);
  server.on("/ledoff", HTTP_GET, handle_ledoff);
  server.on("/getdistance", HTTP_GET, handle_getdistance);
  server.on("/relay1on", HTTP_GET, handle_relay1on);
  server.on("/relay1off", HTTP_GET, handle_relay1off);
  server.on("/relay2on", HTTP_GET, handle_relay2on);
  server.on("/relay2off", HTTP_GET, handle_relay2off);
  server.on("/relay3on", HTTP_GET, handle_relay3on);
  server.on("/relay3off", HTTP_GET, handle_relay3off);
  server.on("/relay4on", HTTP_GET, handle_relay4on);
  server.on("/relay4off", HTTP_GET, handle_relay4off);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void handle_OnConnect() {
  String html = SendHTML(LEDstatus);
  server.send(200, "text/html", html);
}

void handle_ledon() {
  LEDstatus = HIGH;
  String html = SendHTML(LEDstatus);
  server.send(200, "text/html", html);
}

void handle_ledoff() {
  LEDstatus = LOW;
  String html = SendHTML(LEDstatus);
  server.send(200, "text/html", html);
}

void handle_getdistance() {
  // Implement logic to get distance
  // Update 'distance' variable
}

void handle_relay1on() {
  // Implement logic to turn on relay 1
  relay1Status = HIGH;
}

void handle_relay1off() {
  // Implement logic to turn off relay 1
  relay1Status = LOW;
}

void handle_relay2on() {
  // Implement logic to turn on relay 2
  relay2Status = HIGH;
}

void handle_relay2off() {
  // Implement logic to turn off relay 2
  relay2Status = LOW;
}

void handle_relay3on() {
  // Implement logic to turn on relay 3
  relay3Status = HIGH;
}

void handle_relay3off() {
  // Implement logic to turn off relay 3
  relay3Status = LOW;
}

void handle_relay4on() {
  // Implement logic to turn on relay 4
  relay4Status = HIGH;
}

void handle_relay4off() {
  // Implement logic to turn off relay 4
  relay4Status = LOW;
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

void loop() {
  server.handleClient();

  /* Update distance from Ultrasonic Sensor */
  updateDistance();

  /* Update Relay Status */
  updateRelayStatus();

  /* Update LED Status */
  digitalWrite(LEDpin, relay1Status);  // LED status based on relay1
}

String SendHTML(uint8_t ledstat) {
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
  
  if (ledstat) {
    ptr +="<p>LED Status: ON</p><a class=\"button button-off\" href=\"/ledoff\">OFF</a>\n";
  } else {
    ptr +="<p>LED Status: OFF</p><a class=\"button button-on\" href=\"/ledon\">ON</a>\n";
  }
  
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

void updateDistance() {
  // Trigger pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the pulse duration on the echo pin
  long duration = pulseIn(echoPin, HIGH);

  // Calculate distance in centimeters
  // Speed of sound is approximately 343 meters/second or 0.0343 cm/microsecond
  // Divide by 2 because the sound travels to the object and back
  distance = duration * 0.0343 / 2.0;

  // Print the distance to Serial for debugging
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}

void updateRelayStatus() {
  // Example: Turn on Relay 1 if the distance is less than 50 cm
  if (distance < 50.0) {
    relay1Status = HIGH;
  } else {
    relay1Status = LOW;
  }

  // Add similar conditions for other relays based on your requirements
}
