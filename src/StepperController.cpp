/*
  StepperController.cpp - Library for controlling multiple A4988 chips concurrently with dynamic motion profiles.
*/

#include <Arduino.h>
#include "StepperController.h"
#include "KiP33.h"
#define HIGH 1
#define LOW 0
#define INPUT 1
#define OUTPUT 0
#define KIP33

#define STEPS_PER_REVOLUTION 200 
/**
 * Constructs the motor controller.
 * @param the Arduino pin used for step control.
 * @param the Arduino pin used for direction control.
 */
StepperController::StepperController(unsigned int motorID) {
    _motorID = motorID;
    _carrierBoardType = A4988;
    _inPosition = false;
    _stepControlMode = FULL_STEP;
    _mode = jog;                          // default mode is jog
    _currentPosition = 0;                 // starting position of motor is 0 steps, this stacks half steps
    _currentPositionRevolutions = 0;
    _motorEnabled = true;                 // true when the motor can move
    _stepActive = false;                  // keeps track of if the coil powered or not
    _upperSoftStop = 0;                   // set upper soft stop to 0
    _lowerSoftStop = 0;                   // set lower soft stop to 0
    _positionSetpoint = 0;                // the default position setpoint is zero
    _isInverted = false;                  // the motor is not inverted by default
    _stepsPerRevolution = STEPS_PER_REVOLUTION * _stepControlMode;
    _sleepOnDisable = true;

    /////// Configure Step, Direction, and Sleep pins for fast writes ///////
    #ifdef KIP33
        if(motorID == 0) {
            // Stepper 0 Core Pin Configuration
            pinModeFast( STEPPER_0_STEP, OUTPUT );
            pinModeFast( STEPPER_0_DIR, OUTPUT );
            pinModeFast( STEPPER_0_SLEEP, OUTPUT );
            // Stepper 0 Microstepping Pin Configuration
            pinModeFast( STEPPER_0_MS1, OUTPUT );
            pinModeFast( STEPPER_0_MS2, OUTPUT );
            pinModeFast( STEPPER_0_MS3, OUTPUT );
        }
    #endif

    #if defined(KIP33)
        if(motorID == 1) {
            // Stepper 1 Core Pin Configuration
            pinModeFast( STEPPER_1_STEP, OUTPUT );
            pinModeFast( STEPPER_1_DIR, OUTPUT );
            pinModeFast( STEPPER_1_SLEEP, OUTPUT );
            // Stepper 1 Microstepping Pin Configuration
            pinModeFast( STEPPER_1_MS1, OUTPUT );
            pinModeFast( STEPPER_1_MS2, OUTPUT );
            pinModeFast( STEPPER_1_MS3, OUTPUT );
        }
    #endif

    #if defined(KIP33)
        if(motorID == 2) {
            // Stepper 2 Core Pin Configuration
            pinModeFast( STEPPER_2_STEP, OUTPUT );
            pinModeFast( STEPPER_2_DIR, OUTPUT );
            pinModeFast( STEPPER_2_SLEEP, OUTPUT );
            // Stepper 2 Microstepping Pin Configuration
            pinModeFast( STEPPER_2_MS1, OUTPUT );
            pinModeFast( STEPPER_2_MS2, OUTPUT );
            pinModeFast( STEPPER_2_MS3, OUTPUT );
        }
    #endif
    
}



/**
 * This method changes the microstepping mode for the motor.
 * ![WARN]: this source is currently voilitile in that it will reset motor position when microstepping configuration is altered.
 */
void StepperController::setStepControlMode(StepControlMode mode) {    
    _stepControlMode = mode;
    _stepsPerRevolution = STEPS_PER_REVOLUTION * mode;
    _currentPosition = 0;
    // _currentPosition = (long long)(_currentPositionRevolutions * _stepsPerRevolution);
    updateMicroStepConfiguration();
}


// TODO: update microstepping for new macros
void StepperController::updateMicroStepConfiguration() {
    
    switch(_carrierBoardType) {
        case A4988:
            // handleMicrostepA4988();
        break;

        case DRV8825:
            // handleMicrostepDRV8825();
        break;

        case DRV8834:
        break;
        
        case DRV8880:
        break;
        
        case MP6500_POT_CC:
        break;
        
        case MP6500_DIGITAL_CC:
        break;
        
        case TB67S279FTG:
        break;
        
        case TB67S249FTG:
        break;
        
        case STSPIN820:
        break;
        
        case STSPIN220:
        break;
    }
}
// void StepperController::handleMicrostepA4988() {
//     #ifdef KIP33
//     switch(_stepControlMode) {
//         case FULL_STEP:
//             switch(_motorID){
//                 case 0:
//                     digitalWriteFast(STEPPER_0_MS1, ((int)(A4988_1&0b100) > 0));
//                     digitalWriteFast(STEPPER_0_MS2, ((int)(A4988_1&0b010) > 0));
//                     digitalWriteFast(STEPPER_0_MS3, ((int)(A4988_1&0b001) > 0));
//                 break;
//                 case 1:
//                     digitalWriteFast(STEPPER_1_MS1, ((int)(A4988_1&0b100) > 0));
//                     digitalWriteFast(STEPPER_1_MS2, ((int)(A4988_1&0b010) > 0));
//                     digitalWriteFast(STEPPER_1_MS3, ((int)(A4988_1&0b001) > 0));
//                 break;
//                 case 2:
//                     digitalWriteFast(STEPPER_2_MS1, ((int)(A4988_1&0b100) > 0));
//                     digitalWriteFast(STEPPER_2_MS2, ((int)(A4988_1&0b010) > 0));
//                     digitalWriteFast(STEPPER_2_MS3, ((int)(A4988_1&0b001) > 0));
//                 break;
//             }
//         break;
//         case HALF_STEP:
//             switch(_motorID){
//                 case 0:
//                     digitalWriteFast(STEPPER_0_MS1, ((int)(A4988_2&0b100) > 0));
//                     digitalWriteFast(STEPPER_0_MS2, ((int)(A4988_2&0b010) > 0));
//                     digitalWriteFast(STEPPER_0_MS3, ((int)(A4988_2&0b001) > 0));
//                 break;
//                 case 1:
//                     digitalWriteFast(STEPPER_1_MS1, ((int)(A4988_2&0b100) > 0));
//                     digitalWriteFast(STEPPER_1_MS2, ((int)(A4988_2&0b010) > 0));
//                     digitalWriteFast(STEPPER_1_MS3, ((int)(A4988_2&0b001) > 0));
//                 break;
//                 case 2:
//                     digitalWriteFast(STEPPER_2_MS1, ((int)(A4988_2&0b100) > 0));
//                     digitalWriteFast(STEPPER_2_MS2, ((int)(A4988_2&0b010) > 0));
//                     digitalWriteFast(STEPPER_2_MS3, ((int)(A4988_2&0b001) > 0));
//                 break;
//             }
//         break;
//         case QUARTER_STEP:
//             switch(_motorID){
//                 case 0:
//                     digitalWriteFast(STEPPER_0_MS1, ((int)(A4988_4&0b100) > 0));
//                     digitalWriteFast(STEPPER_0_MS2, ((int)(A4988_4&0b010) > 0));
//                     digitalWriteFast(STEPPER_0_MS3, ((int)(A4988_4&0b001) > 0));
//                 break;
//                 case 1:
//                     digitalWriteFast(STEPPER_1_MS1, ((int)(A4988_4&0b100) > 0));
//                     digitalWriteFast(STEPPER_1_MS2, ((int)(A4988_4&0b010) > 0));
//                     digitalWriteFast(STEPPER_1_MS3, ((int)(A4988_4&0b001) > 0));
//                 break;
//                 case 2:
//                     digitalWriteFast(STEPPER_2_MS1, ((int)(A4988_4&0b100) > 0));
//                     digitalWriteFast(STEPPER_2_MS2, ((int)(A4988_4&0b010) > 0));
//                     digitalWriteFast(STEPPER_2_MS3, ((int)(A4988_4&0b001) > 0));
//                 break;
//             }
//         break;
//         case EIGHTH_STEP:
//             switch(_motorID){
//                 case 0:
//                     digitalWriteFast(STEPPER_0_MS1, ((int)(A4988_8&0b100) > 0));
//                     digitalWriteFast(STEPPER_0_MS2, ((int)(A4988_8&0b010) > 0));
//                     digitalWriteFast(STEPPER_0_MS3, ((int)(A4988_8&0b001) > 0));
//                 break;
//                 case 1:
//                     digitalWriteFast(STEPPER_1_MS1, ((int)(A4988_8&0b100) > 0));
//                     digitalWriteFast(STEPPER_1_MS2, ((int)(A4988_8&0b010) > 0));
//                     digitalWriteFast(STEPPER_1_MS3, ((int)(A4988_8&0b001) > 0));
//                 break;
//                 case 2:
//                     digitalWriteFast(STEPPER_2_MS1, ((int)(A4988_8&0b100) > 0));
//                     digitalWriteFast(STEPPER_2_MS2, ((int)(A4988_8&0b010) > 0));
//                     digitalWriteFast(STEPPER_2_MS3, ((int)(A4988_8&0b001) > 0));
//                 break;
//             }
//         break;
//         case SIXTEENTH_STEP:
//         switch(_motorID){
//                 case 0:
//                     digitalWriteFast(STEPPER_0_MS1, ((int)(A4988_16&0b100) > 0));
//                     digitalWriteFast(STEPPER_0_MS2, ((int)(A4988_16&0b010) > 0));
//                     digitalWriteFast(STEPPER_0_MS3, ((int)(A4988_16&0b001) > 0));
//                 break;
//                 case 1:
//                     digitalWriteFast(STEPPER_1_MS1, ((int)(A4988_16&0b100) > 0));
//                     digitalWriteFast(STEPPER_1_MS2, ((int)(A4988_16&0b010) > 0));
//                     digitalWriteFast(STEPPER_1_MS3, ((int)(A4988_16&0b001) > 0));
//                 break;
//                 case 2:
//                     digitalWriteFast(STEPPER_2_MS1, ((int)(A4988_16&0b100) > 0));
//                     digitalWriteFast(STEPPER_2_MS2, ((int)(A4988_16&0b010) > 0));
//                     digitalWriteFast(STEPPER_2_MS3, ((int)(A4988_16&0b001) > 0));
//                 break;
//             }
//         break;
//     }
//     #endif
// }

// void StepperController::handleMicrostepDRV8825() {
//     #ifdef KIP33
//     switch(_stepControlMode) {
//         case FULL_STEP:
//             switch(_motorID){
//                 case 0:
//                     digitalWriteFast(STEPPER_0_MS1, ((int)(DRV8825_1&0b100) > 0));
//                     digitalWriteFast(STEPPER_0_MS2, ((int)(DRV8825_1&0b010) > 0));
//                     digitalWriteFast(STEPPER_0_MS3, ((int)(DRV8825_1&0b001) > 0));
//                 break;
//                 case 1:
//                     digitalWriteFast(STEPPER_1_MS1, ((int)(DRV8825_1&0b100) > 0));
//                     digitalWriteFast(STEPPER_1_MS2, ((int)(DRV8825_1&0b010) > 0));
//                     digitalWriteFast(STEPPER_1_MS3, ((int)(DRV8825_1&0b001) > 0));
//                 break;
//                 case 2:
//                     digitalWriteFast(STEPPER_2_MS1, ((int)(DRV8825_1&0b100) > 0));
//                     digitalWriteFast(STEPPER_2_MS2, ((int)(DRV8825_1&0b010) > 0));
//                     digitalWriteFast(STEPPER_2_MS3, ((int)(DRV8825_1&0b001) > 0));
//                 break;
//             }
//         break;
//         case HALF_STEP:
//             switch(_motorID){
//                 case 0:
//                     digitalWriteFast(STEPPER_0_MS1, ((int)(DRV8825_2&0b100) > 0));
//                     digitalWriteFast(STEPPER_0_MS2, ((int)(DRV8825_2&0b010) > 0));
//                     digitalWriteFast(STEPPER_0_MS3, ((int)(DRV8825_2&0b001) > 0));
//                 break;
//                 case 1:
//                     digitalWriteFast(STEPPER_1_MS1, ((int)(DRV8825_2&0b100) > 0));
//                     digitalWriteFast(STEPPER_1_MS2, ((int)(DRV8825_2&0b010) > 0));
//                     digitalWriteFast(STEPPER_1_MS3, ((int)(DRV8825_2&0b001) > 0));
//                 break;
//                 case 2:
//                     digitalWriteFast(STEPPER_2_MS1, ((int)(DRV8825_2&0b100) > 0));
//                     digitalWriteFast(STEPPER_2_MS2, ((int)(DRV8825_2&0b010) > 0));
//                     digitalWriteFast(STEPPER_2_MS3, ((int)(DRV8825_2&0b001) > 0));
//                 break;
//             }
//         break;
//         case QUARTER_STEP:
//             switch(_motorID){
//                 case 0:
//                     digitalWriteFast(STEPPER_0_MS1, ((int)(DRV8825_4&0b100) > 0));
//                     digitalWriteFast(STEPPER_0_MS2, ((int)(DRV8825_4&0b010) > 0));
//                     digitalWriteFast(STEPPER_0_MS3, ((int)(DRV8825_4&0b001) > 0));
//                 break;
//                 case 1:
//                     digitalWriteFast(STEPPER_1_MS1, ((int)(DRV8825_4&0b100) > 0));
//                     digitalWriteFast(STEPPER_1_MS2, ((int)(DRV8825_4&0b010) > 0));
//                     digitalWriteFast(STEPPER_1_MS3, ((int)(DRV8825_4&0b001) > 0));
//                 break;
//                 case 2:
//                     digitalWriteFast(STEPPER_2_MS1, ((int)(DRV8825_4&0b100) > 0));
//                     digitalWriteFast(STEPPER_2_MS2, ((int)(DRV8825_4&0b010) > 0));
//                     digitalWriteFast(STEPPER_2_MS3, ((int)(DRV8825_4&0b001) > 0));
//                 break;
//             }
//         break;
//         case EIGHTH_STEP:
//             switch(_motorID){
//                 case 0:
//                     digitalWriteFast(STEPPER_0_MS1, ((int)(DRV8825_8&0b100) > 0));
//                     digitalWriteFast(STEPPER_0_MS2, ((int)(DRV8825_8&0b010) > 0));
//                     digitalWriteFast(STEPPER_0_MS3, ((int)(DRV8825_8&0b001) > 0));
//                 break;
//                 case 1:
//                     digitalWriteFast(STEPPER_1_MS1, ((int)(DRV8825_8&0b100) > 0));
//                     digitalWriteFast(STEPPER_1_MS2, ((int)(DRV8825_8&0b010) > 0));
//                     digitalWriteFast(STEPPER_1_MS3, ((int)(DRV8825_8&0b001) > 0));
//                 break;
//                 case 2:
//                     digitalWriteFast(STEPPER_2_MS1, ((int)(DRV8825_8&0b100) > 0));
//                     digitalWriteFast(STEPPER_2_MS2, ((int)(DRV8825_8&0b010) > 0));
//                     digitalWriteFast(STEPPER_2_MS3, ((int)(DRV8825_8&0b001) > 0));
//                 break;
//             }
//         break;
//         case SIXTEENTH_STEP:
//             switch(_motorID){
//                 case 0:
//                     digitalWriteFast(STEPPER_0_MS1, ((int)(DRV8825_16&0b100) > 0));
//                     digitalWriteFast(STEPPER_0_MS2, ((int)(DRV8825_16&0b010) > 0));
//                     digitalWriteFast(STEPPER_0_MS3, ((int)(DRV8825_16&0b001) > 0));
//                 break;
//                 case 1:
//                     digitalWriteFast(STEPPER_1_MS1, ((int)(DRV8825_16&0b100) > 0));
//                     digitalWriteFast(STEPPER_1_MS2, ((int)(DRV8825_16&0b010) > 0));
//                     digitalWriteFast(STEPPER_1_MS3, ((int)(DRV8825_16&0b001) > 0));
//                 break;
//                 case 2:
//                     digitalWriteFast(STEPPER_2_MS1, ((int)(DRV8825_16&0b100) > 0));
//                     digitalWriteFast(STEPPER_2_MS2, ((int)(DRV8825_16&0b010) > 0));
//                     digitalWriteFast(STEPPER_2_MS3, ((int)(DRV8825_16&0b001) > 0));
//                 break;
//             }
//         break;
//         case STEP_32:
//             switch(_motorID){
//                 case 0:
//                     digitalWriteFast(STEPPER_0_MS1, ((int)(DRV8825_32&0b100) > 0));
//                     digitalWriteFast(STEPPER_0_MS2, ((int)(DRV8825_32&0b010) > 0));
//                     digitalWriteFast(STEPPER_0_MS3, ((int)(DRV8825_32&0b001) > 0));
//                 break;
//                 case 1:
//                     digitalWriteFast(STEPPER_1_MS1, ((int)(DRV8825_32&0b100) > 0));
//                     digitalWriteFast(STEPPER_1_MS2, ((int)(DRV8825_32&0b010) > 0));
//                     digitalWriteFast(STEPPER_1_MS3, ((int)(DRV8825_32&0b001) > 0));
//                 break;
//                 case 2:
//                     digitalWriteFast(STEPPER_2_MS1, ((int)(DRV8825_32&0b100) > 0));
//                     digitalWriteFast(STEPPER_2_MS2, ((int)(DRV8825_32&0b010) > 0));
//                     digitalWriteFast(STEPPER_2_MS3, ((int)(DRV8825_32&0b001) > 0));
//                 break;
//             }
//         break;
//     }
//     #endif
// }
/**
 * Converts seconds into microseconds.
 * @param seconds to convert
 */
unsigned long StepperController::secondsToMicros( double seconds ) {
    return (unsigned long) (seconds * 1000000.f);
}

/**
 * Gets the current mode of the motor.
 * @returns mode of the motor (0-3)
 *  --------------
 * | speed    | 0 |
 * | jog      | 1 |
 * | position | 2 |
 * | profile  | 3 |
 *  --------------
 */
int StepperController::getMode() {
    return (int) _mode;
}

double StepperController::getSpeed() {
    return _currentSpeed;
}

/**
 * Inverts the motor direction. Toggles the default direction for sign.
 */
void StepperController::invert() {
    _isInverted = !_isInverted;
}

void StepperController::reverse() {
    _isInverted = true;
}

/**
 * Sets how many steps the motor must make to turn one revolution.
 * @param number of steps per revolution
 */
void StepperController::setStepsPerRevolution( int steps ) {
    _stepsPerRevolution = steps;
}

/**
 * Gets the adjusted direction that takes the inverted state into account
 * @returns the adjusted direction
 */
bool StepperController::getDirection() {
    return _isInverted ? !_direction : _direction;
}

/**
 * Gives this motor a slave motor.
 * The motor passed will now follow the exact same step instructions as this motor.
 * @param address of motor to make slave
 * @param address of motor to make slave
 */
void StepperController::setSlave(StepperController &motor) {
    _hasSlave = true;
    _slave = &motor;
    _slave->disable(); // prevent motor form interpreting any other commands.
}

/**
 * Clears the slave and sets it free.
 */
void StepperController::clearSlave() {
    _hasSlave = false;
    _slave->enable(); // allow the motor to be controlled by the outside world again.
}

/**
 * Typical stepper drivers will step on the leading edge of the square wave.
 */ 
void StepperController::step() {

    if (getDirection()) {
        #if defined(KIP33) && (defined(STEPPER_0_DIR) || defined(STEPPER_1_DIR) || defined(STEPPER_2_DIR))
        switch(_motorID) {
            case 0: 
            digitalWriteFast( STEPPER_0_DIR, HIGH);
            // digitalWrite( STEPPER_0_DIR, HIGH);
            break;
            case 1: digitalWriteFast( STEPPER_1_DIR, HIGH);
            break;
            case 2: digitalWriteFast( STEPPER_2_DIR, HIGH);
            break;
        }
        #endif
        
    } else {
        #if defined(KIP33) && (defined(STEPPER_0_DIR) || defined(STEPPER_1_DIR) || defined(STEPPER_2_DIR))
        switch(_motorID) {
            case 0: 
            digitalWriteFast( STEPPER_0_DIR, LOW);
            // digitalWrite( STEPPER_0_DIR, LOW);
            break;
            case 1: digitalWriteFast( STEPPER_1_DIR, LOW);
            break;
            case 2: digitalWriteFast( STEPPER_2_DIR, LOW);
            break;
        }
        #endif
    }
    
    
    #if defined(KIP33) && (defined(STEPPER_0_STEP) || defined(STEPPER_1_STEP) || defined(STEPPER_2_STEP))
    switch (_motorID) {
        case 0: 
        // digitalWriteFast( STEPPER_0_STEP, HIGH);
        digitalWrite( STEPPER_0_STEP, HIGH);
        break;
        case 1: digitalWriteFast( STEPPER_1_STEP, HIGH);
        break;
        case 2: digitalWriteFast( STEPPER_2_STEP, HIGH);
        break;
    }
    _stepActive = true;
    _timeSinceLastStep = 0;
    _direction ? _currentPosition++ : _currentPosition--; // if direction true, conventionally clockwise, then increment the current position. if negative, decrement. 
    
    // tracks position in revolutions
    if (getDirection()) {
        _currentPositionRevolutions += 1.f/((double)_stepsPerRevolution);
    } else {
        _currentPositionRevolutions -= 1.f/((double)_stepsPerRevolution);
    }
    
    if ( _hasSlave ) {
        _slave->step();
    }
    
    
    _stepActive = false;
    // digitalWriteFast( STEPPER_0_STEP, LOW);
    // digitalWriteFast( STEPPER_1_STEP, LOW);
    // digitalWriteFast( STEPPER_2_STEP, LOW);
    switch (_motorID) {
        case 0: 
        // digitalWriteFast( STEPPER_0_STEP, LOW);
        digitalWrite( STEPPER_0_STEP, LOW);
        break;
        case 1: digitalWriteFast( STEPPER_1_STEP, LOW);
        break;
        case 2: digitalWriteFast( STEPPER_2_STEP, LOW);
        break;
    }
    #endif

    
}

/**
 * Helper function givem rotations per minute will compute the microsecond delay between steps.
 * @param the roations per minute
 * @returns microsecond interval between steps
 */
unsigned long StepperController::rpmToMicros( double RPM ) {
    return (unsigned long)((60.f * 1e6) / (fabs(RPM) * ((double)_stepsPerRevolution)));
}

/**
 * Converts rotations into approximate discrete steps.
 * @param number of rotations to convert
 * @return number of steps the motor needs to take to equate to approximate rotations.
 */
long long StepperController::rotationsToSteps( double rotations ) {
    return (rotations * (double)_stepsPerRevolution);
}

/**
 * Function used to get the current position of the motor in revolutions.
 * @returns the current position of the motor in revolutions
 */
double StepperController::getPosition() {
    return _currentPosition / (_stepsPerRevolution);
}

// long StepperController::getRawPosition() {
//     return _currentPosition
// }

/**
 * Funciton that will set the motor's speed in revolutions per minute. Uses speed sign as direction.
 * This function only effects motor behavior in modes: jog, speed, position.
 * @param speed in revolutions per minute.
 */
void StepperController::setSpeed( double speed ) {
    _direction = speed > 0;
    _currentSpeed = speed;
}

/**
 * Sets the 'soft' stops of the motor. This bound the motor to up to two values in revolutions.
 * 
 * @param the lower soft stop value in revolutions.
 * @param the upper soft stop value in revolutions.
 */
void StepperController::setRange( double min, double max ) {
    _lowerSoftStop = rotationsToSteps(min);
    _upperSoftStop = rotationsToSteps(max);
}

/**
 * This sets the positional setpoint of the motor in revolutions.
 * @param the absolute 'goal' revolutions value.
 */
void StepperController::setPosition( double setpoint ) {
    _positionSetpoint = rotationsToSteps(setpoint);
    _revPositionSetpoint = setpoint;
}

/**
 * Sets the current position of the motor to the soft zero (0) positon.
 */
void StepperController::tare() {
    _currentPosition = 0;
    _currentPositionRevolutions = 0;
}

void StepperController::setSleepOnDisable(bool sleep) {
    _sleepOnDisable = sleep;
}

/** 
 * Allows the motor to move when updated.
 */
void StepperController::enable() {
    _motorEnabled = true;

    #ifdef KIP33
    switch(_motorID) {
        case 0: digitalWriteFast(STEPPER_0_SLEEP, HIGH);
        break;
        case 1: digitalWriteFast(STEPPER_1_SLEEP, HIGH);
        break;
        case 2: digitalWriteFast(STEPPER_2_SLEEP, HIGH);
        break;
    }
    #endif

}

/**
 * Prevents the motor from moving when updated.
 */
void StepperController::disable() {
    _motorEnabled = false;
    if(_sleepOnDisable) {
        #ifdef KIP33
        switch(_motorID) {
            case 0: digitalWriteFast(STEPPER_0_SLEEP, LOW);
            break;
            case 1: digitalWriteFast(STEPPER_1_SLEEP, LOW);
            break;
            case 2: digitalWriteFast(STEPPER_2_SLEEP, LOW);
            break;
        }
        #endif
    }
}

/**
 * Sets the motor mode to jog.
 * Jog mode is based on sending the motor a command to move x revolutions in a given direciton.
 */
void StepperController::setJogMode() {
    _mode = jog;
}

/**
 * Sets the motor mode to speed.
 * Speed mode tells the motor to spin in a direction at a specified RPM
 */
void StepperController::setSpeedMode() {
    _mode = speed;
}

/**
 * Sets the motor mode to postion.
 * Position mode is where the motor will turn towards a specific 'goal' position.
 * The motor will move to the specified poisition at the specifed speed.
 */
void StepperController::setPositionMode(){
    _mode = position;
}

/**
 * Sets the motor mode to follow a specific motion profile.
 * The motor will run with control of both positon, velocity, and acceleration.
 * The motor will abide by a position, time map as long as it's physically possible for the given motor.
 */
void StepperController::setProfileMode(){
    _mode = profile;
}

/**
 * Commands the motor to jog a certain number of rotations. Uses sign as direction.
 * @param number of roations to move relative to current motor position.
 */
void StepperController::setJog( double rotations ) {
    _positionSetpoint = _currentPosition + ( rotationsToSteps( rotations ));
}

/**
 * Checks to see if the motor is withing the set soft stop range with consideration of direciton.
 * @returns if the motor is in allowable range and will not break range in direction.
 */
bool StepperController::motorInRange(){
    if ( _upperSoftStop != 0 && _lowerSoftStop != 0 ) {
        if ( _currentPosition >= _upperSoftStop && _direction ) {
            return false;
        }
        if ( _currentPosition <= _lowerSoftStop && !_direction ) {
            return false;
        }
    }
    return true;
}

/**
 * Updates the motor when it's in speed mode.
 * @param the change in time form last loop.
 */
void StepperController::updateSpeedMode( unsigned long dt ) {
    if ( !motorInRange() ) {
        return;
    }
    unsigned long _delayTime = rpmToMicros(_currentSpeed);
    if ( _timeSinceLastStep >= _delayTime ) {
        step();
    }
    _timeSinceLastStep += dt;
}

double StepperController::getPositionSetpoint() {
    return _positionSetpoint;
}

double StepperController::getPositionSetpointRevolutions(){
    return _revPositionSetpoint;
}

bool StepperController::isInPosition(){
    return _inPosition;
}

void StepperController::configureDriverCarrier(DriverCarrierBoard boardType) {
    _carrierBoardType = boardType;
}

/**
 * Updates the motor when it's in position mode.
 * @param the change in time form last loop.
 */
unsigned long _delayTime;
void StepperController::updatePositionMode( unsigned long dt ) {
    // if ( !motorInRange() ) {
    //     return;
    // }
    _delayTime = rpmToMicros(_currentSpeed);
    
    // check if motor is in position according to setpoint
    if ( _currentPosition == _positionSetpoint ) {
        _inPosition = true;
        return;
    }
    _inPosition = false;


    _direction = ( _positionSetpoint - _currentPosition ) > 0; // compute direction given current postion and setpoint.
    if ( (_positionSetpoint - _currentPosition) == 0) {
        return;
    }
    if ( _timeSinceLastStep >= _delayTime ) {
        step();
    }
    _timeSinceLastStep += dt;
}

/**
 * Updates the motor when it's in motion profile mode.
 * @param the change in time form last loop.
 */
void StepperController::updateProfileMode( unsigned long dt ) {
    
}

/**
 * Updates the motor's state and behavior. This should run every Arduino loop.
 * @param the change in time since last loop, in microseconds.
 */
void StepperController::update( unsigned long dt) {
    if ( !_motorEnabled ) { // if the motor is disabled, don't run any code
        return;
    }
    switch(_mode) {
        case jog: // jogging is just relative positioning.
            updatePositionMode( dt );
        break;

        case position:
            updatePositionMode( dt );
        break;

        case speed:
            updateSpeedMode( dt );
        break;

        case profile:
            updateProfileMode( dt );
        break;
    }
}


/**
 * Destructs the motor controller.
 */
StepperController::~StepperController () {

}