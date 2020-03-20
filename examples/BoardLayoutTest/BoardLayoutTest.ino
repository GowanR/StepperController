#include <StepperController.h>
#include <DevBoardLayout.h>
#include <Arduino.h>

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
    _dev.motor0.setSpeed(200);
    _dev.motor0.setPosition(i);
    
    // _dev.motor0.reverse();
    _dev.motor0.enable();
    // _dev.motor0.setPosition(100);

    _dev.motor1.setPositionMode();
    _dev.motor1.setStepControlMode(SIXTEENTH_STEP);
    _dev.motor1.setStepsPerRevolution(200*32);
    // pinMode(13, OUTPUT);
    // pinMode(53, OUTPUT);
    // pinMode(16, OUTPUT);
    // digitalWrite(13, HIGH);
    // digitalWrite(53, HIGH);
    // digitalWrite(16, LOW);
    _dev.motor1.invert();
    _dev.motor1.setSpeed(200);
    _dev.motor1.setPosition(1);
    
    _dev.motor1.enable();

    // _dev.motor2.setSpeedMode();
    // _dev.motor2.setSpeed(-20);
    // _dev.motor2.setStepControlMode(FULL_STEP);
    // _dev.motor2.enable();
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