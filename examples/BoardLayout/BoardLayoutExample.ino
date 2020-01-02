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
    _dev.motor0.setSpeed(100);
    _dev.motor0.setStepControlMode(FULL_STEP);
    // _dev.motor0.setStepControlMode(QUARTER_STEP);
    _dev.motor0.enable();
    // _dev.motor0.setPosition(100);

    _dev.motor1.setPositionMode();
    _dev.motor1.setStepControlMode(QUARTER_STEP);
    _dev.motor1.invert();
    _dev.motor1.setSpeed(100);
    _dev.motor1.setPosition(100);
    
    _dev.motor1.enable();

    // _dev.motor2.setSpeedMode();
    // _dev.motor2.setSpeed(-20);
    // _dev.motor2.setStepControlMode(FULL_STEP);
    // _dev.motor2.enable();
}

void loop() {
  lastTime = micros();
  _dev.motor0.update( dt );
  // _dev.motor1.update( dt );
  dt = micros() - lastTime;
}