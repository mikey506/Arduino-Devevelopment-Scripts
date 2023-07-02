// Pinout mapping
const int totalPins = 20;  // Total number of GPIO pins on Arduino Uno

void setup() {
  Serial.begin(9600);  // Initialize serial communication

  // Set all pins as inputs
  for (int pin = 0; pin < totalPins; ++pin) {
    pinMode(pin, INPUT);
  }

  // Print board information
  Serial.println("Arduino Uno Debug Information");
  Serial.println("------------------------------------");
  Serial.print("Board: Arduino Uno");
  Serial.print(", CPU: ");
  Serial.print(F_CPU / 1000000);
  Serial.println(" MHz");
  Serial.print("Flash Memory: ");
  Serial.print((int)FLASHEND / 1024);
  Serial.println(" KB");
  Serial.print("SRAM: ");
  Serial.print((int)RAMEND / 1024);
  Serial.println(" KB");
  Serial.println("------------------------------------");
  Serial.println("Pinout Information (Condensed)");
  Serial.println("------------------------------------");
}

void loop() {
  // Read and print the state of each pin
  for (int pin = 0; pin < totalPins; ++pin) {
    int pinState = digitalRead(pin);
    Serial.print("P");
    Serial.print(pin);
    Serial.print(":");
    Serial.print(pinState);

    // Check if the pin is capable of PWM output
    if (pin == 3 || pin == 5 || pin == 6 || pin == 9 || pin == 10 || pin == 11) {
      Serial.print(" PWM");
    }

    // Check if the pin is an analog input
    if (pin >= 14 && pin <= 19) {
      Serial.print(" ANALOG");
    }

    // Print additional information about the pin
    switch (pin) {
      case 0:
        Serial.print(" RX");
        break;
      case 1:
        Serial.print(" TX");
        break;
      case 13:
        Serial.print(" LED");
        break;
      default:
        // No additional information for other pins
        break;
    }

    if (pin < totalPins - 1) {
      Serial.print(", ");
    } else {
      Serial.println();
    }
  }

  // Print memory and CPU usage
  Serial.println("------------------------------------");
  Serial.print("Free SRAM: ");
  Serial.print(freeMemory());
  Serial.print(" bytes");
  Serial.print(", CPU Usage: ");
  Serial.print(millis() / 10);
  Serial.println("%");
  Serial.println("------------------------------------");

  delay(1000);  // Delay for better readability
}

// Function to calculate free SRAM
int freeMemory() {
  extern int __heap_start, *__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}
