# **env_control_arduino_uno.h**


<img src="https://i.imgur.com/fYrUBhl.png" width=50% height=50%>
This script is an example of a temperature and humidity monitoring system using an Arduino board and the AM2320 sensor. Below is the full documentation for using this script, including the libraries used and pinout instructions.

Libraries Used
The following libraries are used in this script:

**Arduino.h**: The core library that provides basic Arduino functions.

**Wire.h**: A library for I2C communication.

**Adafruit_Sensor.h**: A library that provides an abstract base class for sensors.

**Adafruit_AM2320.h**: A library for interfacing with the AM2320 temperature and humidity sensor.

Please make sure you have installed these libraries before using the script. You can install libraries in the Arduino IDE by navigating to "Sketch" -> "Include Library" -> "Manage Libraries" and searching for the library names.

Pinout Instructions (Relay Board)

The script uses three relay pins to control the relays. The relay pins are defined as constants in the script:

**RELAY_1_PIN = 2;**

**RELAY_2_PIN = 3;**

**RELAY_3_PIN = 4;**

(If more than 3 relays being used, pins are sequential)

**VCC = 3.3v**

**GND = GND**

Pinout Diagram for Temp/Himidity Sensor: 
![AM2320 Sensor GPIO](https://www.makerguides.com/wp-content/uploads/2020/10/AM2320-digital-temperature-and-humidity-sensor-with-Arduino-wiring-diagram-schematic-featured-image.png)

GPIO Information for Arduino Uno:
![Arduino Uno Pinout](https://images.prismic.io/circuito/8e3a980f0f964cc539b4cbbba2654bb660db6f52_arduino-uno-pinout-diagram.png)

Make sure to connect the relay pins to the appropriate GPIO pins on your Arduino board. In the example above, the relay pins are connected to GPIO pins 2, 3, and 4.

Additionally, the script communicates with the AM2320 sensor using the I2C protocol. The sensor is connected to the Arduino's I2C bus, which typically uses the SDA (data) and SCL (clock) pins. The I2C communication is handled by the Wire library, and the initialization of the sensor is done using the am2320.begin() function in the setup() function:

**Wire.begin();**

**am2320.begin();**

Make sure to connect the SDA and SCL pins of the AM2320 sensor to the corresponding pins on your Arduino board.

Once you have the hardware set up and the libraries installed, you can upload the script to your Arduino board and monitor the temperature and humidity readings. The script will print the readings, relay state information, and system information to the serial monitor. Additionally, it checks for high temperature or humidity conditions and prints a message if detected.

# env_control_HLK-W801.h

This is a revision of the Arduino Uno Script designed to work with the HLK-W801 development board which has Wifi and Bluetooth, wifi functionality was added and a small ssh server also.

![HLK-W801](https://i.imgur.com/PWb0hHD.png)

Please note that you will need to replace "YOUR_WIFI_SSID" and "YOUR_WIFI_PASSWORD" with your actual Wi-Fi credentials. Additionally, make sure you have installed the necessary libraries (Adafruit_Sensor, Adafruit_AM2320, ESP8266WiFi, ESPAsyncWebServer) using the Arduino Library Manager before uploading the code to your board.

![GPIO Info](https://i.imgur.com/BV8bf6W.png)
