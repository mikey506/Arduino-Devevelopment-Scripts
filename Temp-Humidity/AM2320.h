#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AM2320.h>

Adafruit_AM2320 am2320;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  am2320.begin();
}

void loop() {
  delay(2000);  // Wait for 2 seconds for accurate readings

  float temperature = am2320.readTemperature();
  float humidity = am2320.readHumidity();

  if (!isnan(temperature) && !isnan(humidity)) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C");

    Serial.print("\tHumidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  } else {
    Serial.println("Failed to read data from AM2320 sensor.");
  }
}
