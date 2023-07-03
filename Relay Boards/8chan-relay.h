// Pinout Configuration
const int relayPins[] = {2, 3, 4, 5, 6, 7, 8, 9};  // GPIO pins connected to the relay board

// Relay Control Function
void controlRelay(int relayNumber, bool state) {
  if (relayNumber >= 1 && relayNumber <= 8) {  // Check if relay number is valid
    int relayPin = relayPins[relayNumber - 1];  // Map relay number to pin index
    digitalWrite(relayPin, state ? LOW : HIGH);  // Set relay pin state based on desired state
    Serial.print("Relay ");
    Serial.print(relayNumber);
    Serial.println(state ? " ON" : " OFF");
  }
}

// Setup Function
void setup() {
  Serial.begin(9600);  // Initialize serial communication
  while (!Serial);     // Wait for the serial monitor to open
  
  for (int i = 0; i < 8; i++) {
    pinMode(relayPins[i], OUTPUT);  // Set relay pins as OUTPUT
    controlRelay(i + 1, false);  // Turn off all relays initially
  }
}

// Main Program
void loop() {
  // Fast Interval: Toggle relay 1 on and off quickly 15 times
  for (int i = 0; i < 15; i++) {
    controlRelay(1, true);  // Turn on relay 1
    delay(200);
    controlRelay(1, false);  // Turn off relay 1
    delay(200);
  }
  
  delay(1000);  // Delay between intervals
  
  // Slow Interval: Toggle relay 1 on and off slowly 15 times
  for (int i = 0; i < 15; i++) {
    controlRelay(1, true);  // Turn on relay 1
    delay(1000);
    controlRelay(1, false);  // Turn off relay 1
    delay(1000);
  }
  
  delay(1000);  // Delay between intervals
}
