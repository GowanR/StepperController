/*
  StepperController.h - Library for controlling multiple A4988 chips concurrently with dynamic motion profiles.
*/

#ifndef __STEPPERCONTROLLER_H_
#define __STEPPERCONTROLLER_H_

#include "Arduino.h"




class StepperController {
    public:
        StepperController( unsigned short stepPin, unsigned short directionPin, unsigned short sleepPin );
        
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
        void setStepsPerRevolution( int steps );// sets the number of steps per revolution. Default is 200.
        int getMode();                         // returns the motor's current mode
        void setSleepOnDisable(bool sleep);    // This will make the motor sleep pin pull when the
        ~StepperController();
    
    private:
        bool _isInverted;                       // switches the direction of the motor for all opetations.
        bool _hasSlave;                         // true if the motor has a follower "slave" motor.
        StepperController *_slave;              // motor that will follow this motor.
        unsigned short _stepPin;
        unsigned short _directionPin;
        unsigned short _sleepPin;
        long _currentPosition;
        long _lowerSoftStop;
        long _upperSoftStop;
        bool _direction;
        bool _sleepOnDisable; // this will make the motor sleep when the motor is disabled which will reduce heat and increase efficiency.
        float _currentSpeed;
        float _positionSetpoint;
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
        int rotationsToSteps( float rotations );            // converts rotations to steps
        bool motorInRange();                                // checks if motor will break out of soft stops directionaly
        bool getDirection();                                // returns the current direciton of the motor
};
#endif