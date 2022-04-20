// libs
#include <Arduino.h>
#include <Wire.h>

// include
#include "BoardInfo.hpp"
#include "Compass.hpp"

Compass compass = Compass();

double getDist(int address) {
    const double k = -0.1090;
    const double b = 54.71;
    return k * analogRead(address) + b;
}

void motor(int l, int r) {
    analogWrite(MOTOR_A_SPEED, min(abs(l), MOTOR_MAX_SPEED));
    digitalWrite(MOTOR_A_FORWARD, (l < 0) ? LOW : HIGH);
    digitalWrite(MOTOR_A_BACKWARDS, (l > 0) ? LOW : HIGH);

    analogWrite(MOTOR_B_SPEED, min(abs(r), MOTOR_MAX_SPEED));
    digitalWrite(MOTOR_B_FORWARD, (r < 0) ? LOW : HIGH);
    digitalWrite(MOTOR_B_BACKWARDS, (r > 0) ? LOW : HIGH);
}

void setup()
{
    Wire.begin();

    // initialize motor pins as output.
    pinMode(MOTOR_A_SPEED, OUTPUT);
    pinMode(MOTOR_A_FORWARD, OUTPUT);
    pinMode(MOTOR_A_BACKWARDS, OUTPUT);
    pinMode(MOTOR_B_SPEED, OUTPUT);
    pinMode(MOTOR_B_FORWARD, OUTPUT);
    pinMode(MOTOR_B_BACKWARDS, OUTPUT);

    // Init LED
    pinMode(ON_BOARD_LED, OUTPUT);

    // initialize gray scale
    pinMode(GC_SENSOR_FRONT, INPUT);
    pinMode(GC_SENSOR_BACK, INPUT);

    // initialize laser range
    pinMode(LR_SENSOR_SIDE_FRONT_RIGHT, INPUT);
    pinMode(LR_SENSOR_SIDE_BACK_RIGHT, INPUT);
    pinMode(LR_SENSOR_SIDE_FRONT_LEFT, INPUT);
    pinMode(LR_SENSOR_SIDE_BACK_LEFT, INPUT);

    // initialize serial
    Serial.begin(115200);

}

void alignToWall(bool leftWall, double maxDif) {
    double front;
    double back;
    double motorSpeed;

    do {
        front = (leftWall) ? getDist(LR_SENSOR_SIDE_FRONT_LEFT) : getDist(LR_SENSOR_SIDE_FRONT_RIGHT);
        back = (leftWall) ? getDist(LR_SENSOR_SIDE_BACK_LEFT) : getDist(LR_SENSOR_SIDE_BACK_RIGHT);
        motorSpeed = 5* (front - back);

        motor(motorSpeed, -motorSpeed);
    }
    while (abs(front - back) > maxDif);
}

void loop() {

    alignToWall(false, 2);
    delay(10);
}