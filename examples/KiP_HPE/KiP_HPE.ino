#include <KiP.h>
#include <Arduino.h>
// "sketch": "examples\\KiP_HPE\\KiP_HPE.ino",
void setup() {
    configMotors();
}

void loop() {
    forward(5);
    turnLeft();
    forward(5);
    turnLeft();
    reverse(5);
}