// libs
#include <Arduino.h>
#include <Wire.h>

// include
#include "Robot.hpp"

Robot robot = Robot();

void setup()
{
    // initialize serial communication
    Serial.begin(9600);

    // initialize Wire (I2C)
    Wire.begin();

    // initialize robot
    robot.reset();
}

void loop() {
    robot.onUpdate();
    robot.debug();
}