/*
  StepperController.cpp - Library for controlling multiple A4988 chips concurrently with dynamic motion profiles.
*/

#include "StepperController.h"

#define STEPS_PER_REVOLUTION 200 
/**
 * Constructs the motor controller.
 * @param the Arduino pin used for step control.
 * @param the Arduino pin used for direction control.
 */
StepperController::StepperController ( unsigned short stepPin, unsigned short directionPin, unsigned short sleepPin ) {
    _inPosition = false;
    _directionPin = directionPin;
    _stepPin = stepPin;
    _sleepPin = sleepPin;
    _stepControlMode = FULL_STEP;
    _microstepConversion = 1;             // represents full step
    _mode = jog;                          // default mode is jog
    _currentPosition = 0;                 // starting position of motor is 0 steps, this stacks half steps
    _currentPositionRevolutions = 0;
    _motorEnabled = true;                 // true when the motor can move
    _stepActive = false;                  // keeps track of if the coil powered or not
    _upperSoftStop = 0;                   // set upper soft stop to 0
    _lowerSoftStop = 0;                   // set lower soft stop to 0
    _positionSetpoint = 0;                // the default position setpoint is zero
    _isInverted = false;                  // the motor is not inverted by default
    _stepsPerRevolution = STEPS_PER_REVOLUTION * _microstepConversion;
    _sleepOnDisable = true;
    pinMode( _stepPin, OUTPUT );
    pinMode( _directionPin, OUTPUT );
    pinMode( _sleepPin, OUTPUT );
}



/**
 * This method changes the microstepping mode for the motor.
 * ![WARN]: this source is currently voilitile in that it will reset motor position when microstepping configuration is altered.
 */
void StepperController::setStepControlMode(StepControlMode mode) {    
    _stepControlMode = mode;
    _microstepConversion = mode;
    _stepsPerRevolution = STEPS_PER_REVOLUTION * mode;
    _currentPosition = 0;
    // _currentPosition = (long long)(_currentPositionRevolutions * _stepsPerRevolution);
    updateMicroStepConfiguration();
}

void StepperController::configureMicroSteppingPins( unsigned short MS1, unsigned short MS2, unsigned short MS3 ) {
    _ms1 = MS1;
    _ms2 = MS2;
    _ms3 = MS3;
    pinMode(_ms1, OUTPUT);
    pinMode(_ms2, OUTPUT);
    pinMode(_ms3, OUTPUT);
}


/** 
 *  Microstepping configuration for A4988 driver.
 *  =============================
 * | STEP TYPE | MS1 | MS2 | MS3 |
 * |   FULL    |  L  |  L  |  L  |
 * |   HALF    |  H  |  L  |  L  |
 * |  QUARTER  |  L  |  H  |  L  |
 * |  EIGHTH   |  H  |  H  |  L  |
 * | SIXTEETH  |  H  |  H  |  H  |
 *  =============================
 * 
 */

void StepperController::updateMicroStepConfiguration() {
    switch (_stepControlMode) {
        case FULL_STEP: // 0 0 0
            digitalWrite(_ms1, LOW);
            digitalWrite(_ms2, LOW);
            digitalWrite(_ms3, LOW);
        break;
        case HALF_STEP: // 1 0 0
            digitalWrite(_ms1, HIGH);
            digitalWrite(_ms2, LOW);
            digitalWrite(_ms3, LOW);
        break;
        case QUARTER_STEP: // 0 1 0
            digitalWrite(_ms1, LOW);
            digitalWrite(_ms2, HIGH);
            digitalWrite(_ms3, LOW);
        break;
        case EIGHTH_STEP: // 1 1 0
            digitalWrite(_ms1, HIGH);
            digitalWrite(_ms2, HIGH);
            digitalWrite(_ms3, LOW);
        break;
        case SIXTEENTH_STEP: // 1 1 1
            digitalWrite(_ms1, HIGH);
            digitalWrite(_ms2, HIGH);
            digitalWrite(_ms3, HIGH);
        break;
        default: // default will result in pulling all microstep configuration pins low
            digitalWrite(_ms1, LOW);
            digitalWrite(_ms2, LOW);
            digitalWrite(_ms3, LOW);
        break;
    }
}

/**
 * Converts seconds into microseconds.
 * @param seconds to convert
 */
unsigned long StepperController::secondsToMicros( float seconds ) {
    return (unsigned long) (seconds * 1000000.f);
}

/**
 * Generates a linked list definition of the motionprofile defined in the arrays.
 * Will return a node with all zeros if the arrays aren't of equal size.
 * @param array (of length n) of position values
 * @param array (of length n) of correlating time stamps to the position values 
 */
// static ProfileNode* StepperController::generateProfile( float pos[], float timestamp[] ) {
//     size_t posSize = sizeof( pos ) / sizeof( pos[0] );
//     size_t timeSize = sizeof( timestamp ) / sizeof( timestamp[0] );
//     if ( posSize == timeSize ) {
//         struct ProfileNode badNode {
//             .position = 0,
//             .timestamp = 0
//         };
//         return badNode;
//     } 
//     struct ProfileNode *head;
//     head = malloc( sizeof( struct ProfileNode) );
//     struct ProfileNode *next = &head;
//     struct ProfileNode *now = malloc( sizeof( struct ProfileNode ) );
//     for ( unsigned short i = 0; i < (unsigned short)posSize; i++ ) {
//         next->position = pos[i];
//         next->timestamp = secondsToMicros( timestamp[i] );
//         next->nextNode = now;
        
//         next = now;
//     }
//     return head;
// }

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

float StepperController::getSpeed() {
    return _currentSpeed;
}

/**
 * Inverts the motor direciton. Toggles the default direction for sign.
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

    digitalWrite( _directionPin, getDirection() ? HIGH : LOW ); // if the set direction pin high or low based on stored direction.
    digitalWrite( _stepPin, HIGH );
    _stepActive = true;
    _timeSinceLastStep = 0;
    _direction ? _currentPosition++ : _currentPosition--; // if direction true, conventionally clockwise, then increment the current position. if negative, decrement. 
    
    // tracks position in revolutions
    if (_direction) {
        _currentPositionRevolutions += 1.f/((double)_stepsPerRevolution);
    } else {
        _currentPositionRevolutions -= 1.f/((double)_stepsPerRevolution);
    }
    
    if ( _hasSlave ) {
        _slave->step();
    }
    digitalWrite( _stepPin, LOW ); // lower the step pin
    _stepActive = false;
}

/**
 * Helper function givem rotations per minute will compute the microsecond delay between steps.
 * @param the roations per minute
 * @returns microsecond interval between steps
 */
unsigned long StepperController::rpmToMicros( float RPM ) {
    return (unsigned long)((60.f * 1e6) / (abs(RPM) * ((float)_stepsPerRevolution)));
}

/**
 * Converts rotations into approximate discrete steps.
 * @param number of rotations to convert
 * @return number of steps the motor needs to take to equate to approximate rotations.
 */
long long StepperController::rotationsToSteps( float rotations ) {
    return (rotations * _stepsPerRevolution);
}

/**
 * Function used to get the current position of the motor in revolutions.
 * @returns the current position of the motor in revolutions
 */
float StepperController::getPosition() {
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
void StepperController::setSpeed( float speed ) {
    _direction = speed > 0;
    _currentSpeed = speed;
}

/**
 * Sets the 'soft' stops of the motor. This bound the motor to up to two values in revolutions.
 * 
 * @param the lower soft stop value in revolutions.
 * @param the upper soft stop value in revolutions.
 */
void StepperController::setRange( float min, float max ) {
    _lowerSoftStop = rotationsToSteps(min);
    _upperSoftStop = rotationsToSteps(max);
}

/**
 * This sets the positional setpoint of the motor in revolutions.
 * @param the absolute 'goal' revolutions value.
 */
void StepperController::setPosition( float setpoint ) {
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
    digitalWrite(_sleepPin, HIGH);
}

/**
 * Prevents the motor from moving when updated.
 */
void StepperController::disable() {
    _motorEnabled = false;
    if(_sleepOnDisable) {
        digitalWrite(_sleepPin, LOW);
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
void StepperController::setJog( float rotations ) {
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

float StepperController::getPositionSetpoint() {
    return _positionSetpoint;
}

float StepperController::getPositionSetpointRevolutions(){
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
void StepperController::updatePositionMode( unsigned long dt ) {
    if ( !motorInRange() ) {
        return;
    }
    unsigned long _delayTime = rpmToMicros(_currentSpeed);
    if ( _currentPosition == _positionSetpoint ) {
        _inPosition = true;
        return;
    }
    _inPosition = false;
    _direction = ( _positionSetpoint - _currentPosition ) >= 0; // compute direction given current postion and setpoint.
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