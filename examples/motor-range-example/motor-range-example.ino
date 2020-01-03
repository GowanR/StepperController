#include <StepperController.h>

unsigned long lastTime;
unsigned long dt;

StepperController motor0(2, 3, 4); 

void setup() {
    motor0.setPositionMode();
    motor0.setPosition(99); // set motor position to 99 revolutions
    motor0.setSpeed(10);
    motor0.setRange(-1, 1); // the motor may not rotate to a position less than -1 revolution or a position greater than 1 revolution
}


void loop() {
  lastTime = micros();
  motor0.update(dt);
  // other arduino code goes here
  dt = micros() - lastTime;
}