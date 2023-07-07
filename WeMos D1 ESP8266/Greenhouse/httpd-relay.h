#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Martine";     // Enter your WiFi SSID
const char* password = "martine123"; // Enter your WiFi password

ESP8266WebServer server(80);

int relayPins[] = {D0, D1, D2, D3};  // Pins for the relays
int relayCount = sizeof(relayPins) / sizeof(relayPins[0]);

bool relayStates[4] = {false};  // Initial states of the relays

int cpuUsage = 0;  // Placeholder for CPU usage
int memoryUsage = 0;  // Placeholder for memory usage
String storageInfo = "--";  // Placeholder for storage information
String firmwareVersion = "--";  // Placeholder for firmware version

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize the relays
  for (int i = 0; i < relayCount; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], HIGH);  // Set initial state to OFF
  }

  // Start the web server
  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();

  // Update system information here
  // Replace with your implementation for CPU usage, memory usage, storage, and firmware version
  cpuUsage = 50;
  memoryUsage = calculateMemoryUsage();
  storageInfo = "--";
  firmwareVersion = "--";
}

void handleRoot() {
  String html = "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";
  html += "<title>Greenhouse Automation Web Console</title>";
  html += "<style>";
  html += "body {";
  html += "  font-family: Arial, Helvetica, sans-serif;";
  html += "  background-color: white;";
  html += "}";
  html += "h1 {";
  html += "  text-align: center;";
  html += "}";
  html += "button {";
  html += "  font-size: 16px;";
  html += "  padding: 10px 20px;";
  html += "  margin: 10px;";
  html += "}";
  html += "</style>";
  html += "</head>";
  html += "<body>";
  html += "<h1>Greenhouse Automation Web Console</h1>";
  html += "<h2>Relay Control</h2>";
  
  for (int i = 0; i < relayCount; i++) {
    html += "<button onclick=\"toggleRelay(" + String(i) + ")\">Toggle Relay " + String(i+1) + "</button>";
  }
  
  html += "<script>";
  html += "function toggleRelay(relayIndex) {";
  html += "  var xhr = new XMLHttpRequest();";
  html += "  xhr.open('GET', '/toggle?relay=' + relayIndex, true);";
  html += "  xhr.send();";
  html += "}";
  html += "</script>";
  html += "</body>";
  html += "</html>";
  
  server.send(200, "text/html", html);
}


void handleToggle() {
  if (server.hasArg("relay")) {
    int relayIndex = server.arg("relay").toInt();
    
    if (relayIndex >= 0 && relayIndex < relayCount) {
      relayStates[relayIndex] = !relayStates[relayIndex];
      digitalWrite(relayPins[relayIndex], relayStates[relayIndex] ? LOW : HIGH);
      server.send(200, "text/html", "Relay toggled");
      return;
    }
  }
  
  server.send(400, "text/html", "Invalid request");
}

int calculateMemoryUsage() {
  // Calculate and return memory usage here
  // You can use ESP.getFreeHeap() to get the available free heap memory
  // Estimate the heap size based on the total RAM size of the ESP8266 module

  int freeHeap = ESP.getFreeHeap();
  int totalRAMSize = ESP.getFlashChipSize() - ESP.getSketchSize() - 0x100000;
  int usedHeap = totalRAMSize - freeHeap;
  int memoryUsagePercentage = usedHeap * 100 / totalRAMSize;

  return memoryUsagePercentage;
}
