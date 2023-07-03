#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AM2320.h>

// Pinout Configuration
const int relayPins[] = {2, 3, 4, 5, 6, 7, 8, 9};  // GPIO pins connected to the relay board
const int numRelays = sizeof(relayPins) / sizeof(relayPins[0]);

// Thresholds
const float HIGH_TEMPERATURE_THRESHOLD = 30.0;  // High temperature threshold in Celsius
const float LOW_TEMPERATURE_THRESHOLD = 20.0;   // Low temperature threshold in Celsius
const float HIGH_HUMIDITY_THRESHOLD = 60.0;     // High humidity threshold in percentage
const float LOW_HUMIDITY_THRESHOLD = 40.0;      // Low humidity threshold in percentage

Adafruit_AM2320 am2320;
unsigned long previousTime = 0;
unsigned long interval = 30000;  // 30-second interval

// Relay Control Function
void controlRelay(int relayNumber, bool state, float temperature, float humidity) {
  if (relayNumber >= 1 && relayNumber <= numRelays) {  // Check if relay number is valid
    int relayPin = relayPins[relayNumber - 1];  // Map relay number to pin index
    digitalWrite(relayPin, state ? LOW : HIGH);  // Set relay pin state based on desired state
  }
}

// Function to print relay information
void printRelayInfo() {
  Serial.print("Relay Info: ");
  for (int i = 1; i <= numRelays; i++) {
    Serial.print("R");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(digitalRead(relayPins[i - 1]) == LOW ? "ON" : "OFF");
    Serial.print(" | ");
  }
  Serial.println();
}

// Setup Function
void setup() {
  Serial.begin(9600);  // Initialize serial communication
  while (!Serial);     // Wait for the serial monitor to open
  
  for (int i = 0; i < numRelays; i++) {
    pinMode(relayPins[i], OUTPUT);  // Set relay pins as OUTPUT
    controlRelay(i + 1, false, 0.0, 0.0);  // Turn off all relays initially
  }

  Wire.begin();
  am2320.begin();
}

// Main Program
void loop() {
  unsigned long currentTime = millis();

  if (currentTime - previousTime >= interval) {
    previousTime = currentTime;

    float temperature = am2320.readTemperature();
    float humidity = am2320.readHumidity();

    if (!isnan(temperature) && !isnan(humidity)) {
      // Output environment information
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.print(" °C");
      Serial.print("\tHumidity: ");
      Serial.print(humidity);
      Serial.println(" %");

      // Control relays based on temperature and humidity
      for (int i = 1; i <= numRelays; i++) {
        controlRelay(i, i == 1 && (temperature > HIGH_TEMPERATURE_THRESHOLD || humidity > HIGH_HUMIDITY_THRESHOLD) ||
                      i == 2 && temperature < LOW_TEMPERATURE_THRESHOLD ||
                      i == 3 && humidity < LOW_HUMIDITY_THRESHOLD, temperature, humidity);
      }

      // Print relay information
      printRelayInfo();
    } else {
      Serial.println("Failed to read data from AM2320 sensor.");
    }
  }
}
