#include <StepperController.h>

unsigned long lastTime;
unsigned long dt;

StepperController motor0(2, 3, 4);
StepperController motor1(5, 6, 7); 

void setup() {
    motor0.setPositionMode();
    motor0.setSpeed(200);
    motor0.setPosition( 5.125 );
    motor1.setPositionMode();
    motor1.setSpeed(200);
    motor1.setPosition(18.33);
    motor1.setSlave(motor0);
}


void loop() {
  lastTime = micros();
  motor0.update(dt);
  motor1.update(dt);
  // other arduino code goes here
  dt = micros() - lastTime;
}