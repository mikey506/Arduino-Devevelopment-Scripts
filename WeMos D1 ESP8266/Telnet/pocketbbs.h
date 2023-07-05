#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "SSID";
const char* password = "PASSWORD";
const int telnetPort = 23;

WiFiServer server(telnetPort);
WiFiClient clients[5];
String messageBoard[10];
int currentMessageIndex = 0;
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

  for (int i = 0; i < 5; i++) {
    clients[i] = WiFiClient();
  }

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
      clients[numClients].println("Welcome to the Message Board!");
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
  if (command.startsWith("post")) {
    handlePostCommand(command, client);
  } else if (command.startsWith("read")) {
    handleReadCommand(client);
  } else if (command.startsWith("delete")) {
    handleDeleteCommand(command, client);
  } else if (command.startsWith("clear")) {
    handleClearCommand(client);
  } else if (command.startsWith("help")) {
    handleHelpCommand(client);
  } else {
    client.println("Unknown command. Type 'help' for a list of commands.");
  }
}

void handlePostCommand(String command, WiFiClient client) {
  command.remove(0, 5); // Remove "post " from the command
  command.trim(); // Remove leading/trailing whitespace

  if (command.length() < 1) {
    client.println("Usage: post [message]");
    return;
  }

  if (currentMessageIndex >= 10) {
    client.println("Message board is full. Please delete some messages.");
    return;
  }

  messageBoard[currentMessageIndex] = command;
  currentMessageIndex++;

  client.println("Message posted successfully.");
}

void handleReadCommand(WiFiClient client) {
  client.print("\033[2J"); // Clear screen
  printAsciiArt(client);
  client.println("=== Message Board ===");

  for (int i = 0; i < currentMessageIndex; i++) {
    client.print(i + 1);
    client.print(". ");
    client.println(messageBoard[i]);
  }
}

void handleDeleteCommand(String command, WiFiClient client) {
  command.remove(0, 7); // Remove "delete " from the command
  command.trim(); // Remove leading/trailing whitespace

  if (command.length() < 1) {
    client.println("Usage: delete [message number]");
    return;
  }

  int messageNumber = command.toInt();
  if (messageNumber < 1 || messageNumber > currentMessageIndex) {
    client.println("Invalid message number.");
    return;
  }

  for (int i = messageNumber - 1; i < currentMessageIndex - 1; i++) {
    messageBoard[i] = messageBoard[i + 1];
  }

  currentMessageIndex--;

  client.println("Message deleted successfully.");
}

void handleClearCommand(WiFiClient client) {
  client.print("\033[2J"); // Clear screen
  printAsciiArt(client);
  client.println("Message board cleared.");

  for (int i = 0; i < currentMessageIndex; i++) {
    messageBoard[i] = "";
  }

  currentMessageIndex = 0;
}

void handleHelpCommand(WiFiClient client) {
  client.print("\033[2J"); // Clear screen
  printAsciiArt(client);
  client.println("Available commands:");
  client.println("post [message] - Post a new message");
  client.println("read - Read all messages");
  client.println("delete [message number] - Delete a message");
  client.println("clear - Clear the message board");
  client.println("help - Display this help message");
}
