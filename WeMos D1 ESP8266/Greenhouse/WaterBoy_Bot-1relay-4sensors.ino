/* Relay Control Pin:    D5 (GPIO14) */
/* Soil Moisture Sensor: */
/*  - Sensor 1:          D1 (GPIO5) */
/*  - Sensor 2:          D2 (GPIO4) */
/*  - Sensor 3:          D3 (GPIO0) */
/*  - Sensor 4:          D4 (GPIO2) */

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>

/* WiFi credentials */
String ssid = "NodeMCU";
String password = "12345678";

/* IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

/* Relay Control Pins */
const int relayControlPin = 14;  // GPIO14 (D5)

/* LED Pin */
const int LEDpin = 2;  // GPIO2

/* Soil Moisture Sensor Pins */
const int moistureSensorPins[] = {A0, A1, A2, A3};  // Analog pins for 4 sensors
const int numSensors = 4;

/* Sensor and Relay Status */
bool relayStatus = LOW;
int LEDstatus = LOW;
int moistureLevels[numSensors] = {0};  // Array to store moisture levels for each sensor

bool autoWateringEnabled = false;
int soilTarget = 500; // Soil moisture target
int targetRange = 150; // Target range (+/-)

int pumpDuration = 3; // Pump duration in seconds

ESP8266WebServer server(80);

// Function prototypes
void loadConfig();
void saveConfig();
void reboot();
void handleAutoWatering();

void handle_OnConnect();
void handle_relayon();
void handle_relayoff();
void handle_SaveConfig();
void handle_NotFound();

void setup() {
  Serial.begin(9600); // Set baud rate to 9600

  // Load configuration
  loadConfig();

  pinMode(LEDpin, OUTPUT);
  pinMode(relayControlPin, OUTPUT);
  for (int i = 0; i < numSensors; i++) {
    pinMode(moistureSensorPins[i], INPUT);
  }

  /* WiFi Setup */
  WiFi.softAP(ssid.c_str(), password.c_str());
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  /* Web Server Setup */
  server.on("/", HTTP_GET, handle_OnConnect);
  server.on("/saveconfig", HTTP_POST, handle_SaveConfig);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void handle_OnConnect() {
  String html = SendHTML(LEDstatus, moistureLevels[0]); // Sending moisture level of first sensor for demonstration, you can modify accordingly
  server.send(200, "text/html", html);
}

void handle_SaveConfig() {
  if (server.args() > 0) {
    for (int i = 0; i < server.args(); i++) {
      // Handling configuration parameters
    }
    saveConfig(); // Save configuration to file
    reboot(); // Reboot the device after updating configuration
  }
  server.send(200, "text/plain", "Configuration saved successfully");
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

void loop() {
  server.handleClient();

  /* Read Soil Moisture Levels */
  int totalMoisture = 0;
  for (int i = 0; i < numSensors; i++) {
    moistureLevels[i] = digitalRead(moistureSensorPins[i]);
    totalMoisture += moistureLevels[i];
  }
  int averageMoisture = totalMoisture / numSensors;

  /* Handle Auto Watering */
  handleAutoWatering(averageMoisture);

  /* Output Soil Moisture Level to Serial Monitor */
  Serial.print("Average Moisture Level: ");
  Serial.println(averageMoisture);

  /* Delay for 1 second */
  delay(1000);
}

unsigned long lastWateringTime = 0; // Variable to store the last time the water pump was activated
unsigned long elapsedTime = 0; // Variable to track the elapsed time since the last watering event
const unsigned long wateringCooldown = 180000; // 3 minutes in milliseconds

void handleAutoWatering(int averageMoisture) {
  // Check if auto watering is enabled
  if (autoWateringEnabled) {
    // Calculate elapsed time since last watering event
    elapsedTime = millis() - lastWateringTime;

    // Check if moisture level is below target range and cooldown time has passed
    if (averageMoisture < (soilTarget - targetRange) && elapsedTime >= wateringCooldown) {
      // Turn on pump
      relayStatus = HIGH;
      digitalWrite(relayControlPin, relayStatus);
      delay(pumpDuration * 1000); // Pump for specified duration
      relayStatus = LOW; // Turn off pump
      digitalWrite(relayControlPin, relayStatus);
      lastWateringTime = millis(); // Update last watering time
    } 
  }
}

String SendHTML(uint8_t ledstat, int moisture) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>WaterBoy Bot - Dashboard</title>\n";
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
  ptr +="<h1>WaterBoy Bot - “Now that’s what I call high-quality H2O.”</h1>\n";
  ptr +="This program is designed for automated plant watering using an ESP8266 microcontroller and 4 soil moisture sensors. It utilizes soil moisture sensors to monitor the moisture level of the soil and activates a water pump when the moisture level falls below a specified threshold. The program includes functionalities to configure settings such as WiFi credentials, IP address details, auto-watering enable/disable, soil moisture target level, target range, and pump duration through a web interface.<br>\n";
  
  // Display Moisture Level
  ptr += "<p>Moisture Level: " + String(moisture) + "</p>\n";

  // Display Auto Watering Status
  ptr += "<p>Auto Watering: ";
  if (autoWateringEnabled) {
    ptr += "Enabled</p>\n";
  } else {
    ptr += "Disabled</p>\n";
  }

  // Configurations Form
  ptr += "<form action=\"/saveconfig\" method=\"post\">";
  ptr += "<h3>Configuration</h3>";
  ptr += "<label>SSID:</label><input type=\"text\" name=\"ssid\" value=\"" + ssid + "\"><br>";
  ptr += "<label>Password:</label><input type=\"password\" name=\"password\" value=\"" + password + "\"><br>";
  ptr += "<label>IP Address:</label><input type=\"text\" name=\"ip\" value=\"" + WiFi.softAPIP().toString() + "\"><br>";
  ptr += "<label>Gateway:</label><input type=\"text\" name=\"gateway\" value=\"" + gateway.toString() + "\"><br>";
  ptr += "<label>Subnet Mask:</label><input type=\"text\" name=\"subnet\" value=\"" + subnet.toString() + "\"><br>";
  ptr += "<label>Auto Watering:</label><input type=\"checkbox\" name=\"autoWatering\" value=\"1\"";
  if (autoWateringEnabled) ptr += " checked";
  ptr += "><br>";
  ptr += "<label>Soil Target:</label><input type=\"number\" name=\"soilTarget\" value=\"" + String(soilTarget) + "\"><br>";
  ptr += "<label>Target Range:</label><input type=\"number\" name=\"targetRange\" value=\"" + String(targetRange) + "\"><br>";
  ptr += "<label>Pump Duration:</label><input type=\"number\" name=\"pumpDuration\" value=\"" + String(pumpDuration) + "\"><br>";
  ptr += "<input type=\"submit\" value=\"Save Config\">";
  ptr += "</form>";

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


void saveConfig() {
  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return;
  }

  // Create JSON object
  DynamicJsonDocument json(1024);
  json["ssid"] = ssid;
  json["password"] = password;
  json["ip"] = WiFi.softAPIP().toString();
  json["gateway"] = gateway.toString();
  json["subnet"] = subnet.toString();
  json["autoWateringEnabled"] = autoWateringEnabled;
  json["soilTarget"] = soilTarget;
  json["targetRange"] = targetRange;
  json["pumpDuration"] = pumpDuration;

  // Serialize JSON to file
  if (serializeJson(json, configFile) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  configFile.close();
}
void loadConfig() {
  if (SPIFFS.begin()) {
    Serial.println("File system mounted");

    if (SPIFFS.exists("/config.json")) {
      Serial.println("Reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        size_t size = configFile.size();
        std::unique_ptr<char[]> buf(new char[size]);
        configFile.readBytes(buf.get(), size);
        DynamicJsonDocument json(1024);
        DeserializationError error = deserializeJson(json, buf.get());
        if (!error) {
          ssid = json["ssid"].as<String>();
          password = json["password"].as<String>();
          local_ip.fromString(json["ip"].as<String>());
          gateway.fromString(json["gateway"].as<String>());
          subnet.fromString(json["subnet"].as<String>());
          autoWateringEnabled = json["autoWateringEnabled"].as<bool>();
          soilTarget = json["soilTarget"].as<int>();
          targetRange = json["targetRange"].as<int>();
          pumpDuration = json["pumpDuration"].as<int>();
        } else {
          Serial.println("Failed to parse config file");
        }
      } else {
        Serial.println("Failed to open config file");
      }
    } else {
      Serial.println("Config file not found");
    }
  } else {
    Serial.println("Failed to mount file system");
  }
}


void reboot() {
  ESP.restart(); // Reboot the device
}
