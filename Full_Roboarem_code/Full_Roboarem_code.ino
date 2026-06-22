#include <Servo.h>

// Initialize two servo objects to control the grippers.
Servo servo;  
Servo servo1;

// Pin assignments for stepper motors controlling different axes.
const int stepX = 2;   // Slider (Belt)
const int dirX  = 3;   
const int stepY = 4;   // Base Rotation (Bottom)
const int dirY  = 5;   
const int stepZ = 6;   // Arm Joint 1
const int dirZ  = 7;   
const int stepA = 8;   // Arm Joint 2
const int dirA  = 10;  
const int enPin = 12;  // Enable pin for all stepper motors

// Pin assignments for the ultrasonic sensor
const int trigPin = 13;
const int echoPin = 30;

// Pin assignment for the limit switch
const int LIMIT_PIN = 31;  

// Variables for storing ultrasonic sensor readings.
long duration;
int distanceCm;
int oldValue = 0, newValue = 0;   

// Helper function to handle stepper movements cleanly
void moveStepper(int stepPin, int dirPin, int direction, int steps, int delayMicros) {
  digitalWrite(dirPin, direction);
  for(int x = 0; x < steps; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(delayMicros);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(delayMicros);
  }
}

void setup() {
  // Attach servos to their corresponding pins.
  servo.attach(9);   
  servo1.attach(11);

  // Set stepper and enable pins as output.
  pinMode(stepX, OUTPUT); pinMode(dirX, OUTPUT);  
  pinMode(stepY, OUTPUT); pinMode(dirY, OUTPUT); 
  pinMode(stepZ, OUTPUT); pinMode(dirZ, OUTPUT); 
  pinMode(stepA, OUTPUT); pinMode(dirA, OUTPUT); 
  pinMode(enPin, OUTPUT);

  // Set ultrasonic sensor pins.
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Use INPUT_PULLUP for safer, cleaner wiring without external resistors
  pinMode(LIMIT_PIN, INPUT_PULLUP);

  Serial.begin(9600);

  // Enable stepper motors (LOW enables most drivers like A4988/DRV8825).
  digitalWrite(enPin, LOW);  
}

void loop() {
  // Read the state of the limit switch (LOW = Pressed if using INPUT_PULLUP)
  int limitState = digitalRead(LIMIT_PIN);

  // Trigger the ultrasonic sensor to send a pulse.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the distance
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * 0.0340 / 2; 
  newValue = distanceCm;

  // Print the distance only if it changes significantly
  if(abs(newValue - oldValue) > 1) {
    Serial.print("Distance: ");
    Serial.concat(distanceCm);
    Serial.println(" cm");
    oldValue = newValue;
  }

  // Check if an object is detected within 10 cm.
  if(distanceCm > 0 && distanceCm <= 10) {
    Serial.println("--- Object Detected: Starting Sequence ---");

    // Move the gripper holder down to grab the object.
    Serial.println("Moving gripper holder down...");
    for (int pos = 0; pos <= 60; pos++) { 
      servo1.write(pos);             
      delay(15);                         
    }

    // Open the gripper to prepare to grab the object.
    Serial.println("Opening gripper...");
    for (int pos = 0; pos <= 180; pos++) { 
      servo.write(pos);
      delay(15);                                     
    }

    // Move Slider (X-axis) forward if limit switch isn't reached
    if(limitState == HIGH){ // HIGH means switch is open/not hit yet
      Serial.println("Moving stepper X slider forward...");
      moveStepper(stepX, dirX, HIGH, 1, 4000); 
      delay(1000);
    } 
    
    // Rotate Base (Y-axis)
    Serial.println("Moving stepper Y bottom...");  
    moveStepper(stepY, dirY, HIGH, 400, 1000);
    delay(1000);

    // Lower Arm Joint 1 (Z-axis)
    Serial.println("Moving stepper Z arm1...");
    moveStepper(stepZ, dirZ, HIGH, 200, 750);
    delay(1000);

    // Lower Arm Joint 2 (A-axis)
    Serial.println("Moving stepper A arm2...");
    moveStepper(stepA, dirA, HIGH, 100, 750);
    delay(1000);

    // -------- SECOND STEP: GRAB AND LIFT --------
    Serial.println("Closing the gripper...");
    for (int pos = 180; pos >= 0; pos--) {  
      servo.write(pos);                                           
      delay(15);                                             
    }
    
    Serial.println("Moving gripper holder up...");   
    for (int pos = 60; pos <= 180; pos++) { 
      servo1.write(pos);            
      delay(15);                                     
    }
    delay(1000);

    // Reverse Arm Joint 2
    Serial.println("Reversing stepper A...");  
    moveStepper(stepA, dirA, LOW, 100, 750);
    delay(1000);

    // Reverse Arm Joint 1
    Serial.println("Reversing stepper Z...");
    moveStepper(stepZ, dirZ, LOW, 200, 750);
    delay(1000);

    // Move Slider (X-axis) back if limit switch conditions require it
    if(limitState == LOW) { // Switch is pressed
      Serial.println("Moving stepper X slider back...");
      moveStepper(stepX, dirX, LOW, 1, 4000);
      delay(1000);
    }

    // Reverse Base Rotation (Y-axis)
    Serial.println("Reversing stepper Y...");      
    moveStepper(stepY, dirY, LOW, 200, 1000);
    delay(1000);

    // -------- THIRD STEP: DROP OFF --------
    Serial.println("Extending arm for drop off...");   
    moveStepper(stepZ, dirZ, HIGH, 200, 750);
    delay(1000);

    moveStepper(stepA, dirA, HIGH, 100, 750);
    delay(1000);

    // Fixed logic: Resets gripper holder back down to 0 degrees safely
    Serial.println("Moving gripper holder down for drop..."); 
    for (int pos = 180; pos >= 0; pos--) {
      servo1.write(pos);             
      delay(15);                                     
    }

    // Fixed logic: Safely opens gripper to release object (0 to 180)
    Serial.println("Opening gripper to release...");
    for (int pos = 0; pos <= 180; pos++) {
      servo.write(pos);              
      delay(15);                      
    }
    delay(1000);
    
    Serial.println("--- Sequence Complete ---");
  }

  delay(200); // Small cycle delay to prevent reading spam
}
