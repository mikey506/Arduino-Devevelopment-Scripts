#include <SPI.h>
#include <SD.h>

// Set the pin for the SD card module's chip select
const int chipSelect = 4;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Initialize SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  
  Serial.println("SD card initialized successfully.");
}

void loop() {
  // Open the file on the SD card
  File file = SD.open("data.txt", FILE_WRITE);
  
  // Check if the file was opened successfully
  if (file) {
    Serial.println("Writing to file...");
    
    // Write some data to the file
    file.println("Hello, SD card!");
    
    // Close the file
    file.close();
    
    Serial.println("Data written successfully.");
  } else {
    Serial.println("Error opening file!");
  }
  
  delay(1000); // Wait for a second
  
  // Open the file for reading
  file = SD.open("data.txt");
  
  // Check if the file was opened successfully
  if (file) {
    Serial.println("Reading from file...");
    
    // Read data from the file
    while (file.available()) {
      Serial.write(file.read());
    }
    
    // Close the file
    file.close();
    
    Serial.println("Data read successfully.");
  } else {
    Serial.println("Error opening file!");
  }
  
  delay(5000); // Wait for 5 seconds before repeating
}
