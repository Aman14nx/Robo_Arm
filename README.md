🚀 Getting Started
1. Electrical Assembly Instructions
Driver Interface: Connect the Step and Direction line pins from your microstepping drivers (e.g., A4988, DRV8825, or TMC2208) directly to the mapped digital pins outlined in the mapping guide.

Global Enable Line: Tie all driver enable (EN) inputs together to Pin 12. Ensure your driver logic treats a low voltage signal (LOW) as active.

Limit Switch Calibration: Wire your hardware endstop switch configuration normally open (NO) across Pin 31 and GND. The embedded firmwares relies on integrated internal microcontroller pullups (INPUT_PULLUP), rendering external hardware resistors unnecessary.

Logic Isolation: Ensure high-current lines supplying stepper motors run via independent external bench power blocks, maintaining a shared common reference ground with the logic micro-controller unit.

2. Software Deployment
Open the Arduino IDE.

Ensure you have the native Servo.h library included in your compilation workspace environment.

Open src/robotic_arm_control.ino.

Connect your Target MCU board (e.g., Arduino Mega 2560 or Arduino Uno) via your local COM port interface.

Select target board configurations inside the IDE tools window and click Upload.

📊 Core Operational Sequence Flow
Scanning Phase: The firmware runs a non-blocking cyclic ping cycle utilizing the ultrasonic array sensor.

Target Acquired: When an object registers below 10cm parameters, the hardware execution loop locks and engages operational profiles.

Approach Sequence: The mechanical wrist pivots down and jaws spread wide.

Multi-Axis Alignment: The linear carriage, rotating base, and secondary links align sequentially toward the object coordinates.

Capture & Retraction: Jaws constrict securely, the mechanical link structure lifts, and the slider repositions clear of obstructions.

Drop & Reset: The system slews to target discharge zones, drops the held mass, and returns to baseline tracking loops.
