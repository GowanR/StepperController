#include <KiP33.h>

unsigned long lastTime;
unsigned long dt;
KiP33 _dev = KiP33();

int i = 1;

void setup() {
    lastTime = micros();
    // Motor definitions
    _dev.motor0.setPositionMode();
    // _dev.motor0.configureDriverCarrier(DRV8825);

    // _dev.motor0.setStepControlMode(FULL_STEP);
    // _dev.motor0.setStepControlMode(STEP_32);
    // _dev.motor0.setStepControlMode(QUARTER_STEP);
    // _dev.motor0.setStepsPerRevolution(200);
    _dev.motor0.setSpeed(60);
    _dev.motor0.setPosition(i);
    _dev.motor0.enable();

    _dev.motor1.setSpeedMode();
    _dev.motor1.configureDriverCarrier(DRV8825);
    _dev.motor1.setSpeed(200);
    _dev.motor1.reverse();
    _dev.motor1.enable();
}

bool turnFlag = true;
void loop() {
  lastTime = micros();
  if(_dev.motor0.isInPosition()){
    if(turnFlag) {
        i = 0;
        turnFlag = false;
    } else {
        i = 1;
        turnFlag = true;
    }
    _dev.motor0.setPosition(i);
    delay(500);
  }
  
  _dev.motor0.update( dt );
//   _dev.motor1.update( dt );
  dt = micros() - lastTime;
}