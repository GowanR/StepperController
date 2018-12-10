/*
  StepperController.cpp - Library for controlling multiple A4988 chips concurrently with dynamic motion profiles.
*/

#include "StepperController.h"

/**
 * Constructs the motor controller.
 * @param the Arduino pin used for step control.
 * @param teh Arduino pin used for direction control.
 */
StepperController::StepperController ( unsigned short stepPin, unsigned short directionPin ) {
    _directionPin = directionPin;
    _stepPin = stepPin;
    _mode = jog;                          // default mode is jog
    _currentPosition = 0;                 // starting position of motor is 0 steps, this stacks half steps
    _motorEnabled = true;                 // true when the motor can move
    _stepActive = false;                  // keeps track of if the coil powered or not
    _upperSoftStop = 0;                   // set upper soft stop to 0
    _lowerSoftStop = 0;                   // set lower soft stop to 0
    _positionSetpoint = 0;                // the default position setpoint is zero
    _isInverted = false;                  // the motor is not inverted by default
    _stepsPerRevolution = 200;
    pinMode( _stepPin, OUTPUT );
    pinMode( _directionPin, OUTPUT );
}

/**
 * Inverts the motor direciton. Toggles the default direction for sign.
 */
void StepperController::invert() {
    _isInverted = !_isInverted;
}

/**
 * Sets how many steps the motor must make to turn one revoltion.
 * @param number of steps per revoltion
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
 */
void StepperController::setSlave(StepperController &motor) {
    _hasSlave = true;
    _slave = &motor;
    _slave->disable(); // prevent motor form interpreting any other commands.
}

/**
 * Clears the salve and sets it free.
 */
void StepperController::clearSlave() {
    _hasSlave = false;
    _slave->enable(); // allow the motor to be controlled by the outside world again.
}

/**
 * Stepper motor will change alternate the current of each coil resulting in "half" of a step.
 * If this is called twice, the stepper motor will have turned a step.
 */ 
void StepperController::step() {
    digitalWrite( _directionPin, getDirection() ? HIGH : LOW );
    digitalWrite( _stepPin, _stepActive ? LOW : HIGH );
    _stepActive = !_stepActive;
    _timeSinceLastStep = 0;
    _direction ? _currentPosition++ : _currentPosition--;

    if ( _hasSlave ) {
        _slave->step();
    }
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
int StepperController::rotationsToSteps( float rotations ) {
    return (int) (rotations * _stepsPerRevolution);
}

/**
 * Function used to get the current position of the motor in revolutions.
 * @returns the current position of the motor in revoltions
 */
float StepperController::getPosition() {
    return _currentPosition / (2.f * _stepsPerRevolution);
}

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
 * @param the lower soft stop value in revoltions.
 * @param the upper soft stop value in revoltions.
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
    _positionSetpoint = rotationsToSteps(setpoint) * 2;
}

/**
 * Sets the current position of the motor to the soft zero (0) positon.
 */
void StepperController::tare() {
    _currentPosition = 0;
}

/** 
 * Allows the motor to move when updated.
 */
void StepperController::enable() {
    _motorEnabled = true;
}

/**
 * Prevents the motor from moving when updated.
 */
void StepperController::disable() {
    _motorEnabled = false;
}

/**
 * Sets the motor mode to jog.
 * Jog mode is based on sending the motor a command to move x revoltions in a given direciton.
 */
void StepperController::setJogMode() {
    _mode = jog;
}

/**
 * Sets the motor mode to speed.
 * Speed mode tells teh motor to spin in a direction at a specified RPM
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
    _positionSetpoint = _currentPosition + ( rotationsToSteps( rotations ) * 2 );
}

/**
 * Checks to see if the motor is withing the set soft stop range with consideration of direciton.
 * @returns if the motor is in allowable range and will not break range in direction.
 */
bool StepperController::motorInRange(){
    if ( _upperSoftStop != 0 && _lowerSoftStop != 0 ) {
        if ( _currentPosition / 2 >= _upperSoftStop && _direction ) {
            return false;
        }
        if ( _currentPosition / 2 <= _lowerSoftStop && !_direction ) {
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
    unsigned long _delayTime = rpmToMicros(_currentSpeed) / 2;
    if ( _timeSinceLastStep >= _delayTime ) {
        step();
    }
    _timeSinceLastStep += dt;
}

/**
 * Updates the motor when it's in position mode.
 * @param the change in time form last loop.
 */
void StepperController::updatePositionMode( unsigned long dt ) {
    if ( !motorInRange() ) {
        return;
    }
    unsigned long _delayTime = rpmToMicros(_currentSpeed) / 2;
    if ( _currentPosition == _positionSetpoint ) {
        return;
    }
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