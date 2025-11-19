// Motor speed control with Start/Stop buttons and indicator LED

//library
///#include <Servo.h>           

const int transistorPin = 9;    // Transistor connected to pin 9 (controls motor)
const int startButton = 2;      // Start button connected to pin 2
const int stopButton = 3;       // Stop button connected to pin 3
const int greenLED = 4;         // Green LED connected to pin 4
const int redLED = 5;           // Red LED connected to pin 4
const int potPin = A3;          // Potentiometer output connected to analog pin 3
const int tempPin = A4;

int potVal = 0;                 // Variable to store the input from the potentiometer
bool motorRunning = false;      // Motor state flag

void setup() {
  Serial.begin(9600);
  pinMode(transistorPin, OUTPUT); 
  pinMode(startButton, INPUT);
  pinMode(stopButton, INPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  digitalWrite(transistorPin, LOW);   // Motor off at start
  digitalWrite(greenLED, LOW);        // LED off at start
}

void loop() {
  // --- Read states ---
  bool startPressed = digitalRead(startButton) == HIGH;  // pressed = LOW
  bool stopPressed = digitalRead(stopButton) == HIGH;
  //Temp
  int rawValue = analogRead(tempPin);
  float voltage = rawValue * (5.0 / 1024.0);
  float temperature = voltage * 100.0;  // LM35: 10mV per °C
  bool highTemp = temperature >= 25.0;


  //Speed of Motor
  potVal = analogRead(potPin);
  int speed = map(potVal, 0, 1023, 0, 255);

  // --- Handle motor control ---
  if (startPressed) {
    motorRunning = true;
    delay(200); // debounce delay
  }
  if (stopPressed) {
    motorRunning = false;
    delay(200); // debounce delay
  }
  if (highTemp){
    motorRunning = false;
    digitalWrite(redLED, HIGH);
  } else{
    digitalWrite(redLED, LOW);
  } 

  // --- Update outputs ---
  if (motorRunning) {
    analogWrite(transistorPin, speed);  // Motor ON
    digitalWrite(greenLED, HIGH);       // LED ON
  } else {
    analogWrite(transistorPin, 0);      // Motor OFF
    digitalWrite(greenLED, LOW);        // LED OFF
  }
}
