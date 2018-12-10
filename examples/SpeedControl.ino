#include <StepperController.h>
#include <Arduino.h>

// configure the pins connected
#define DIR 4
#define STEP 3
unsigned long lastTime;
unsigned long dt;


StepperController motor0(STEP, DIR);

void setup() {
    lastTime = micros();
    motor0.setSpeedMode();  // set the motor to speed mode
    motor0.setSpeed( 200 ); // tell the motor to run clockwise at 200 RPM
}


void loop() {
  lastTime = micros();
  motor0.update(dt);

  // other arduino code
  dt = micros() - lastTime;
}