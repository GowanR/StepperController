
/*
  KiP.h - This is the set of functions used for the hpe/gs version of kip.
  Author: gowan@asu.edu
  Version: 11/8/2019
*/

#ifndef __KIPCONTROLLER_H_
#define __KIPCONTROLLER_H_
#define PI 3.14159265359
#define WHEEL_OD 2.5870 // inches
#define AXLE_RADIUS 1.90945 // inches
#define DEGREE_OFFSET 10
#include "StepperController.h"

StepperController m0 = StepperController(15,13,18);
// StepperController m1 = StepperController(14,36,12);
StepperController m1 = StepperController(14,19,12);



void configMotors() {
  m1.setStepControlMode(SIXTEENTH_STEP);
  m0.setStepControlMode(SIXTEENTH_STEP);
  m0.setJogMode();
  m1.setJogMode();
  m1.setSpeed(40);
  m0.setSpeed(40);
  m0.enable();
  m1.enable();
  m1.invert(); // add reversed function for many calls
}


double inchesToRPM(double inches) {
  return  inches / (PI * WHEEL_OD);
}
double degreesToRadians(double degrees) {
  return (degrees * PI)/180.0;
}
double applyOffset(double degrees) {
  if (degrees > 0) {
    return degrees + DEGREE_OFFSET;
  }
  if (degrees < 0) {
    return degrees - DEGREE_OFFSET;
  }
  return degrees;
}
void turn(double degrees) {
  double radians = degreesToRadians(applyOffset(degrees));
  double rev = inchesToRPM(AXLE_RADIUS * radians);
  m0.setJog(rev);
  m1.setJog(-rev);
  unsigned long lastTime = 0;
  unsigned long dt = 0;
  while (true) {
    lastTime = micros();
    m0.update( dt );
    m1.update( dt );
    dt = micros() - lastTime;
    if (m0.isInPosition() && m1.isInPosition()){
      break;
    }
  }
}
void turnLeft(double degrees) {
  turn(degrees);
}
void turnRight(double degrees) {
  turn(-degrees);
}
void turnLeft() {
    turn(90);
}
void turnRight() {
  turn(-90);
  
}
void forward(double inches) {
  double rev = inchesToRPM(inches);
  m0.setJog(rev);
  m1.setJog(rev);
  unsigned long lastTime = 0;
  unsigned long dt = 0;
  while (true) {
    lastTime = micros();
    m0.update( dt );
    m1.update( dt );
    dt = micros() - lastTime;
    if (m0.isInPosition()){
      break;
    }
  }
}
void reverse(double inches) {
  forward(-1 * inches);
}

class KiP {
    public:
        
    
};


#endif