/*
  KiP33.h - This is a wrapper for working with the KiP board revision 3.3
*/
#ifndef __KIP33_H
#define __KIP33_H

#include "StepperController.h"

class KiP33 {
    public:
        KiP33();
        // define all three motors on KiP 3.3
        StepperController motor0 = StepperController( 22, 23, 24 );
        StepperController motor1 = StepperController( 25, 26, 27 );
        StepperController motor2 = StepperController( 28, 29, 30 );

        void configureMicroStepAllMotors(int microstepsPerStep);
        void enableAllMotors();
        void disableAllMotors();
        void update(unsigned long dt);
    private:

};
#endif