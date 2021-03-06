#include "Robot.hpp"




void Robot::init() {
    // initialize motor pins as output.
    pinMode(MOTOR_A_SPEED, OUTPUT);
    pinMode(MOTOR_A_FORWARD, OUTPUT);
    pinMode(MOTOR_A_BACKWARDS, OUTPUT);
    pinMode(MOTOR_B_SPEED, OUTPUT);
    pinMode(MOTOR_B_FORWARD, OUTPUT);
    pinMode(MOTOR_B_BACKWARDS, OUTPUT);

    // Init LED
    pinMode(LED_BUILTIN, OUTPUT);

    // initialize gray scale
    pinMode(GC_SENSOR_FRONT, INPUT);
    pinMode(GC_SENSOR_BACK, INPUT);

    // initialize laser range
    pinMode(LR_SENSOR_FRONT, INPUT);
    pinMode(LR_SENSOR_LEFT, INPUT);

    // initialize reset button
    pinMode(RESET_BUTTON, INPUT);

    if (thermometer.begin()) {
        Serial.println("Error initializing thermometer");
    }
    if (SERVO_RETREAT_POSITION < SERVO_DEFAULT_POSITION) {
        servo.attach(53, SERVO_DEFAULT_POSITION, SERVO_RETREAT_POSITION);
    } else {
        servo.attach(53, SERVO_RETREAT_POSITION, SERVO_DEFAULT_POSITION);
    }

    lastKitDrop = 0;
    servo.write(SERVO_DEFAULT_POSITION);
    delay(1000);

    // set initial state
    Robot::reset();
}

void Robot::motor(int l, int r) {
    analogWrite(MOTOR_A_SPEED, min(abs(l), MOTOR_MAX_SPEED));
    digitalWrite(MOTOR_A_FORWARD, (l < 0) ? LOW : HIGH);
    digitalWrite(MOTOR_A_BACKWARDS, (l > 0) ? LOW : HIGH);

    analogWrite(MOTOR_B_SPEED, min(abs(r), MOTOR_MAX_SPEED));
    digitalWrite(MOTOR_B_FORWARD, (r < 0) ? LOW : HIGH);
    digitalWrite(MOTOR_B_BACKWARDS, (r > 0) ? LOW : HIGH);
}

void Robot::turnTo(int angle) {

    double diff;
    double currentOrientation;

    do {
        compass.read();
        currentOrientation = compass.getAngle() - startOrientation;
        diff = (angle % 360) - currentOrientation;

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
        } else if (abs(diff) > 1) {
            state = 4;
        } else {
            state = -1;
            break;
        }

        switch (state) {
            case 1:
                if (diff > 0) {
                    motor(255, -255);
                } else {
                    motor(-255, 255);
                }
                break;
            case 2:
                if (diff > 0) {
                    motor(200, -200);
                } else {
                    motor(-200, 200);
                }
                break;
            case 3:
                if (diff > 0) {
                    motor(70, -70);
                } else {
                    motor(-70, 70);
                }
                break;
            case 4:
                if (diff > 0) {
                    motor(50, -50);
                } else {
                    motor(-50, 50);
                }
                break;
            default:
                motor(0, 0);
                break;
        }

        delay(50);

    } while (abs(diff) > 0.5 && digitalRead(RESET_BUTTON) == LOW);
    motor(0, 0);
}

void Robot::dropKit() {
    if (millis() - lastKitDrop > KIT_DROP_COOLDOWN_TIME) {
        lastKitDrop = millis();
        turnTo(targetedOrientation + 90);
        servo.write(SERVO_RETREAT_POSITION);
        delay(2000);
        servo.write(SERVO_DEFAULT_POSITION);
        for (int i = 0; i < 15; ++i) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(100);
            digitalWrite(LED_BUILTIN, LOW);
            delay(100);
        }
    }
}

void Robot::checkForNewTiles() {
    if (abs(lastAngle - compass.getAngle()) > 20) {
        lastAngle = compass.getAngle();
        return;
    }

    if (abs(lastLRSensorReadingLeft - getDist(LR_SENSOR_LEFT)) > 40) {
        if (!curTile->getTile(targetedOrientation)) {
            curTile->addNewTile(targetedOrientation);
            curTile = curTile->getTile(targetedOrientation);
            Serial.println("New tile added to the front");
        }

        int dir = (targetedOrientation - 90) % 360;
        if (!curTile->getTile(dir)) {
            curTile->addNewTile(dir);
            Serial.println("New tile added to the left");
        }

    }
    if (abs(lastLRSensorReadingRight - getDist(LR_SENSOR_RIGHT)) > 40 && false) {
        if (!curTile->getTile(targetedOrientation))
            curTile->addNewTile(targetedOrientation);

        int dir = (targetedOrientation + 90) % 360;
        if (!curTile->getTile(dir))
            curTile->addNewTile(dir);
    }

    lastAngle = compass.getAngle();
    lastLRSensorReadingLeft = getDist(LR_SENSOR_LEFT);
    lastLRSensorReadingRight = getDist(LR_SENSOR_RIGHT);
}

void Robot::onUpdate() {

    if (digitalRead(RESET_BUTTON)) {
        motor(0, 0);
        Serial.println("Waiting for reset button to be released");
        while (digitalRead(RESET_BUTTON));
        Serial.println("Wait for reset button to be pressed");
        while (!digitalRead(RESET_BUTTON));
        while (digitalRead(RESET_BUTTON));
        Serial.println("Resetting");
        Robot::reset();
        return;
    }

    digitalWrite(LED_BUILTIN, LOW);
    turnTo(targetedOrientation);
    scanForHeatedObject();
    avoidCollisions();
    scanForOpenSpace();

    // If there's a wall in front
    if (analogRead(GC_SENSOR_FRONT) < 200) {
        motor(-100, -100);
        delay(300);
        targetedOrientation = (targetedOrientation + (random() % 2 == 0 ? 90 : -90)) % 360;
    } else if (compass.getPitch() > 20) {
        motor(180, 180);
    } else {
        motor(100, 100);
    }

    turnState = 0;
}

void Robot::reset() {
    targetedOrientation = 0;
    compass.read();
    startOrientation = compass.getAngle();
    lastAngle = compass.getAngle();
    lastLRSensorReadingLeft = getDist(LR_SENSOR_LEFT);
    lastLRSensorReadingRight = getDist(LR_SENSOR_RIGHT);
    delay(1000);
    analogWrite(LED_BUILTIN, HIGH);
    delay(1000);
    analogWrite(LED_BUILTIN, LOW);
}

void Robot::debug() {
    compass.read();
    Serial.print("Servo: ");
    Serial.print(servo.read());
    Serial.print("\tLRS_FRONT: ");
    Serial.print(getDist(LR_SENSOR_FRONT));
    Serial.print("\tLRS_LEFT: ");
    Serial.print(getDist(LR_SENSOR_LEFT));
    Serial.print("\tcompass: ");
    Serial.print(compass.getPitch());
    Serial.print("\tGC_Front: ");
    Serial.print(analogRead(GC_SENSOR_FRONT));
    Serial.print("\tthermometer: ");
    Serial.print(thermometer.getObjectTempCelsius() - thermometer.getAmbientTempCelsius());
    Serial.print("\tTime: ");
    Serial.println(millis());
}

void Robot::scanForHeatedObject() {
    if (thermometer.getObjectTempCelsius() - thermometer.getAmbientTempCelsius() >
        MINIMAL_TEMPERATURE_DIFFERENCE_FOR_HEATED_OBJECT) {
        dropKit();
    }
}

void Robot::scanForOpenSpace() {
    if (getDist(LR_SENSOR_LEFT) < MIN_SHORT_DISTANCE_TO_RECOGNIZE_OPEN_SPACE) {
        lastShortDistanceReading = millis();
    } else {
        if (millis() - lastShortDistanceReading > MIN_DURATION_AFTER_SHORT_DISTANCE_TO_TURN_INTO_OPEN_SPACE) {
            if ((random() % 100) < PROBABILITY_OF_TURNING_INTO_OPEN_SPACE) {
                targetedOrientation = targetedOrientation + 90;
            }
            lastShortDistanceReading = millis();
        }
    }
}

void Robot::avoidCollisions() {
    if (getDist(LR_SENSOR_FRONT) < 9) {
        switch (turnState) {
            case 0:
                if (getDist(LR_SENSOR_FRONT) < 10) {
                    turnState = 1;
                    targetedOrientation = targetedOrientation + 90;
                }

                if (getDist(LR_SENSOR_LEFT) >= 10) {
                    turnState *= 2;
                }
                break;
            case 1:
                if (getDist(LR_SENSOR_FRONT) < 10) {
                    targetedOrientation = targetedOrientation + 90;
                }
                turnState = 0;
                break;
            case 2:
                if (getDist(LR_SENSOR_FRONT) < 10) {
                    targetedOrientation = targetedOrientation + 180;
                } else {
                    targetedOrientation = targetedOrientation + ((random() % 2) ? 0 : 180);
                }
                turnState = 0;
                break;
        }
    }
}