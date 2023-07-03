#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AM2320.h>

const int HIGH_TEMPERATURE_THRESHOLD = 30;
const int HIGH_HUMIDITY_THRESHOLD = 70;
const int LOW_TEMPERATURE_THRESHOLD = 15;
const int LOW_HUMIDITY_THRESHOLD = 40;

// Relay Pins
const int RELAY_1_PIN = 2;
const int RELAY_2_PIN = 3;
const int RELAY_3_PIN = 4;

Adafruit_AM2320 am2320;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  am2320.begin();

  // Set up relay pins
  pinMode(RELAY_1_PIN, OUTPUT);
  pinMode(RELAY_2_PIN, OUTPUT);
  pinMode(RELAY_3_PIN, OUTPUT);

  // Print initialization complete message
  Serial.println("Initialization complete");
}

void loop() {
  // Read temperature and humidity
  float temperature = readTemperature();
  float humidity = readHumidity();
  Serial.println("-----------------------------------");
  // Print temperature and humidity information with trend
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C");
  printTrend(getTemperatureTrend(temperature));
  Serial.print("\tHumidity: ");
  Serial.print(humidity);
  Serial.print(" %");
  printTrend(getHumidityTrend(humidity));
  Serial.println();

  // Print relay state information
  printRelayState();

  // Print system information
  printSystemInfo();

  // Check for high temperature or humidity
  if (temperature > HIGH_TEMPERATURE_THRESHOLD || humidity > HIGH_HUMIDITY_THRESHOLD) {
    Serial.println("Last Relay Change: High temperature or humidity detected!");
  }

  delay(10000); // Delay for 10 seconds
}

void printTrend(float value) {
  if (value > 0) {
    Serial.print(" (+)");
  } else if (value < 0) {
    Serial.print(" (-)");
  } else {
    Serial.print(" (=)");
  }
}

float getTemperatureTrend(float currentTemperature) {
  // Code to calculate temperature trend goes here
  // Replace with actual temperature trend calculation logic
  return random(-1, 2); // Returns -1, 0, or 1 randomly
}

float getHumidityTrend(float currentHumidity) {
  // Code to calculate humidity trend goes here
  // Replace with actual humidity trend calculation logic
  return random(-1, 2); // Returns -1, 0, or 1 randomly
}

void printRelayState() {
  // Print relay state information
  Serial.print("Relay State: ");
  Serial.print("R1: ");
  Serial.print(digitalRead(RELAY_1_PIN) == HIGH ? "ON" : "OFF");
  Serial.print(" / R2: ");
  Serial.print(digitalRead(RELAY_2_PIN) == HIGH ? "ON" : "OFF");
  Serial.print(" / R3: ");
  Serial.print(digitalRead(RELAY_3_PIN) == HIGH ? "ON" : "OFF");
  Serial.println();
}

void printSystemInfo() {
  // Print CPU information
  Serial.print("CPU Frequency: ");
  Serial.print(F_CPU / 1000000);
  Serial.print(" MHz / ");

  // Print memory information
  Serial.print("Memory: ");
  Serial.print(getTotalMemory());
  Serial.print(" - ");
  Serial.print(getUsedMemory());
  Serial.print(" bytes (Used) / ");

  // Print storage information
  Serial.print("Storage: ");
  Serial.print(getTotalStorage() / 1024);
  Serial.print(" - ");
  Serial.print(getUsedStorage() / 1024);
  Serial.println(" KB (Used)");

  Serial.print("Variables >> ");
  Serial.print(" High Temp: ");
  Serial.print(HIGH_TEMPERATURE_THRESHOLD);
  Serial.print(" / High Humidity: ");
  Serial.print(HIGH_HUMIDITY_THRESHOLD);
  Serial.print("/ Low Temp: ");
  Serial.print(LOW_TEMPERATURE_THRESHOLD);
  Serial.print("/ Low Humidity: ");
  Serial.println(LOW_HUMIDITY_THRESHOLD);
}

float readTemperature() {
  return am2320.readTemperature();
}

float readHumidity() {
  return am2320.readHumidity();
}

unsigned int getTotalMemory() {
  extern unsigned int __data_start, __data_end, __bss_end;
  return (unsigned int)&__data_end - (unsigned int)&__data_start + (unsigned int)&__bss_end;
}

unsigned int getUsedMemory() {
  extern unsigned int __heap_start;
  return (unsigned int)&__heap_start - 0x20000000;
}

unsigned int getTotalStorage() {
  return getSketchSize();
}

unsigned int getUsedStorage() {
  return getSketchSize() - getFreeSketchSpace();
}

unsigned int getSketchSize() {
  extern unsigned int __data_start;
  return (unsigned int)&__data_start - 0x20000000;
}

unsigned int getFreeSketchSpace() {
  extern unsigned int __heap_start;
  return (unsigned int)&__heap_start - 0x20000000;
}


