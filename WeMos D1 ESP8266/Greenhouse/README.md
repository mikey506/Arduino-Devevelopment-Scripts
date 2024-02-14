# auto_pot-soil_irrigation.ino

Script utilizing 1ch relay and soil moisture sensor to automate watering for potted soil.

## Soil Moisture Sensor:

The soil moisture sensor typically has four pins: VCC, GND, AO (Analog Output), and DO (Digital Output). Here's how you can connect it:

**VCC:** Connect to a 3.3V pin on the ESP8266 board.

**GND:** Connect to a GND pin on the ESP8266 board.

**AO** (Analog Output): Connect to an analog input pin (e.g., A0) on the ESP8266 board for analog readings. **(Initial script uses Analog)**

**DO** (Digital Output): Optional. Connect to a digital input pin if you want to use the digital output.

## Single Channel Relay:

The single-channel relay typically has three pins: VCC, GND, and IN (Control Input). Here's how you can connect it:


**VCC:** Connect to a 5V pin on the ESP8266 board (if the relay operates at 5V).

**GND:** Connect to a GND pin on the ESP8266 board.

**IN (Control Input)**: Connect to a digital output pin (D5) on the WeeMos board to control the relay.

## Wifi AP Settings (Default)

**SSID:** NodeMCU

**Wifi Password:** 12345678

**AP IP Address:** 192.168.1.1

**AP Gateway:** 192.168.1.1

**IP Subnet Mask:** 255.255.255.0

## Web Interface

**Dashboard URL:** http://192.168.1.1/

**Enable Relay:** http://192.168.1.1/relayon

**Disable Relay:**  http://192.168.1.1/relayoff
