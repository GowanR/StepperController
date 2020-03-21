/*
  StepperController.h - Library for controlling multiple A4988 chips concurrently with dynamic motion profiles.
*/

#ifndef __STEPPERCONTROLLER_H_
#define __STEPPERCONTROLLER_H_

#include <avr/io.h>
#include <digitalWriteFast.h>
#include <math.h>

// Driver Carrier Boards

// A4988 - https://www.pololu.com/product/1182
#define A4988_1  0b000
#define A4988_2  0b100
#define A4988_4  0b010
#define A4988_8  0b110
#define A4988_16 0b111

// DRV8825 - https://www.pololu.com/product/2133
#define DRV8825_1  0b000
#define DRV8825_2  0b100
#define DRV8825_4  0b010
#define DRV8825_8  0b110
#define DRV8825_16 0b001
#define DRV8825_32 0b111

// DRV8834 - https://www.pololu.com/product/2134
// TODO: extra attention to floating pins.
#define DRV8834_1  0b00
#define DRV8834_2  0b10
#define DRV8834_4  0b00 // floating, low
#define DRV8834_8  0b01
#define DRV8834_16 0b11
#define DRV8834_32 0b01 // floating, high

// DRV8880 - https://www.pololu.com/product/2971
#define DRV8880_1  0b00
#define DRV8880_NONCIRCULAR_2 0b10
#define DRV8880_2  0b01
#define DRV8880_4  0b11
#define DRV8880_8  0b00 // floating, low
#define DRV8880_16 0b01 // floating, high

// MP6500 (POT CC) - https://www.pololu.com/product/2966
#define MP6500_POT_CC_1 0b00
#define MP6500_POT_CC_2 0b10
#define MP6500_POT_CC_4 0b01
#define MP6500_POT_CC_8 0b11

// MP6500 (DIGITAL CC) - https://www.pololu.com/product/2968
#define MP6500_DIGITAL_CC_1 0b00
#define MP6500_DIGITAL_CC_2 0b10
#define MP6500_DIGITAL_CC_4 0b01
#define MP6500_DIGITAL_CC_8 0b11

// TB67S279FTG - https://www.pololu.com/product/3098
#define TB67S279FTG_STANDBY 0b000 // Outputs disabled.
#define TB67S279FTG_1 0b001
#define TB67S279FTG_NC_2A 0b010 // noncircular half step ("a")
#define TB67S279FTG_4 0b011
#define TB67S279FTG_NC_2B 0b100 // noncircular half step ("b")
#define TB67S279FTG_8 0b101
#define TB67S279FTG_16 0b110
#define TB67S279FTG_32 0b111

// TB67S249FTG - https://www.pololu.com/product/3096
#define TB67S249FTG_STANDBY 0b000 // Outputs disabled.
#define TB67S249FTG_1 0b001
#define TB67S249FTG_NC_2A 0b010 // noncircular half step ("a")
#define TB67S249FTG_4 0b011
#define TB67S249FTG_NC_2B 0b100 // noncircular half step ("b")
#define TB67S249FTG_8 0b101
#define TB67S249FTG_16 0b110
#define TB67S249FTG_32 0b111

// STSPIN820 - https://www.pololu.com/product/2878
#define STSPIN820_1   0b000
#define STSPIN820_2   0b100
#define STSPIN820_4   0b010
#define STSPIN820_8   0b110
#define STSPIN820_16  0b001
#define STSPIN820_32  0b101
#define STSPIN820_128 0b011
#define STSPIN820_256 0b111

// STSPIN220 - https://www.pololu.com/product/2876
#define STSPIN220_1    0b0000
#define STSPIN220_2    0b1010
#define STSPIN220_4    0b0101
#define STSPIN220_8    0b1110
#define STSPIN220_16   0b1111
#define STSPIN220_32   0b0100
#define STSPIN220_64   0b1101
#define STSPIN220_128  0b1000 
#define STSPIN220_256  0b1100


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
        StepperController(unsigned int motorID);
        void step();                            // causes motor to take a step (alternate coil power)
        void update( unsigned long dt );        // updates the motor
        void tare();                            // tares position of stepper to zero steps
        void enable();                          // enables the motor to run
        void disable();                         // disables the motor from running
        void setJogMode();                      // sets the motor to jog mode
        void setSpeedMode();                    // sets the motor to speed mode
        void setPositionMode();                 // sets the motor to position mode
        void setProfileMode();                  // sets the motor to profile mode
        void setSpeed( double speed );           // sets the speed of the motor in RPM. Applies to jog, speed, and position modes
        void setPosition( double setpoint );     // sets the target position of the motor. Will approach the target at the set speed
        void setJog( double rotations );         // will jog the motor `rotations` number of rotations at set speed
        void setRange( double min, double max );  // sets soft stops (in rotations) for stepper motor
        // void setProfile( ProfileNode *head);    // sets the current profile to the given node
        double getSpeed();                       // returns the speed of the motor
        double getPosition();                    // gets the current position, in rotations, that the motor is at
        void setSlave(StepperController &motor);// sets the slave motor
        void clearSlave();                      // removes the slave/breaks slave motor free
        void invert();                          // inverts the direction of the motor
        void reverse();                         // reverses the motor but keeps it reversed
        void setStepsPerRevolution( int steps );// sets the number of steps per revolution. Default is 200.
        int getMode();                         // returns the motor's current mode
        void setSleepOnDisable(bool sleep);    // This will make the motor sleep pin pull when the
        void setStepControlMode(StepControlMode mode); // sets the microstepping control mode for the given stepper motor.
        double getPositionSetpoint();           // returns the position that the motor is homing to if it's in potision mode.
        double getPositionSetpointRevolutions();
        bool isInPosition();                   // checks if motor is in position
        void configureDriverCarrier(DriverCarrierBoard boardType);
        // double getTrajectory();
        ~StepperController();
    
    private:
        unsigned int _motorID;
        bool _isInverted;                       // switches the direction of the motor for all opetations.
        bool _hasSlave;                         // true if the motor has a follower "slave" motor.
        StepperController *_slave;              // motor that will follow this motor.
        StepControlMode _stepControlMode; // mode of microstepping
        DriverCarrierBoard _carrierBoardType;
        long long _currentPosition;
        long long _lowerSoftStop;
        long long _upperSoftStop;
        long double _currentPositionRevolutions; // tracks the current position of the motor in revolutions
        bool _direction;
        bool _inPosition;
        bool _sleepOnDisable; // this will make the motor sleep when the motor is disabled which will reduce heat and increase efficiency.
        double _currentSpeed;
        long long _positionSetpoint;
        double _revPositionSetpoint;
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
        unsigned long secondsToMicros( double seconds );    // converts seconds to microseconds
        void updateSpeedMode( unsigned long dt );
        void updatePositionMode( unsigned long dt );
        void updateProfileMode( unsigned long dt );
        unsigned long rpmToMicros( double RPM );             // converts RPM into microsecond delays between stepping
        long long rotationsToSteps( double rotations );            // converts rotations to steps
        bool motorInRange();                                // checks if motor will break out of soft stops directionaly
        bool getDirection();                                // returns the current direciton of the motor
        void updateMicroStepConfiguration();                // helper function for confiuguring the digital pins for micro stepping
        
        void handleMicrostepA4988();
        void handleMicrostepDRV8825();
};
#endif