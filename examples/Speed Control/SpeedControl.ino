#include <StepperController.h>


#define DIR 4
#define STEP 3
#define SLEEP 5

unsigned long lastTime;
unsigned long dt;

StepperController motor0(STEP, DIR, SLEEP); // instantiate motor0 as a StepperController with the given pins 

void setup() {
    motor0.setSpeedMode(); // set the motor to operate in position mode.
    motor0.setSpeed(100); // set the speed of the motor to 100 RPM in the positive direction.
}


void loop() {
  lastTime = micros();
  motor0.update(dt); // update StepperController instance each loop
  // other arduino code goes here
  dt = micros() - lastTime;
}