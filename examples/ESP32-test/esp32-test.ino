#include <StepperController.h>
// #include <Arduino.h>
// "sketch": "examples\\ESP32-test\\esp32-test.ino",
unsigned long lastTime;
unsigned long dt;
StepperController m0 = StepperController(15,13,18);
// StepperController m1 = StepperController(14,36,12);
StepperController m1 = StepperController(14,19,12);

void setup() {
    lastTime = micros();

    // Motor definitions
    // m0.setPositionMode();
    m0.setSpeedMode();
    m1.setSpeedMode();
    // _dev.motor0.setPosition(10);
    m0.setSpeed(40);
    m1.setSpeed(40);
    // _dev.motor0.setPosition(1);
    m1.setStepControlMode(SIXTEENTH_STEP);
    m0.setStepControlMode(SIXTEENTH_STEP);

    m1.enable();
    m0.enable();
    m0.setPosition(0.5);
}
  
float a = 200;
void loop() {
  
  lastTime = micros();
  m0.update( dt );
  m1.update( dt );
  dt = micros() - lastTime;
  
}