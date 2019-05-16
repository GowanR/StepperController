#include <StepperController.h>
#include <DevBoardLayout.h>
#include <Arduino.h>
//#include <Wire.h>

#define SLAVE_ADDRESS 0x04

unsigned long lastTime;
unsigned long dt;
DevBoardLayout _dev = DevBoardLayout();

void setup() {
    Wire.begin(0x8);
    Wire.onReceive(receiveEvent);
    lastTime = micros();

    // Motor definitions
    _dev.motor0.setSpeedMode();
    // _dev.motor0.setPosition(10);
    _dev.motor0.setSpeed(200);
    _dev.motor0.setPosition(1);
    _dev.motor0.enable();

    _dev.motor1.setSpeedMode();
    _dev.motor1.invert();
    _dev.motor1.setPosition(10);
    _dev.motor1.setSpeed(200);
    _dev.motor1.enable();


    _dev.motor2.setSpeedMode();
    _dev.motor2.setSpeed(-200);
    _dev.motor2.enable();
    _dev.servo0.write(90);
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
  _dev.motor2.update( dt );
  // _dev.servo0.write(90);
  dt = micros() - lastTime;
}

void receiveEvent(int howMany) {
  // while (Wire.available()) { // loop through all but the last
  //   char c = Wire.read(); // receive byte as a character
  //   if (c == 0x0) {
  //     _dev.motor2.setSpeed(200);
  //   } else if (c == 0x1) {
  //     _dev.motor2.setSpeed(-200);
  //   }
  // }
}