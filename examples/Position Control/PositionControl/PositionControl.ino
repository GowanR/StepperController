#include <StepperController.h>


#define DIR 4
#define STEP 3
#define SLEEP 5

unsigned long lastTime;
unsigned long dt;

StepperController motor0(STEP, DIR, SLEEP); // instantiate motor0 as a StepperController with the given pins 

void setup() {
    motor0.setPositionMode(); // set the motor to operate in position mode.
    motor0.setSpeed(200); // set the speed of the motor to 200 RPM, sign doesn't matter for position control.
    motor0.setPosition( 5.125 ); // set the motor to turn to 5.125 revolutions.
}


void loop() {
  lastTime = micros();
  motor0.update(dt); // update StepperController instance each loop
  // other arduino code goes here
  dt = micros() - lastTime;
}