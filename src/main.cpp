// libs
#include <Arduino.h>
#include <Wire.h>

// include
#include "Robot.hpp"

Robot robot;

void setup() {
    // initialize serial communication
    Serial.begin(9600);
    while (!Serial) {}

    // initialize Wire (I2C)
    Wire.begin();

    // initialize robot
    robot.init();
}

void loop() {
    robot.onUpdate();
    robot.debug();
}