/*
  StepperController.h - Library for controlling multiple A4988 chips concurrently with dynamic motion profiles.
*/

#include "Arduino.h"


class StepperController {
    public:
        StepperController( unsigned short stepPin, unsigned short directionPin );
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
        float getPosition();                    // gets the current position, in rotations, that the motor is at
        ~StepperController();
    
    private:
        unsigned short _stepPin;
        unsigned short _directionPin;
        long _currentPosition;
        long _lowerSoftStop;
        long _upperSoftStop;
        bool _direction;
        float _currentSpeed;
        float _positionSetpoint;
        bool _stepActive;
        unsigned long _timeSinceLastStep;   // in microseconds
        bool _motorEnabled;                 // if false, motor will not do anything
        enum Mode {
            speed,     // motor will spin at specific RPM
            jog,       // motor will jog n rotations at x RPM
            position,  // motor will snap to given positon in rotations
            profile    // motor will follow the acceleration, positon, and velocity profile
        };
        Mode _mode;
        void updateSpeedMode( unsigned long dt );
        void updatePositionMode( unsigned long dt );
        void updateProfileMode( unsigned long dt );
        unsigned long rpmToMicros( float RPM );             // converts RPM into microsecond delays between stepping
        int rotationsToSteps( float rotations );            // converts rotations to steps
        bool motorInRange();                                // checks if motor will break out of soft stops directionaly
        
};
