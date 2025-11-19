#include <AccelStepper.h>

// Stepper setup (4-wire)
#define MotorInterfaceType 4
AccelStepper myStepper(MotorInterfaceType, 8, 10, 9, 11);

// Constants
const float stepsPerRevolution = 2048.0;
const float degreesPerStep = 360.0 / stepsPerRevolution;

// Safe speed limit
const float MAX_SAFE_SPEED = 500.0; // steps per second

void setup() {
  Serial.begin(9600);
  myStepper.setAcceleration(200);
  Serial.println("Stepper Motor Control with Speed Protection");
}

void loop() {
  float speed = 0;
  float angle = 0;

  // Ask for speed
  Serial.println("Enter speed (steps/sec): ");
  while (Serial.available() == 0) {} // wait for input
  String speedInput = Serial.readStringUntil('\n');
  speedInput.trim();
  speed = speedInput.toFloat();

  // Apply speed protection
  if (speed <= 0) speed = 100;               // fallback for invalid input
  if (speed > MAX_SAFE_SPEED) speed = MAX_SAFE_SPEED; // limit speed
  myStepper.setMaxSpeed(speed);
  Serial.print("Speed set to: ");
  Serial.println(speed);

  // Ask for rotation angle
  Serial.println("Enter rotation angle (+/- degrees): ");
  while (Serial.available() == 0) {} // wait for input
  String angleInput = Serial.readStringUntil('\n');
  angleInput.trim();
  angle = angleInput.toFloat();
  long steps = angle / degreesPerStep;
  myStepper.move(steps);
  Serial.print("Rotating ");
  Serial.print(angle);
  Serial.println(" degrees");

  // Move the motor to the target
  while (myStepper.distanceToGo() != 0) {
    myStepper.run();
  }

  Serial.println("Done!\n");
}
