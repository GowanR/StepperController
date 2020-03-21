#include <StepperController.h>
#include <DevBoardLayout.h>
#include<BoardConfig.h>
unsigned long lastTime;
unsigned long dt;
DevBoardLayout _dev = DevBoardLayout();

int i = 1;

void setup() {
    lastTime = micros();
    // Motor definitions
    _dev.motor0.setPositionMode();
    // _dev.motor0.setSpeedMode();

    // _dev.motor0.setStepControlMode(FULL_STEP);
    _dev.motor0.setStepControlMode(QUARTER_STEP);
    // _dev.motor0.setStepControlMode(QUARTER_STEP);
    _dev.motor0.setStepsPerRevolution(200 * 32);
    _dev.motor0.setSpeed(60);
    _dev.motor0.setPosition(i);
    _dev.motor0.enable();
}

bool _fullStepEnabled = false;
void loop() {
  lastTime = micros();
  if(_dev.motor0.isInPosition()){
    delay(1000);
    i++;
    _dev.motor0.setPosition(i);
  }
  
  _dev.motor0.update( dt );
  // _dev.motor1.update( dt );
  dt = micros() - lastTime;
}