# Catalex MicroSD Card Adapter v1.0 (sdcard_catalex.h)

![Pinout](https://mschoeffler.com/wp-content/uploads/2017/02/fritzing_bb.png)

Make sure you have the necessary libraries installed. You can install them through the Arduino Library Manager. Look for the "SD" library by Adafruit and the "SPI" library by Arduino.

This script initializes the SD card module, opens a file called "data.txt," and writes the string "Hello, SD card!" to it. Then it closes the file and reopens it for reading. It reads the contents of the file and sends them to the serial monitor.

Remember to connect the Catalex SD Card module properly to your Arduino board, including the necessary power and SPI connections. Adjust the chipSelect pin according to your wiring configuration.

Upload the script to your Arduino board, open the serial monitor, and you should see the output of the read and write operations.
