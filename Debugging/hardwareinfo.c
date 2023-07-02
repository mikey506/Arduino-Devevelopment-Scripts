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
  Serial.print(F_CPU / 10000000);
  Serial.println(" MHz");
  Serial.print("Flash Memory: ");
  Serial.print((int)FLASHEND / 1024);
  Serial.println(" KB");
  Serial.print("SRAM: ");
  Serial.print((int)RAMEND / 1024);
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
  Serial.println("Additional Diagnostic Information");
  Serial.println("------------------------------------");
  // Print software info
  Serial.print("Software Version: ");
  Serial.println(__VERSION__);
  Serial.print("Compiler Optimization Level: ");
#ifdef __OPTIMIZE__
  Serial.print(__OPTIMIZE__);
#else
  Serial.print("None");
#endif
  Serial.println();
  Serial.print("Kernel Version: ");
#ifdef __KERNEL_VERSION__
  Serial.println(__KERNEL_VERSION__);
#else
  Serial.println("N/A");
#endif
  Serial.println("------------------------------------");

  delay(100000);  // Delay for better readability
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
}

// Function to calculate free SRAM
int freeMemory() {
  extern int __heap_start, *__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}
