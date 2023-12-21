#include <Servo.h>

// Initialize two servo objects to control the grippers.
Servo servo;  
Servo servo1;

// Positions for servos indicating the degree of rotation.
int pos1 = 0;  
int pos = 0;  

// Pin assignments for stepper motors controlling different axes.
const int stepX = 2; // Stepper for the X-axis
const int dirX  = 3; // Direction for the X-axis
const int stepY = 4; // Stepper for the Y-axis
const int dirY  = 5; // Direction for the Y-axis
const int stepZ = 6; // Stepper for the Z-axis
const int dirZ  = 7; // Direction for the Z-axis
const int stepA = 8; // Stepper for an additional axis
const int dirA  = 10; // Direction for the additional axis
const int enPin = 12;  // Enable pin for stepper motors

// Pin assignments for the ultrasonic sensor used for distance measurement.
const int trigPin = 13;
const int echoPin = 30;

// Variables for storing ultrasonic sensor readings.
long duration;
int distanceCm, distanceInch;
int oldValue = 0 , newValue = 0;  

// Pin assignment for a limit switch to detect extreme positions.
int LIMIT = 31;  

void setup() {
  // Attach servos to their corresponding pins.
  servo.attach(9);  
  servo1.attach(11);

  // Set stepper and enable pins as output.
  pinMode(stepX, OUTPUT);
  pinMode(dirX, OUTPUT);  
  pinMode(stepY, OUTPUT); 
  pinMode(dirY, OUTPUT); 
  pinMode(stepZ, OUTPUT);
  pinMode(dirZ, OUTPUT); 
  pinMode(stepA, OUTPUT);
  pinMode(dirA, OUTPUT); 
  pinMode(enPin, OUTPUT);

  // Set ultrasonic sensor pins.
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Set the limit switch pin as input.
  pinMode(LIMIT, INPUT);

  // Initialize serial communication at 9600 bits per second.
  Serial.begin(9600);

  // Enable stepper motors.
  digitalWrite(enPin, LOW);  
}

void loop() {
  // Read the state of the limit switch.
  int LI = digitalRead(LIMIT);

  // Trigger the ultrasonic sensor to send a pulse.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the time between sending and receiving the pulse from the ultrasonic sensor.
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * 0.0340 / 2; // Convert time to distance.
  newValue = distanceCm;

  // Print the distance if it has changed.
  if(newValue != oldValue) {
    Serial.println(distanceCm);  // Print the distance in cm.
    oldValue = newValue;
  }

  // Short delay before the next loop iteration.
  delay(1000);

  // Check if an object is detected within 10 cm.
  if(distanceCm <= 10) {
    Serial.println("Object is detected");

    // Move the gripper holder to grab the object.
    for (pos = 0; pos <= 60; pos += 1) { 
      servo1.write(pos);             
      delay(15);    
      Serial.println("Moving gripper holder");                    
    }

    // Open the gripper to prepare to grab the object.
    for (pos = 0; pos <= 180; pos += 1) { 
      servo.write(pos);
      Serial.println("Opening gripper");             
      delay(15);                              
    }

    // Move the X-axis stepper if the limit switch is not engaged.
    if(LI == LOW){
      digitalWrite(dirX, HIGH); 
      digitalWrite(stepX, HIGH);
      delayMicroseconds(4000);
      digitalWrite(stepX, LOW);
      delay(1000);
      Serial.println("Moving stepper X slider");
    } 
   delay(1000);
   digitalWrite(dirY,HIGH);
   for(int x = 0; x < 400; x++) {  
   Serial.println("moving stepper Y bottom");  
   digitalWrite(stepY,HIGH);         
   delayMicroseconds(1000);           
   digitalWrite(stepY,LOW);          
   delayMicroseconds(1000);           
  }
   delay(1000);
  digitalWrite(dirZ,HIGH);
 for(int x = 0; x < 200; x++) {
   Serial.println("moving stepper Z arm1");
    digitalWrite(stepZ,HIGH);
     delayMicroseconds(750);
      digitalWrite(stepZ,LOW);
       delayMicroseconds(750);
  }
   delay(1000);
    digitalWrite(dirA,HIGH);
     Serial.println("moving stepper A arm2");
   for(int x = 0; x < 100; x++) {
    digitalWrite(stepA,HIGH);
     delayMicroseconds(750);
      digitalWrite(stepA,LOW);
       delayMicroseconds(750);
  }
   delay(1000);
  //////// SECOND STEP
   Serial.println("closing the grepper");
    for (pos = 180; pos >= 0; pos -= 1) {  
    servo.write(pos);                           
    delay(15);                                      
  }
   for (pos = 60; pos <= 180; pos += 1) { 
    servo1.write(pos); 
     Serial.println("moving grepper holder");            
    delay(15);                              
  }
   delay(1000);
    digitalWrite(dirA,LOW);
  for(int x = 0; x < 100; x++) {  
     Serial.println("moving stepper A");      
    digitalWrite(stepA,HIGH);              
     delayMicroseconds(750);                  
      digitalWrite(stepA,LOW);
       delayMicroseconds(750);
  }
   delay(1000);
   digitalWrite(dirZ,LOW);
   for(int x = 0; x < 200; x++) {
     Serial.println("moving stepper Z");
    digitalWrite(stepZ,HIGH);
     delayMicroseconds(750); 
      digitalWrite(stepZ,LOW);
       delayMicroseconds(750);
  }
   delay(1000);
    if(LI ==HIGH) {
 digitalWrite(dirX,LOW);
     Serial.println("moving stepper X slider");
    digitalWrite(stepX,HIGH);
     delayMicroseconds(4000);
      digitalWrite(stepX,LOW);
       delayMicroseconds(2000);
  }
   delay(1000);
    digitalWrite(dirY,LOW);
    for(int x = 0; x < 200; x++) {
       Serial.println("moving stepper Y");      
    digitalWrite(stepY,HIGH);           
     delayMicroseconds(1000);           
      digitalWrite(stepY,LOW);           
       delayMicroseconds(1000);      
    }
    ////////////////// third step
    delay(1000);
    digitalWrite(dirZ,HIGH);
  for(int x = 0; x < 200; x++) {   
     Serial.println("moving stepper Z");   
    digitalWrite(stepZ,HIGH);           
     delayMicroseconds(750);           
      digitalWrite(stepZ,LOW);           
       delayMicroseconds(750);            
  }
   delay(1000);
     digitalWrite(dirA,HIGH);
  for(int x = 0; x < 100; x++) {
     Serial.println("moving stepper A");
    digitalWrite(stepA,HIGH);
     delayMicroseconds(750);
      digitalWrite(stepA,LOW);
       delayMicroseconds(750);
  }
   delay(1000);
   for (pos = 100; pos <= 0; pos -= 1) {
     Serial.println("MOVING GREPPER HOLDER"); 
    servo1.write(pos);             
    delay(15);                              
  }
  for (pos = 0; pos >= 180; pos += 1) {
     Serial.println("moving GREPPER");
    servo.write(pos);              
    delay(15); 
     delay(1000);                      
  }
  }}
      
