// libs
#include <Arduino.h>
#include <Wire.h>

// include
#include "Compass.hpp"
#include "Robot.hpp"

Compass compass;
Robot robot;

void setup()
{
    // initialize serial communication
    Serial.begin(9600);

    // initialize Wire (I2C)
    Wire.begin();

    // initialize compass
    compass = Compass();

    // initialize robot
    robot = Robot(compass.getAngle());
}

int angle = 0;

void loop() {
    robot.onUpdate();

    digitalWrite(13, LOW);
}