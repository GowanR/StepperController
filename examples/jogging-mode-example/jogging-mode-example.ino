#include <StepperController.h>

unsigned long lastTime;
unsigned long dt;

StepperController motor0(2, 3, 4); 
StepperController motor1(5, 6, 7); 
void setup() {
    
    // motor0 configured to demonstrate jogging mode
    motor0.setJogMode(); // set the motor to operate in jog mode.
    motor0.setSpeed(100); // set the speed of the motor to 100 RPM.
    
    // the following code will result in the motor0's position setpoint to be set to 10 revolutions in the positive direction.
    motor0.setJog(12); // set the motor to jog 12 revolutions in the positive direction.
    motor0.setJog(-2); // set the motor to jog 2 revolutions in the negative direction.

    // motor1 configured to demonstrate position mode compared to jogging mode.
    motor1.setPositionMode(); // set the motor to operate in position mode.
    motor1.setSpeed(100); // set the speed of the motor to 100 RPM.

    // the following will result in motor1's position setpoint to be -2 revolutions.
    motor1.setPosition(12);
    motor1.setPosition(-2);

}


void loop() {
  lastTime = micros();
  motor0.update(dt);
  motor1.update(dt);
  // other arduino code goes here
  dt = micros() - lastTime;
}