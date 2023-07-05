#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "ssid";
const char* password = "password";
const int telnetPort = 23;

WiFiServer server(telnetPort);
WiFiClient clients[5];
int numClients = 0;

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  server.begin();

  Serial.println("Server started");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (!clients[numClients] || !clients[numClients].connected()) {
    clients[numClients] = server.available();
    if (clients[numClients]) {
      // New client connected, send welcome message
      clients[numClients].print("\033[2J"); // Clear screen
      printAsciiArt(clients[numClients]);
      clients[numClients].println("Welcome!");
      clients[numClients].println("Type 'help' for a list of commands.");
    }
  } else {
    if (clients[numClients].available()) {
      String command = readCommand(clients[numClients]);
      processCommand(command, clients[numClients]);
    }
  }
}

void printAsciiArt(WiFiClient client) {
  client.println("\033[1;32m");
  client.println("  _ \\            |         |   ");
  client.println(" |   | _ \\   __| |  /  _ \\ __| ");
  client.println(" ___/ (   | (      <   __/ |   ");
  client.println("_|   \\___/ \\___|_|\\_\\\\___|\\__| ");
  client.println("                               ");
  client.println("");
  client.println("\033[1;34m");
  client.println("  __ )  __ )  ___|  ");
  client.println("  __ \\  __ \\___ \\  ");
  client.println(" |   | |   |     | ");
  client.println(" ____/ ____/_____/  ");
  client.println("\033[0m");
}

String readCommand(WiFiClient client) {
  String command;
  while (client.available()) {
    char c = client.read();
    command += c;
    if (c == '\n') {
      break;
    }
  }
  command.trim();
  return command;
}

void processCommand(String command, WiFiClient client) {
  if (command.startsWith("help")) {
    handleHelpCommand(client);
  } else if (command.startsWith("gpio")) {
    handleGPIOCommand(client);
  } else if (command.startsWith("hw")) {
    handleHardwareCommand(client);
  } else if (command.startsWith("stats")) {
    handleStatsCommand(client);
  } else {
    client.println("Unknown command. Type 'help' for a list of commands.");
  }
}

void handleHelpCommand(WiFiClient client) {
  client.print("\033[2J"); // Clear screen
  printAsciiArt(client);
  client.println("Available commands:");
  client.println("help - Display this help message");
  client.println("gpio - Display information related to all pins");
  client.println("hw - Display information related to all development board hardware specs");
  client.println("stats - Display system statistics such as uptime, memory usage, etc.");
}

void handleGPIOCommand(WiFiClient client) {
  client.println("\033[1;36m"); // Set text color to cyan

  // Display GPIO Pins Information Menu
  client.println("GPIO Pins Information:");
  client.println("-------------------------------------------");
  client.println("\033[1;33m"); // Set text color to yellow

  // Iterate over all GPIO pins
  for (int pin = 0; pin < 16; ++pin) {
    String pinInfo = "Pin ";
    pinInfo += String(pin);
    pinInfo += ": ";
    pinInfo += (digitalRead(pin) == HIGH) ? "HIGH" : "LOW";
    client.println(pinInfo);
  }

  client.println("\033[0m"); // Reset text color
  client.println("-------------------------------------------");

  // Additional GPIO debugging information
  client.println("\033[1;35m"); // Set text color to magenta
  client.println("Additional GPIO Debugging Information:");
  // Add your custom code here to provide additional GPIO debugging information

  client.println("\033[0m"); // Reset text color
}

void handleHardwareCommand(WiFiClient client) {
  // Handle hw command logic
  client.println("\033[1;32m");
  client.println("Development Board Hardware Information:");
  client.println("\033[0m");
  // Get CPU information
  String cpuInfo = "CPU: ";
  cpuInfo += ESP.getCpuFreqMHz();
  cpuInfo += "MHz";
  client.println(cpuInfo);

  // Get flash memory size
  String flashSize = "Flash Memory: ";
  flashSize += ESP.getFlashChipSize() / (1024 * 1024);
  flashSize += "MB";
  client.println(flashSize);

  // Get free heap memory
  String freeHeap = "Free Heap Memory: ";
  freeHeap += ESP.getFreeHeap();
  freeHeap += " bytes";
  client.println(freeHeap);

  // Get heap memory stats
  uint32_t freeHeapSize, maxHeapSize;
  uint8_t heapFragmentation;
  ESP.getHeapStats(&freeHeapSize, &maxHeapSize, &heapFragmentation);

  String heapStats = "Heap Memory Stats: ";
  heapStats += "Free: ";
  heapStats += freeHeapSize;
  heapStats += " bytes, Max: ";
  heapStats += maxHeapSize;
  heapStats += " bytes, Fragmentation: ";
  heapStats += heapFragmentation;
  heapStats += "%";
  client.println(heapStats);

  // Get sketch size
  String sketchSize = "Sketch Size: ";
  sketchSize += ESP.getSketchSize();
  sketchSize += " bytes";
  client.println(sketchSize);

  // Get available sketch space
  String sketchSpace = "Available Sketch Space: ";
  sketchSpace += ESP.getFreeSketchSpace();
  sketchSpace += " bytes";
  client.println(sketchSpace);

  // Get network information
  client.println("\033[1;32m");
  client.println("Network Information:");
  client.println("\033[0m");
  client.print("MAC Address: ");
  client.println(WiFi.macAddress());
  client.print("IP Address: ");
  client.println(WiFi.localIP());
  client.print("Gateway: ");
  client.println(WiFi.gatewayIP());
  client.print("DNS Server: ");
  client.println(WiFi.dnsIP());

  client.println("\033[1;32m");
  client.println("Other Info:");
  client.println("\033[0m");

  // Get power consumption
  String powerConsumption = "Power Consumption: ";
  powerConsumption += ESP.getVcc();
  powerConsumption += " V";
  client.println(powerConsumption);

  // List open ports
  client.println("Open Ports:");
  client.println("Port  Protocol");

  // Add your custom code here to list open ports

  // Get firmware version
  String firmwareVersion = "Firmware Version: ";
  firmwareVersion += ESP.getCoreVersion();
  client.println(firmwareVersion);
}

void handleStatsCommand(WiFiClient client) {
  // Handle stats command logic
  client.println("Stats command is not implemented yet.");
}
