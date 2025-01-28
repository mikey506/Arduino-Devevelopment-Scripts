# ESP8266 ISS Tracker

![image](https://github.com/user-attachments/assets/d096a4e3-c29d-45a4-a5b0-8b692f055f18)


A web-based ISS (International Space Station) tracker using an ESP8266 microcontroller, Open Notify API, and Leaflet.js for map visualization. This project fetches real-time ISS coordinates and displays them on an interactive map.

---

## **Features**
- Connects to WiFi and starts a web server on the ESP8266.
- Fetches ISS coordinates from the [Open Notify API](http://open-notify.org/Open-Notify-API/ISS-Location-Now/) every 10 seconds.
- Serves a responsive web dashboard with:
  - An interactive Leaflet.js map.
  - Real-time ISS position updates using JavaScript.
- Provides a `/iss-data` API endpoint for ISS location data in JSON format.

---

## **How It Works**
1. **ESP8266 Setup:**
   - Connects to WiFi and starts an HTTP server.
   - Fetches ISS location data (`latitude` and `longitude`) from Open Notify API.

2. **Web Dashboard:**
   - Hosts an HTML page with Leaflet.js for map rendering.
   - JavaScript periodically queries the `/iss-data` endpoint to update the ISS marker on the map.

3. **API Endpoint:**
   - `/iss-data` provides the latest ISS coordinates in JSON format (e.g., `{"lat": 51.6584, "lon": -142.5489}`).

---

## **Installation**
1. Install the following libraries in your Arduino IDE:
   - **ESP8266WiFi**
   - **ESP8266WebServer**
   - **ArduinoJson**

2. Configure WiFi credentials in the code:
   ```cpp
   const char* ssid = "Your_SSID";
   const char* password = "Your_PASSWORD";
   ```

3. Upload the code to your ESP8266 board.

---

## **Usage**
### **Connect to the ESP8266:**
1. Open the Serial Monitor to get the IP address of the ESP8266.
2. Navigate to `http://<ESP_IP>` in your web browser.

### **Interactive Map:**
- View the ISS's position on a full-screen map.
- The marker updates every 10 seconds with the latest ISS location.

### **API Endpoint:**
- Access `http://<ESP_IP>/iss-data` to get the ISS's current coordinates in JSON format.

---

## **Testing**
### **Serial Monitor Example:**
```plaintext
WiFi connected! IP: 192.168.1.100
ISS Position: Latitude 51.6584, Longitude -142.5489
Serving /iss-data: {"lat": 51.6584, "lon": -142.5489}
```

### **Browser:**
- A full-screen map displays the ISS's current position.
- The marker updates every 10 seconds with new data.

### **Example API Output:**
#### **/iss-data Response:**
```json
{
  "lat": 51.6584,
  "lon": -142.5489
}
```

---

## **Credits**
- **Leaflet.js:** Interactive map rendering ([Leaflet.js](https://leafletjs.com)).
- **Open Notify API:** Provides real-time ISS position data ([Open Notify API](http://open-notify.org/)).
- **OpenStreetMap:** Map tiles ([OpenStreetMap](https://www.openstreetmap.org/)).

---

## **License**
This project is open-source and available under the MIT License.
