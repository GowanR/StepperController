/*
  KiP33.h - This is a wrapper for working with the KiP board revision 3.3
*/
#ifndef __KIP33_H
#define __KIP33_H

#define KIP33



#ifdef KIP33
// Stepper 0
    // Code pins
    #define STEPPER_0_STEP 22
    #define STEPPER_0_DIR 23
    #define STEPPER_0_SLEEP 24

    // Microstep pins
    #define STEPPER_0_MS1 10
    #define STEPPER_0_MS2 11
    #define STEPPER_0_MS3 12

// Stepper 1
    // Code pins
    #define STEPPER_1_STEP 25
    #define STEPPER_1_DIR 26
    #define STEPPER_1_SLEEP 27

    // Microstep pins
    #define STEPPER_1_MS1 13
    #define STEPPER_1_MS2 53
    #define STEPPER_1_MS3 16

// Stepper 2
    // Code pins
    #define STEPPER_2_STEP 28
    #define STEPPER_2_DIR 29
    #define STEPPER_2_SLEEP 30

    // Microstep pins
    #define STEPPER_2_MS1 17
    #define STEPPER_2_MS2 6
    #define STEPPER_2_MS3 7

#endif


#include "StepperController.h"
#include <BoardConfig.h>
class KiP33 {
    public:
        KiP33();
        // define all three motors on KiP 3.3
        #define STEPPER_0
        StepperController motor0 = StepperController(0);
        #undef STEPPER_0

        #define STEPPER_1
        StepperController motor1 = StepperController(1);
        #undef STEPPER_1

        #define STEPPER_2
        StepperController motor2 = StepperController(2);
        #undef STEPPER_2

        // void configureMicroStepAllMotors(int microstepsPerStep);
        // void enableAllMotors();
        // void disableAllMotors();
        // void update(unsigned long dt);
    private:

};




#endif