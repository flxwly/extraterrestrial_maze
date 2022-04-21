// libs
#include <Arduino.h>
#include <Wire.h>

// include
#include "BoardInfo.hpp"
#include "Compass.hpp"
#include "Robot.hpp"
#include "LaserRangeSensor.hpp"

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

void alignToWall(bool leftWall, double maxDif) {
    double front;
    double back;
    double motorSpeed;

    do {
        front = (leftWall) ? getDist(LR_SENSOR_SIDE_FRONT_LEFT) : getDist(LR_SENSOR_SIDE_FRONT_RIGHT);
        back = (leftWall) ? getDist(LR_SENSOR_SIDE_BACK_LEFT) : getDist(LR_SENSOR_SIDE_BACK_RIGHT);
        motorSpeed = 5 * lround(pow(front - back, 3));

        motor(motorSpeed, -motorSpeed);
    }
    while (abs(front - back) > maxDif);
}

void loop() {
    alignToWall(false, 2);
//    Serial.println("------- Right -------");
//    Serial.print("Front: ");
//    Serial.println(getDist(LR_SENSOR_SIDE_FRONT_RIGHT));
//    Serial.print("Back: ");
//    Serial.println(getDist(LR_SENSOR_SIDE_BACK_RIGHT));
//    Serial.println("------- Left -------");
//    Serial.print("Front: ");
//    Serial.println(getDist(LR_SENSOR_SIDE_FRONT_LEFT));
//    Serial.print("Back: ");
//    Serial.println(getDist(LR_SENSOR_SIDE_BACK_LEFT));

    delay(200);
}