#include "Robot.hpp"

Robot::Robot(double orientation) : startOrientation(orientation) {
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

    thermometer.begin();
    servo.attach(53);
}

void Robot::motor(int l, int r) {
    analogWrite(MOTOR_A_SPEED, min(abs(l), MOTOR_MAX_SPEED));
    digitalWrite(MOTOR_A_FORWARD, (l < 0) ? LOW : HIGH);
    digitalWrite(MOTOR_A_BACKWARDS, (l > 0) ? LOW : HIGH);

    analogWrite(MOTOR_B_SPEED, min(abs(r), MOTOR_MAX_SPEED));
    digitalWrite(MOTOR_B_FORWARD, (r < 0) ? LOW : HIGH);
    digitalWrite(MOTOR_B_BACKWARDS, (r > 0) ? LOW : HIGH);
}

void Robot::alignLeft(double maxDif) {
    double front;
    double back;
    double motorSpeed;

    do {
        front = getDist(LR_SENSOR_SIDE_FRONT_LEFT);
        back = getDist(LR_SENSOR_SIDE_BACK_LEFT);
        motorSpeed = 5 * lround(pow(front - back, 3));

        motor(motorSpeed, -motorSpeed);
    } while (abs(front - back) > maxDif);
}

void Robot::alignRight(double maxDif) {
    double front;
    double back;
    double motorSpeed;

    do {
        front = getDist(LR_SENSOR_SIDE_FRONT_RIGHT);
        back = getDist(LR_SENSOR_SIDE_BACK_RIGHT);
        motorSpeed = 5 * lround(pow(front - back, 3));

        motor(-motorSpeed, motorSpeed);
    } while (abs(front - back) > maxDif);
}

void Robot::dropKit() {
    servo.write(45);
    delay(70);  // wait for servo to move
    servo.write(90);
}

void Robot::onUpdate() {
    // update the onboard led
    digitalWrite(ON_BOARD_LED, HIGH);

    compass.read();


    turnTo( 90);
    motor(100, 100);
    for (int i = 0; i < 40; ++i) {
        if (thermometer.getObjectTempCelsius() - thermometer.getAmbientTempCelsius() > 5) {
            dropKit();
        }
        delay(10);
    }

//    Serial.print("LRS: ");
//    Serial.print(getDist(LR_SENSOR_SIDE_FRONT_RIGHT));
//    Serial.print("\t");
//    Serial.print(getDist(LR_SENSOR_SIDE_BACK_RIGHT));
//    Serial.print("\t");
//    Serial.print(getDist(LR_SENSOR_SIDE_FRONT_LEFT));
//    Serial.print("\t");
//    Serial.print(getDist(LR_SENSOR_SIDE_BACK_LEFT));
//    Serial.print("\t");
    Serial.print(compass.getAngle());
    Serial.print("\t");
    Serial.println(thermometer.getObjectTempCelsius() - thermometer.getAmbientTempCelsius());



    //alignLeft(0.1);
    //motor(50, -50);
}

void Robot::turnTo(double angle) {

    double diff;
    double currentOrientation;

    do {
        compass.read();
        currentOrientation = startOrientation - compass.getAngle();
        diff = angle - currentOrientation;

        if (diff > 180) {
            diff -= 360;
        } else if (diff < -180) {
            diff += 360;
        }

        int state = 0;
        if (abs(diff) > 100) {
            state = 1;
        } else if (abs(diff) > 50) {
            state = 2;
        } else if (abs(diff) > 15) {
            state = 3;
        } else {
            state = 4;
        }

        switch (state) {
            case 1:
                if (diff < 0) {
                    motor(255, -255);
                } else {
                    motor(-255, 255);
                }
                break;
            case 2:
                if (diff < 0) {
                    motor(200, -200);
                } else {
                    motor(-200, 200);
                }
                break;
            case 3:
                if (diff < 0) {
                    motor(70, -70);
                } else {
                    motor(-70, 70);
                }
                break;
            case 4:
                if (diff < 0) {
                    motor(50, -50);
                } else {
                    motor(-50, 50);
                }
                break;
        }

        delay(50);

    } while (abs(diff) > 1);
}
