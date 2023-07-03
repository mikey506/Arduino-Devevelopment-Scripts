# **env_control.h**

This script is an example of a temperature and humidity monitoring system using an Arduino board and the AM2320 sensor. Below is the full documentation for using this script, including the libraries used and pinout instructions.

Libraries Used
The following libraries are used in this script:

**Arduino.h**: The core library that provides basic Arduino functions.

**Wire.h**: A library for I2C communication.

**Adafruit_Sensor.h**: A library that provides an abstract base class for sensors.

**Adafruit_AM2320.h**: A library for interfacing with the AM2320 temperature and humidity sensor.

Please make sure you have installed these libraries before using the script. You can install libraries in the Arduino IDE by navigating to "Sketch" -> "Include Library" -> "Manage Libraries" and searching for the library names.

Pinout Instructions
The script uses three relay pins to control the relays. The relay pins are defined as constants in the script:

**const int RELAY_1_PIN = 2;**

**const int RELAY_2_PIN = 3;**

**const int RELAY_3_PIN = 4;**

Make sure to connect the relay pins to the appropriate GPIO pins on your Arduino board. In the example above, the relay pins are connected to GPIO pins 2, 3, and 4.

Additionally, the script communicates with the AM2320 sensor using the I2C protocol. The sensor is connected to the Arduino's I2C bus, which typically uses the SDA (data) and SCL (clock) pins. The I2C communication is handled by the Wire library, and the initialization of the sensor is done using the am2320.begin() function in the setup() function:

**Wire.begin();**

**am2320.begin();**

Make sure to connect the SDA and SCL pins of the AM2320 sensor to the corresponding pins on your Arduino board.

Once you have the hardware set up and the libraries installed, you can upload the script to your Arduino board and monitor the temperature and humidity readings. The script will print the readings, relay state information, and system information to the serial monitor. Additionally, it checks for high temperature or humidity conditions and prints a message if detected.
