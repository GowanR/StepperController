#include <StepperController.h>
#include <Arduino.h>


#define DIR 4
#define STEP 3

unsigned long lastTime;
unsigned long dt;

StepperController motor0(STEP, DIR);

void setup() {

    lastTime = micros();

    motor0.setPositionControl();
    motor0.setSpeed(200); // set the speed of the motor to 200 RPM, sign doesn't matter.
    motor0.setPosition( 5.125 ); // set the motor to turn to 5.125 revolutions.
        
}


void loop() {
  lastTime = micros();
  motor0.update(dt);

  // other arduino code
  dt = micros() - lastTime;
}