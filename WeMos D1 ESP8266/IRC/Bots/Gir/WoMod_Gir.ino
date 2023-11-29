#include <pgmspace.h>
#include "config.h"
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Arduino.h>

// Enter your WiFi credentials
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";

// Enter your IRC bot credentials
const char* botPassword = "abc123";
const char* botNick = "Gir";
const char* botUsername = "Girby";

// Enter your IRC server details
const char* server = "irc.server.org";
const int port = 6667;

// Enter the channel you want to join
const char* channel = "#lobby";

// Initialize the WiFi client and IRC client
WiFiClient client;
String ircServerResponse;

enum ConnectionState {
  DISCONNECTED,
  CONNECTING,
  CONNECTED
};

ConnectionState connectionState = DISCONNECTED;

void setup() {
  Serial.begin(115200);

  while (!Serial) {
    delay(100);
  }

  Serial.println("Booting");
  
  connectToWiFi();
}

void loop() {
  switch (connectionState) {
    case DISCONNECTED:
      startIRCConnection();
      break;

    case CONNECTING:
      // Handle connecting state if needed
      break;

    case CONNECTED:
      handleIRCMessages();
      break;
  }
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected to WiFi");
}

void startIRCConnection() {
  Serial.print("Connecting to IRC server: ");
  Serial.println(server);

  if (client.connect(server, port)) {
    Serial.println("Connected to IRC server");

    client.print("PASS ");
    client.println(botPassword);
    delay(500);
    
    client.print("NICK ");
    client.println(botNick);
    delay(500);
    
    client.print("USER ");
    client.println(botUsername);
    delay(500);
    
    Serial.print("Joining Channel: ");
    Serial.println(channel);
    
    client.print("JOIN ");
    client.println(channel);
    
    Serial.println("Channel Joined");
    Serial.println("==== Connection to server completed ====");

    // Move to connected state
    connectionState = CONNECTED;
  } else {
    Serial.println("Failed to connect to IRC server");

    Serial.println("Retrying in 1 minute...");
    delay(60000);  // 1 minute delay
  }
}

void handleIRCMessages() {
  if (client.connected()) {
    if (client.available()) {
      String message = client.readStringUntil('\n');
      Serial.println(message);

      if (message.startsWith("PING")) {
        ircServerResponse = "PONG" + message.substring(4);
        client.println(ircServerResponse);
      }

      if (message.indexOf("PRIVMSG") != -1) {
        // Handle private messages
      }
    }
  } else {
    Serial.println("Disconnected from IRC server");

    Serial.println("Reconnecting...");
    connectionState = DISCONNECTED;
  }
}
