# UltraSonic_Relay_HTTPd.ino

| Component                      | Pin       | Connection                |
|---------------------------------|-----------|---------------------------|
| **Ultrasonic Sensor (HC-SR04)** |
| VCC                             | 5V        | Power                     |
| GND                             | GND       | Ground                    |
| Trig                            | GPIO5 (D1)| Trigger Pin               |
| Echo                            | GPIO4 (D2)| Echo Pin                  |
| **4-Channel Relay Board**       |
| VCC                             | 5V (Ext.) | External Power            |
| GND                             | GND       | Common Ground (ESP8266)   |
| IN1                             | GPIO14 (D5)| Relay 1 Control           |
| IN2                             | GPIO12 (D6)| Relay 2 Control           |
| IN3                             | GPIO13 (D7)| Relay 3 Control           |
| IN4                             | GPIO15 (D8)| Relay 4 Control           |
| **LED (Optional)**              |
| Anode (+)                       | GPIO2 (D4)| Positive Connection      |
| Cathode (-)                     | GND       | Negative Connection (GND)|
| **Notes**                       |
| -                               | -         | -                         |
| Ensure 5V external power for the relay board.|
| Verify proper connections and consider adding resistors for LED based on specifications.|
| Implement `updateDistance` and `updateRelayStatus` functions for sensor and relay control.|
