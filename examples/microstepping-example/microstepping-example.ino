#include <StepperController.h>

unsigned long lastTime;
unsigned long dt;

StepperController motor0(2, 3, 4); 

void setup() {
    motor0.setPositionMode();
    motor0.setPosition(1.0625);
    motor0.setSpeed(10);
    // Configure microstepping with pins:
    // MS1 = 5
    // MS2 = 6
    // MS3 = 7
    motor0.configureMicroSteppingPins(5, 6, 7);
    motor0.setStepControlMode(4); // configure motor to quarter step mode, 4 microsteps per step.
}


void loop() {
  lastTime = micros();
  motor0.update(dt);
  // other arduino code goes here
  dt = micros() - lastTime;
}