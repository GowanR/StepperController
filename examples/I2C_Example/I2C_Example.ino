#include <Wire.h>
#include <StepperController.h>

#define DIR 4
#define STEP 3
#define SLEEP 5

unsigned long lastTime;
unsigned long dt;

StepperController motor0(STEP, DIR, SLEEP); // instantiate motor0 as a StepperController with the given pins 

void setup() {
  Wire.begin(0x8); // connect to i2c bus with address 0x8.
  Wire.onReceive(receiveEvent); // register receive event function
  motor0.setSpeedMode(); // set the motor to operate in position mode.
  motor0.setSpeed(0); // set the speed of the motor to zero so the rotor is stationary.
}

void loop() {
  lastTime = micros();
  motor0.update(dt); // update StepperController instance each loop
  // other arduino code goes here
  dt = micros() - lastTime;
}

void receiveEvent(int howMany) {
  while (Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    motor0.setSpeed((int)c); // set the motor speed to char cast as int
  }
}