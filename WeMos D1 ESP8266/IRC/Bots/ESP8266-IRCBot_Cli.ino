#include <ESP8266WiFi.h>
#include <IRCClient.h>
#include <Wire.h>


// Wifi settings
#define ssid         "SSID"   // Edit These!
#define password     "XXXX"

// IRC settings
#define IRC_SERVER   "irc.ephasic.org"  // Edit These!
#define IRC_PORT     6667
#define IRC_NICKNAME "WaterBoyBot"
#define IRC_USER     "BobbyBoucherBot"

#define REPLY_TO     "Master" // Reply only to this nick.. Edit This!

WiFiClient wiFiClient;
IRCClient client(IRC_SERVER, IRC_PORT, wiFiClient);


void setup() {
  Serial.begin(9600); // Set baud rate to 9600
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  delay(100); 

  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  WiFi.printDiag(Serial);

  client.setCallback(callback);
  client.setSentCallback(debugSentCallback);
}

void loop() {
  if (!client.connected()) {
    Serial.println("Attempting IRC connection...");
    // Attempt to connect
    if (client.connect(IRC_NICKNAME, IRC_USER)) {
      Serial.println("connected");
    } else {
      Serial.println("failed... try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    return;
  }

  client.loop();
}

//  IRC Triggers and output
void callback(IRCMessage ircMessage) {
  // PRIVMSG ignoring CTCP messages
  if (ircMessage.command == "PRIVMSG" && ircMessage.text[0] != '\001') {
    String message("<" + ircMessage.nick + "> " + ircMessage.text);
    Serial.println(message);
  }
  // Channels to JOIN
  if (ircMessage.nick == REPLY_TO  && ircMessage.parameters == IRC_NICKNAME  && ircMessage.text == "!join") {
      //client.sendMessage(ircMessage.nick, "Hi " + ircMessage.nick + "! I'm your IRC bot.");
      //client.sendMessage(ircMessage.nick, "joining channels...");
      client.sendRaw("JOIN #Lobby");  // Channels EDIT THESE!
      client.sendRaw("JOIN #Plants");  // Channels
  }
  
  //  ESP Specific
  if (ircMessage.text == "!ledon") {
      digitalWrite(LED_BUILTIN, LOW);    
      client.sendMessage(ircMessage.parameters, "ESP8266 Onboard LED: ON");   
  }
  if (ircMessage.text == "!ledoff") {
      digitalWrite(LED_BUILTIN, HIGH);    
      client.sendMessage(ircMessage.parameters, "ESP8266 Onboard LED: OFF");   
  }
  if (ircMessage.text == "!getvcc") {
      float vccd = ESP.getVcc();
      client.sendMessage(ircMessage.parameters, "ESP8266 Voltage: " + String(((vccd/1024)*100),2) + "v");
  }
  if (ircMessage.text == "!getfreeheap") {
      float freeheap = ESP.getFreeHeap();
      client.sendMessage(ircMessage.parameters, "ESP8266 Free Heap: " + String((freeheap),0) + " bytes");
  }
  if (ircMessage.text == "!getcycles") {
      float cycles = ESP.getCycleCount();
      client.sendMessage(ircMessage.parameters, "ESP8266 Instruction Cycles since startup: " + String((cycles),0));
  }
  if (ircMessage.text == "!getcpu") {
      float freq = ESP.getCpuFreqMHz();
      client.sendMessage(ircMessage.parameters, "ESP8266 CPU Frequency: " + String((freq),0) + "MHz");
  }
  if (ircMessage.text == "!getresetreason") {
      String reason = ESP.getResetReason();
      client.sendMessage(ircMessage.parameters, "ESP8266 Restart Reason: " + String(reason));
  }
  if (ircMessage.text == "!getchipid") {
      float chipid = ESP.getChipId();
      client.sendMessage(ircMessage.parameters, "ESP8266 ChipID: 0x" + String((chipid),HEX));
  }
  if (ircMessage.text == "!getresetinfo") {
      String resetinfo = ESP.getResetInfo();
      client.sendMessage(ircMessage.parameters, "ESP8266 Reset Info: " + resetinfo);
  }
  if (ircMessage.text == "!getsketchsize") {
      float sketchsize = ESP.getSketchSize();
      client.sendMessage(ircMessage.parameters, "ESP8266 Sketch Size: " + String((sketchsize),0));
  }
  if (ircMessage.text == "!getversion") {
      const char* sdkversion = ESP.getSdkVersion();
      String coreversion = ESP.getCoreVersion();
      String fullversion = ESP.getFullVersion();
      uint8_t bootversion = ESP.getBootVersion();
      client.sendMessage(ircMessage.parameters, "ESP8266 SDK Version: " + String(sdkversion));
      client.sendMessage(ircMessage.parameters, "ESP8266 Core Version: " + String(coreversion));
      client.sendMessage(ircMessage.parameters, "ESP8266 Full Version: " + String(fullversion));
      client.sendMessage(ircMessage.parameters, "ESP8266 Boot Version: " + String(bootversion));
  }

  //     Wifi scan
  if (ircMessage.text == "!scanwifi") {
     int n = WiFi.scanNetworks();
     client.sendMessage(ircMessage.parameters, "ESP8266 found " + String(n) + " SSIDs:");
     for (int i = 0; i < n; i++) {
      client.sendMessage(ircMessage.parameters, String(i+1) + ") " + String(WiFi.SSID(i)) + " Ch: " + String(WiFi.channel(i)) + " (" + String(WiFi.RSSI(i)) + "dBm) " + String(WiFi.encryptionType(i) == ENC_TYPE_NONE ? ":OPEN" : ""));  
     }
  }
    
    
    if (ircMessage.text == "!help") {  
      client.sendMessage(ircMessage.nick, "ESP8266 IRC-Bot Help");  
      client.sendMessage(ircMessage.nick, "ESP Commands:");
      client.sendMessage(ircMessage.nick, "!ledon :turn on onboard LED");
      client.sendMessage(ircMessage.nick, "!ledoff :turn off onboard LED");
      client.sendMessage(ircMessage.nick, "!getvcc :shows ESP8266 voltage");
      client.sendMessage(ircMessage.nick, "!getfreeheap :shows ESP8266 free heap memory");
      client.sendMessage(ircMessage.nick, "!getcycles :shows ESP8266 cpu instruction cycle count since start as an unsigned 32-bit");
      client.sendMessage(ircMessage.nick, "!getcpu :shows ESP8266 CPU Frequency");
      client.sendMessage(ircMessage.nick, "!getresetreason :shows ESP8266 Reset Reason");
      client.sendMessage(ircMessage.nick, "!getresetinfo :shows ESP8266 Reset debug info");
      client.sendMessage(ircMessage.nick, "!getchipid :shows ESP8266 Chip ID");
      client.sendMessage(ircMessage.nick, "!getsketchsize :shows ESP8266 Arduino Sketch Size");
      client.sendMessage(ircMessage.nick, "!getversion :shows ESP8266 SDK, Core, Full, and Boot Versions");
      client.sendMessage(ircMessage.nick, "!scanwifi :scans wifi and displays SSIDs, Channel, RSSI, encryption type");
      client.sendMessage(ircMessage.nick, " ");
    }
  /*
  // debug
  Serial.print("prefix: " + ircMessage.prefix);
  Serial.print(" nick: " + ircMessage.nick);
  Serial.print(" user: " + ircMessage.user);
  Serial.print(" host: " + ircMessage.host);
  Serial.print(" command: " + ircMessage.command);
  Serial.print(" parameters: " + ircMessage.parameters);
  Serial.println(" text: " + ircMessage.text);
  */
  Serial.println(ircMessage.original);
  return;
}

void debugSentCallback(String data) {
  Serial.println(data);
}
