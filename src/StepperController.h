/*
  StepperController.h - Library for controlling multiple A4988 chips concurrently with dynamic motion profiles.
*/

#ifndef __STEPPERCONTROLLER_H_
#define __STEPPERCONTROLLER_H_

#include "Arduino.h"




enum StepControlMode { // definition of possible control modes. default is full step.
    FULL_STEP = 1,
    HALF_STEP = 2,
    QUARTER_STEP = 4,
    EIGHTH_STEP = 8,
    SIXTEENTH_STEP = 16,
    STEP_32 = 32,
    STEP_128 = 128,
    STEP_256 = 256
};
enum DriverCarrierBoard {
    A4988,
    DRV8825,
    DRV8834,
    DRV8880,
    MP6500_POT_CC,
    MP6500_DIGITAL_CC,
    TB67S279FTG,
    TB67S249FTG,
    STSPIN820,
    STSPIN220
};
class StepperController {
    public:
        StepperController( unsigned short stepPin, unsigned short directionPin, unsigned short sleepPin );
        void configureMicroSteppingPins(
            unsigned short MS1, 
            unsigned short MS2, 
            unsigned short MS3
        );                                      // sets the pins used to configure microstepping.
        void step();                            // causes motor to take a step (alternate coil power)
        void update( unsigned long dt );        // updates the motor
        void tare();                            // tares position of stepper to zero steps
        void enable();                          // enables the motor to run
        void disable();                         // disables the motor from running
        void setJogMode();                      // sets the motor to jog mode
        void setSpeedMode();                    // sets the motor to speed mode
        void setPositionMode();                 // sets the motor to position mode
        void setProfileMode();                  // sets the motor to profile mode
        void setSpeed( float speed );           // sets the speed of the motor in RPM. Applies to jog, speed, and position modes
        void setPosition( float setpoint );     // sets the target position of the motor. Will approach the target at the set speed
        void setJog( float rotations );         // will jog the motor `rotations` number of rotations at set speed
        void setRange( float min, float max );  // sets soft stops (in rotations) for stepper motor
        // void setProfile( ProfileNode *head);    // sets the current profile to the given node
        float getSpeed();                       // returns the speed of the motor
        float getPosition();                    // gets the current position, in rotations, that the motor is at
        void setSlave(StepperController &motor);// sets the slave motor
        void clearSlave();                      // removes the slave/breaks slave motor free
        void invert();                          // inverts the direction of the motor
        void reverse();                         // reverses the motor but keeps it reversed
        void setStepsPerRevolution( int steps );// sets the number of steps per revolution. Default is 200.
        int getMode();                         // returns the motor's current mode
        void setSleepOnDisable(bool sleep);    // This will make the motor sleep pin pull when the
        void setStepControlMode(StepControlMode mode); // sets the microstepping control mode for the given stepper motor.
        float getPositionSetpoint();           // returns the position that the motor is homing to if it's in potision mode.
        float getPositionSetpointRevolutions();
        bool isInPosition();                   // checks if motor is in position
        void configureDriverCarrier(DriverCarrierBoard boardType);
        // float getTrajectory();
        ~StepperController();
    
    private:
        unsigned short _ms1, _ms2, _ms3;        // microstepping pins
        bool _isInverted;                       // switches the direction of the motor for all opetations.
        bool _hasSlave;                         // true if the motor has a follower "slave" motor.
        StepperController *_slave;              // motor that will follow this motor.
        StepControlMode _stepControlMode; // mode of microstepping
        DriverCarrierBoard _carrierBoardType;
        unsigned short _microstepConversion;
        unsigned short _stepPin;
        unsigned short _directionPin;
        unsigned short _sleepPin;
        long long _currentPosition;
        long long _lowerSoftStop;
        long long _upperSoftStop;
        long double _currentPositionRevolutions; // tracks the current position of the motor in revolutions
        bool _direction;
        bool _inPosition;
        bool _sleepOnDisable; // this will make the motor sleep when the motor is disabled which will reduce heat and increase efficiency.
        float _currentSpeed;
        long double _positionSetpoint;
        float _revPositionSetpoint;
        bool _stepActive;
        int _stepsPerRevolution;             // number of steps per revoltion of the stepper motor. Typically 200.
        unsigned long _timeSinceLastStep;   // in microseconds
        bool _motorEnabled;                 // if false, motor will not do anything
        enum Mode {
            speed,     // motor will spin at specific RPM
            jog,       // motor will jog n rotations at x RPM
            position,  // motor will snap to given positon in rotations
            profile    // motor will follow the acceleration, positon, and velocity profile
        };
        Mode _mode;
        unsigned long secondsToMicros( float seconds );    // converts seconds to microseconds
        void updateSpeedMode( unsigned long dt );
        void updatePositionMode( unsigned long dt );
        void updateProfileMode( unsigned long dt );
        unsigned long rpmToMicros( float RPM );             // converts RPM into microsecond delays between stepping
        long long rotationsToSteps( float rotations );            // converts rotations to steps
        bool motorInRange();                                // checks if motor will break out of soft stops directionaly
        bool getDirection();                                // returns the current direciton of the motor
        void updateMicroStepConfiguration();                // helper function for confiuguring the digital pins for micro stepping
};
#endif