#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "YourSSID";     // Change this to your WiFi SSID
const char* password = "YourPassword"; // Change this to your WiFi password

const int trigPin = 0; // GPIO3
const int echoPin = 4; // GPIO2
// VCC to 5v & GND to GND

ESP8266WebServer server(80);

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);      
   
  Serial.begin(9600);
  Serial.println("Starting HC-SR04 Test...");  

  // Connect to WiFi
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IPAddress(192, 168, 1, 1), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);
  Serial.println("WiFi AP started");

  // Initialize web server
  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient(); // Handle web server client requests
  
  unsigned long pulseLength;
  unsigned int inches;
  unsigned int centimeters;
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  pulseLength = pulseIn(echoPin, HIGH);
  
  inches = pulseLength / 148; 
  centimeters = pulseLength / 58;  
                                    
  Serial.print("Distance = ");
  Serial.print(inches);
  Serial.print(" inches");
  Serial.print("       ");
  Serial.print(centimeters);
  Serial.println(" centimeters");
  delay(1000);
}

void handleRoot() {
  unsigned long pulseLength;
  unsigned int inches;
  unsigned int centimeters;
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  pulseLength = pulseIn(echoPin, HIGH);
  
  inches = pulseLength / 148; 
  centimeters = pulseLength / 58;  

  String page = "<html><head><meta http-equiv='refresh' content='1'><style>";
  page += "body {font-family: Arial, sans-serif;}";
  page += "h1 {font-size: 3em;}";
  page += "p {font-size: 2em;}";
  page += "</style></head><body>";
  page += "<h1>Distance</h1>";
  page += "<p>Inches: " + String(inches) + "</p>";
  page += "<p>Centimeters: " + String(centimeters) + "</p>";
  page += "</body></html>";

  server.send(200, "text/html", page);
}
