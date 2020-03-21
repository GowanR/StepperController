#include "KiP33.h"

KiP33::KiP33() {
    // The following microstepping pin configurations are for the KiP PCB Rev. 3.3.
    // TODO: Fix microstepping for non-A4988 drivers. (custom truth tables)
    // motor0.configureMicroSteppingPins(10, 11, 12);
    // motor1.configureMicroSteppingPins(13, 53, 16);
    // motor2.configureMicroSteppingPins(17, 6, 7);
}

// void KiP33::configureMicroStepAllMotors(int microstepsPerStep) {
//     motor0.setStepControlMode(microstepsPerStep);
//     motor1.setStepControlMode(microstepsPerStep);
//     motor2.setStepControlMode(microstepsPerStep);
// }

// void KiP33::enableAllMotors() {
//     motor0.enable();
//     motor1.enable();
//     motor2.enable();
// }

// void KiP33::disableAllMotors() {
//     motor0.disable();
//     motor1.disable();
//     motor2.disable();
// }

// void KiP33::update(unsigned long dt) {
//     motor0.update(dt);
//     motor1.update(dt);
//     motor2.update(dt);
// }