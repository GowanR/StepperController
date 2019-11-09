#include <StepperController.h>
#include <DevBoardLayout.h>
#include <Arduino.h>

unsigned long lastTime;
unsigned long dt;
DevBoardLayout _dev = DevBoardLayout();

void setup() {
    lastTime = micros();

    // Motor definitions
    // _dev.motor0.setPositionMode();
    _dev.motor0.setSpeedMode();
    // _dev.motor0.setPosition(10);
    _dev.motor0.setSpeed(20);
    // _dev.motor0.setPosition(1);
    _dev.motor0.setStepControlMode(FULL_STEP);
    _dev.motor0.enable();
    // _dev.motor0.setPosition(10);

    // _dev.motor1.setPositionMode();
    // _dev.motor1.setPosition(-6);
    _dev.motor1.setSpeedMode();
    _dev.motor1.invert();
    // _dev.motor1.setPosition(10);
    _dev.motor1.setSpeed(20);
    _dev.motor1.setStepControlMode(FULL_STEP);
    _dev.motor1.enable();


    _dev.motor2.setSpeedMode();
    _dev.motor2.setSpeed(-20);
    _dev.motor2.setStepControlMode(FULL_STEP);
    _dev.motor2.enable();
    // _dev.servo0.write(90);
    // _dev.motor1.setPositionMode();
    // _dev.motor1.setPosition(10);
    // _dev.motor1.setSpeed(200);
    // _dev.motor1.enable();
    // _dev.motor1.setSlave(_dev.motor2);

    // _dev.motor2.enable();
    // _dev.servo0.write(15);
}

float a = 200;
void loop() {
  
  lastTime = micros();
  _dev.motor0.update( dt );
  _dev.motor1.update( dt );
  // _dev.motor2.update( dt );
  // _dev.servo0.write(90);
  dt = micros() - lastTime;
}