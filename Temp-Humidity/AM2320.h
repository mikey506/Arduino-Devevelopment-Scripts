#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AM2320.h>

Adafruit_AM2320 am2320;
unsigned long previousTime = 0;
unsigned long interval = 30000;  // 30-second interval

void setup() {
  Serial.begin(9600);
  Wire.begin();
  am2320.begin();
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - previousTime >= interval) {
    previousTime = currentTime;

    float temperature = am2320.readTemperature();
    float humidity = am2320.readHumidity();

    if (!isnan(temperature) && !isnan(humidity)) {
      Serial.print("[");
      Serial.print(currentTime);
      Serial.print(" ms] ");
      
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
}
