// Delay time for blinking and fading (in milliseconds)
const int delayTime = 500;

void setup() {
  // Set the LED pin as an output
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Turn on the LED
  digitalWrite(ledPin, HIGH);
  delay(delayTime);

  // Blink the LED
  digitalWrite(ledPin, LOW);
  delay(delayTime);
  digitalWrite(ledPin, HIGH);
  delay(delayTime);
  
  // Fade the LED in
  for (int brightness = 0; brightness <= 255; brightness++) {
    analogWrite(ledPin, brightness);
    delay(10);
  }
  
  // Fade the LED out
  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(ledPin, brightness);
    delay(10);
  }

  // Turn off the LED
  digitalWrite(ledPin, LOW);
  delay(delayTime);
}
