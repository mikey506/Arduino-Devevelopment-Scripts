#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AM2320.h>

/* Put your SSID & Password */
const char* ssid = "NodeMCU";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

const int RELAY1_PIN = 2;  // GPIO2 (D4)
const int RELAY2_PIN = 3;  // GPIO3 (D3)
const int RELAY3_PIN = 4;  // GPIO4 (D2)
const int RELAY4_PIN = 5;  // GPIO5 (D1)
const int RELAY5_PIN = 12; // GPIO12 (D6)
const int RELAY6_PIN = 13; // GPIO13 (D7)
const int RELAY7_PIN = 14; // GPIO14 (D5)
const int RELAY8_PIN = 15; // GPIO15 (D8)

const int SENSOR_PIN = 0;  // Analog pin A0

Adafruit_AM2320 am2320;

bool relayStatus[8] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};

void setup() {
  Serial.begin(115200);
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  pinMode(RELAY4_PIN, OUTPUT);
  pinMode(RELAY5_PIN, OUTPUT);
  pinMode(RELAY6_PIN, OUTPUT);
  pinMode(RELAY7_PIN, OUTPUT);
  pinMode(RELAY8_PIN, OUTPUT);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handle_OnConnect);
  server.on("/ledon", handle_ledon);
  server.on("/ledoff", handle_ledoff);
  server.on("/relay", handle_relay);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  updateRelays();
}

void handle_OnConnect() {
  String html = SendHTML();
  server.send(200, "text/html", html);
}

void handle_ledon() {
  setRelayStatus(0, true);
  String html = SendHTML();
  server.send(200, "text/html", html);
}

void handle_ledoff() {
  setRelayStatus(0, false);
  String html = SendHTML();
  server.send(200, "text/html", html);
}

void handle_relay() {
  if (server.hasArg("relay")) {
    int relayIndex = server.arg("relay").toInt();
    if (relayIndex >= 1 && relayIndex <= 8) {
      bool relayState = server.arg("state") == "on";
      setRelayStatus(relayIndex - 1, relayState);
    }
  }
  String html = SendHTML();
  server.send(200, "text/html", html);
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

void setRelayStatus(int index, bool state) {
  if (index >= 0 && index < 8) {
    relayStatus[index] = state;
    digitalWrite(getRelayPin(index), state ? HIGH : LOW);
  }
}

void updateRelays() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(getRelayPin(i), relayStatus[i] ? HIGH : LOW);
  }
}

int getRelayPin(int index) {
  switch (index) {
    case 0: return RELAY1_PIN;
    case 1: return RELAY2_PIN;
    case 2: return RELAY3_PIN;
    case 3: return RELAY4_PIN;
    case 4: return RELAY5_PIN;
    case 5: return RELAY6_PIN;
    case 6: return RELAY7_PIN;
    case 7: return RELAY8_PIN;
    default: return -1;
  }
}

String SendHTML() {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Greenhouse Automation</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +=".relay {display: block;width: 200px;margin: 10px auto;}\n";
  ptr +=".relay-state {display: inline-block;width: 120px;vertical-align: middle;}\n";
  ptr +=".relay-label {display: inline-block;width: 60px;vertical-align: middle;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Greenhouse Automation</h1>\n";
  
  for (int i = 0; i < 8; i++) {
    ptr += "<div class=\"relay\">\n";
    ptr += "<span class=\"relay-label\">Relay " + String(i + 1) + "</span>\n";
    ptr += "<span class=\"relay-state\">" + (relayStatus[i] ? "ON" : "OFF") + "</span>\n";
    ptr += "<a class=\"button button-on\" href=\"/relay?relay="Here's the continuation of the script:

+ String(i + 1) + "&state=on\">ON</a>\n";
    ptr += "<a class=\"button button-off\" href=\"/relay?relay=" + String(i + 1) + "&state=off\">OFF</a>\n";
    ptr += "</div>\n";
  }

  // Sensor data
  float temperature = am2320.readTemperature();
  float humidity = am2320.readHumidity();
  ptr += "<h3>Sensor Data</h3>\n";
  ptr += "<p><strong>Temperature:</strong> " + String(temperature) + "°C</p>\n";
  ptr += "<p><strong>Humidity:</strong> " + String(humidity) + "%</p>\n";

  // Debug/Information Section
  ptr += "<h3>Debug/Information Section</h3>\n";
  ptr += "<p><strong>CPU:</strong> " + String(ESP.getCpuFreqMHz()) + " MHz</p>\n";
  ptr += "<p><strong>Memory:</strong> " + String(ESP.getFreeHeap()) + " bytes (free)</p>\n";
  ptr += "<p><strong>Storage Space:</strong> " + String(ESP.getFlashChipRealSize()) + " bytes (total), " + String(ESP.getFlashChipSize() - ESP.getSketchSize()) + " bytes (free)</p>\n";
  ptr += "<p><strong>Firmware:</strong> " + String(ESP.getSketchSize()) + " bytes (size), " + String(ESP.getFreeSketchSpace()) + " bytes (available)</p>\n";

  // Networking Information
  ptr += "<p><strong>Device IP:</strong> " + WiFi.softAPIP().toString() + "</p>\n";
  ptr += "<p><strong>MAC Address:</strong> " + WiFi.softAPmacAddress() + "</p>\n";

  // Additional Hardware Information
  ptr += "<p><strong>Chip ID:</strong> " + String(ESP.getChipId()) + "</p>\n";
  ptr += "<p><strong>Core Version:</strong> " + ESP.getCoreVersion() + "</p>\n";
  ptr += "<p><strong>SDK Version:</strong> " + String(ESP.getSdkVersion()) + "</p>\n";
  ptr += "<p><strong>Boot Version:</strong> " + String(ESP.getBootVersion()) + "</p>\n";

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
