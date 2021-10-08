


/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>
#include "BOARD_INFO.hpp"

void motor(int l, int r) {
    analogWrite(MOTOR_A_SPEED, abs(l));
    digitalWrite(MOTOR_A_FORWARD, (l < 0) ? LOW : HIGH);
    digitalWrite(MOTOR_A_BACKWARDS, (l > 0) ? LOW : HIGH);

    analogWrite(MOTOR_B_SPEED, abs(r));
    digitalWrite(MOTOR_B_FORWARD, (r < 0) ? LOW : HIGH);
    digitalWrite(MOTOR_B_BACKWARDS, (r > 0) ? LOW : HIGH);
}


void setup()
{
    // initialize motor pins as output.
    pinMode(MOTOR_A_SPEED, OUTPUT);
    pinMode(MOTOR_A_FORWARD, OUTPUT);
    pinMode(MOTOR_A_BACKWARDS, OUTPUT);
    pinMode(MOTOR_B_SPEED, OUTPUT);
    pinMode(MOTOR_B_FORWARD, OUTPUT);
    pinMode(MOTOR_B_BACKWARDS, OUTPUT);

    // initialize gray scale
    pinMode(GRAY_SCALE_FRONT, INPUT);

    // initialize serial
    Serial.begin(9600);

}

int grayScaleSensor() {
    return analogRead(GRAY_SCALE_FRONT);
}

void loop()
{

    // turn on different to white
    if (grayScaleSensor() < 200) {
        motor(-200, -200);
    } else {
        motor(200, 200);
    }

    Serial.println(grayScaleSensor());

    delay(100);

    // wait for a second#

}
