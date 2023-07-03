// Pinout mapping
const int totalPins = 20;  // Total number of GPIO pins on Arduino Uno

void setup() {
  Serial.begin(9600);  // Initialize serial communication

  // Print board information
  Serial.println("Arduino Uno Debug Information");
  Serial.println("------------------------------------");
  Serial.print("Board Information:\n");
  Serial.print("Model and Variant: Arduino Uno");
  Serial.print(", CPU Architecture: ");
#ifdef __AVR__
  Serial.print("AVR");
#elif defined(__SAM3X8E__)
  Serial.print("SAM (ARM)");
#elif defined(ESP8266)
  Serial.print("ESP8266");
#elif defined(ESP32)
  Serial.print("ESP32");
#else
  Serial.print("Unknown");
#endif
  Serial.print(", Clock Frequency: ");
  Serial.print(F_CPU / 1000000);
  Serial.println(" MHz");
  Serial.print("Memory Information:\n");
  Serial.print("Flash Memory Size: ");
  Serial.print((int)FLASHEND / 1024);
  Serial.println(" KB");
  Serial.print("SRAM Size: ");
  Serial.print((int)RAMEND / 1024);
  Serial.println(" KB");
  Serial.print("EEPROM Size: ");
  Serial.print((int)E2END / 1024);
  Serial.println(" KB");
  Serial.println("------------------------------------");
  Serial.println("Pinout Information (Optimized Table)");
  Serial.println("------------------------------------");
}

void loop() {
  // Read and print the state of each pin in a table
  Serial.println("------------------------------------");
  Serial.println("| Pin  | State | Capabilities        |");
  Serial.println("------------------------------------");

  for (int row = 0; row < totalPins; row += 3) {
    Serial.print("| P");
    Serial.print(row);
    Serial.print("  |  ");
    printPinInfo(row);

    Serial.print("  | P");
    Serial.print(row + 1);
    Serial.print("  |  ");
    printPinInfo(row + 1);

    Serial.print("  | P");
    Serial.print(row + 2);
    Serial.print("  |  ");
    printPinInfo(row + 2);

    Serial.println("  |");
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

  // Print software-related diagnostic information
  Serial.println("Software Information:");
  Serial.println("------------------------------------");
  Serial.print("Software Version: ");
  Serial.println(__VERSION__);
  Serial.print("Compiler Optimization Level: ");
#ifdef __OPTIMIZE__
  Serial.print(__OPTIMIZE__);
#else
  Serial.print("None");
#endif
  Serial.println();

  delay(1000);  // Delay for better readability
}

// Function to print pin information
void printPinInfo(int pin) {
  int pinState = digitalRead(pin);
  Serial.print(pinState);

  // Check if the pin is capable of PWM output
  if (pin == 3 || pin == 5 || pin == 6 || pin == 9 || pin == 10 || pin == 11) {
    Serial.print(" PWM");
  }

  // Check if the pin is an analog input
  if (pin >= 14 && pin <= 19) {
    Serial.print(" ANALOG");
  }

  Serial.print("    ");
}

// Function to calculate free SRAM
int freeMemory() {
  extern int __heap_start, *__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}
