# WaterBoy-Bot.ino

Script utilizing 1ch relay and soil moisture sensor to automate watering for potted soil.

## 1x Soil Moisture Sensor + 1ch Relay ( **WaterBoy-Bot.ino** ):

The soil moisture sensor typically has four pins: VCC, GND, AO (Analog Output), and DO (Digital Output). Here's how you can connect it:

**VCC:** Connect to a 3.3V pin on the ESP8266 board.

**GND:** Connect to a GND pin on the ESP8266 board.

**AO** (Analog Output): Connect to an analog input pin (e.g., A0) on the ESP8266 board for analog readings. **(Initial script uses Analog)**

**DO** (Digital Output): Optional. Connect to a digital input pin if you want to use the digital output.

## 4x Soil Moisture Sensors (digital output) + 1ch Relay ( **WaterBoy_Bot-1relay-4sensors.ino** )

**4x Soil Moisture Sensors & GPIO:**
- Sensor 1:          **D1 (GPIO5)**
- Sensor 2:          **D2 (GPIO4)**
- Sensor 3:          **D3 (GPIO0)**
- Sensor 4:          **D4 (GPIO2)**
- 1ch Relay input uses sane **D5** like mentioned below.
- 4x GND to **GND**
- 4x VCC to **3.3v**

**Additional Information**
In most cases, you can hook up multiple digital soil moisture sensors to the same 3.3V GPIO pin on the WeMos D1 Development board. Digital sensors typically require very little current, so they can usually share the same power source without any issues.

Make sure that the digital sensors are compatible with sharing a power source and that they do not interfere with each other when connected in parallel. Sometimes, interference can occur due to shared signal lines or electrical noise. If you experience any issues, you might need to use separate power sources for each sensor or implement isolation techniques to prevent interference.

**Isolation Techniques (if needed):** Optical Isolation / Galvanic Isolation / Differential Signaling / Ground Isolation / Shielding / Filters and Decoupling Capacitors

## Single Channel Relay:

The single-channel relay typically has three pins: VCC, GND, and IN (Control Input). Here's how you can connect it:


**VCC:** Connect to a 5V pin on the ESP8266 board (if the relay operates at 5V).

**GND:** Connect to a GND pin on the ESP8266 board.

**IN (Control Input)**: Connect to a digital output pin (**D5**) on the WeeMos board to control the relay.

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
