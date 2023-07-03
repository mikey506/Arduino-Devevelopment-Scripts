// Pinout Configuration
const int relayPins[] = {2, 3, 4, 5, 6, 7, 8, 9};  // GPIO pins connected to the relay board
const int numRelays = sizeof(relayPins) / sizeof(relayPins[0]);

// Relay Control Function
void controlRelay(int relayNumber, bool state) {
  if (relayNumber >= 1 && relayNumber <= numRelays) {  // Check if relay number is valid
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
  
  for (int i = 0; i < numRelays; i++) {
    pinMode(relayPins[i], OUTPUT);  // Set relay pins as OUTPUT
    controlRelay(i + 1, false);  // Turn off all relays initially
  }
}

// Main Program
void loop() {
  for (int i = 0; i < numRelays; i++) {
    int relayNumber = i + 1;
    controlRelay(relayNumber, true);  // Turn on current relay
    delay(1000);
    controlRelay(relayNumber, false);  // Turn off current relay
  }
  
  delay(1000);  // Delay between cycles
}

