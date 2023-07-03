#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_ADDR   0x3C // OLED display address (0x3C for 128x32, 0x3D for 128x64)

Adafruit_SSD1306 display(-1);

void setup() {
  // Initialize the OLED display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();

  // Set up some initial display settings
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  // Print a welcome message
  display.println("Hello, Arduino!");
  display.display();

  // Wait for a moment
  delay(2000);

  // Clear the display
  display.clearDisplay();
  display.display();
}

void loop() {
  // Print a simple counter on the OLED display
  for (int i = 0; i < 10; i++) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Count: ");
    display.print(i);
    display.display();
    delay(1000);
  }
}
