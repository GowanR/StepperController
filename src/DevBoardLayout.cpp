#include "DevBoardLayout.h"

DevBoardLayout::DevBoardLayout() {
    // The following microstepping pin configurations are for the KiP PCB Rev. 3.3.
    motor0.configureMicroSteppingPins(10, 11, 12);
    motor1.configureMicroSteppingPins(13, 53, 16);
    motor2.configureMicroSteppingPins(17, 6, 7);
}
