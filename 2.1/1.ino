// Controlling servo motor with ultrasonic sensor, LCD and LEDs

#include <Servo.h>                // Include the Servo library
#include <LiquidCrystal_I2C.h>    // Include LCD I2C library

Servo myServo;                    // Define servo object
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD address 0x27, 16 columns, 2 rows

// Ultrasonic pins
const int trigPin = 7;
const int echoPin = 8;

// LED pins
const int RED_LED = 3;
const int GREEN_LED = 4;

void setup() {
  Serial.begin(9600);
  
  myServo.attach(9);              // Connect servo signal to pin 9
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  lcd.init();                     // Initialize the LCD
  lcd.backlight();                // Turn on the backlight
  lcd.clear();
}

void loop() {
  // --- Measure distance ---
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.0343 / 2; // Convert to cm

  // --- Display distance on LCD ---
  lcd.setCursor(0, 0);
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print(" cm");

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // --- Servo, LEDs, and LCD control ---
  if (distance >= 10 && distance <= 30) {
    // Map distance (10–30 cm) to servo angle (0–180°)
    int angle = map(distance, 10, 30, 0, 180);
    myServo.write(angle);
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("Angle: ");
    lcd.print(angle);
    lcd.print(" deg");

    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  } 
  else {
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("Out of range     "); // clear leftover text
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  }

  delay(300); // Small delay for stability
}
