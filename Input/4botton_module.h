// Constants for button pins
const int BUTTON_1_PIN = 8;
const int BUTTON_2_PIN = 9;
const int BUTTON_3_PIN = 10;
const int BUTTON_4_PIN = 11;

// Variables to store button states
int button1State = 0;
int button2State = 0;
int button3State = 0;
int button4State = 0;

void setup() {
  // Set button pins as inputs
  pinMode(BUTTON_1_PIN, INPUT);
  pinMode(BUTTON_2_PIN, INPUT);
  pinMode(BUTTON_3_PIN, INPUT);
  pinMode(BUTTON_4_PIN, INPUT);

  // Enable internal pull-up resistors for the buttons
  digitalWrite(BUTTON_1_PIN, HIGH);
  digitalWrite(BUTTON_2_PIN, HIGH);
  digitalWrite(BUTTON_3_PIN, HIGH);
  digitalWrite(BUTTON_4_PIN, HIGH);

  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Read the state of each button
  button1State = digitalRead(BUTTON_1_PIN);
  button2State = digitalRead(BUTTON_2_PIN);
  button3State = digitalRead(BUTTON_3_PIN);
  button4State = digitalRead(BUTTON_4_PIN);

  // Print the state of each button only if it is pressed
  if (button1State == LOW) {
    Serial.println("Button 1 is pressed");
  }
  if (button2State == LOW) {
    Serial.println("Button 2 is pressed");
  }
  if (button3State == LOW) {
    Serial.println("Button 3 is pressed");
  }
  if (button4State == LOW) {
    Serial.println("Button 4 is pressed");
  }
  delay(1000); // Add a small delay to avoid reading the buttons too quickly
}
