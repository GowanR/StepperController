#include <StepperController.h>

unsigned long lastTime;
unsigned long dt;

StepperController motor0(2, 3, 4); 

// define positions for motor to rock between.
float positionOne = 8.f;
float positionTwo = -2.f;

// define travel speeds between positions.
float speedOne = 200;
float speedTwo = 50;

void setup() {
    motor0.setPositionMode();
    motor0.setPosition(positionOne);
    motor0.setSpeed(speedOne);

}


void loop() {
  lastTime = micros();
  motor0.update(dt);
  
  // Rocker Logic
  if(motor0.isInPosition()) {
      if (motor0.getPositionSetpoint() == positionOne){
        motor0.setPosition(positionTwo);
        motor0.setSpeed(speedTwo);
      } else if (motor0.getPositionSetpoint() == positionTwo){
        motor0.setPosition(positionOne);
        motor0.setSpeed(speedOne);
      }
      
  }


  dt = micros() - lastTime;
}