**AM2320.h**

The revised script for reading temperature and humidity from the AM2320 sensor utilizes the following libraries:

Wire: This library is a built-in library in the Arduino IDE that provides the I2C communication protocol functionality. It is used for communication with the AM2320 sensor over the I2C bus.

Adafruit_Sensor: This is an Adafruit library that provides a common interface for reading sensor data. It is used by the Adafruit AM2320 library to interact with the AM2320 sensor.

Adafruit_AM2320: This is the Adafruit library specifically designed for the AM2320 temperature and humidity sensor. It provides functions to read temperature and humidity values from the sensor.

To use these libraries in your Arduino IDE, you need to ensure they are installed. Here's how you can install them:

Open your Arduino IDE.
Go to "Sketch" -> "Include Library" -> "Manage Libraries".
In the Library Manager, search for each library by name: "Wire", "Adafruit Sensor", and "Adafruit AM2320".
Click on each library and click the "Install" button to install them.
Make sure to install the latest versions of the libraries to ensure compatibility and access to the necessary functions.

![AM2320 Sensor GPIO](https://www.makerguides.com/wp-content/uploads/2020/10/AM2320-digital-temperature-and-humidity-sensor-with-Arduino-wiring-diagram-schematic-featured-image.png)
