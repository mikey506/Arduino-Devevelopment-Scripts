// Pin Definitions
const int echoPin = 2;  // Echo pin of HC-SR04 connected to Arduino D2
const int trigPin = 3;  // Trigger pin of HC-SR04 connected to Arduino D3

// Variables
long duration;  // Time taken by the ultrasonic wave to travel back and forth
int distance;   // Distance measured by the sensor

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  
  // Set pin modes
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  
  // Give the sensor some time to stabilize
  delay(2000);
}

void loop() {
  // Send a short pulse to trigger the sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Measure the duration of the pulse from the sensor
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance in centimeters
  distance = duration * 0.034 / 2;
  
  // Print the distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Delay before the next measurement
  delay(1000);
}
