#include "Robot.hpp"

Robot::Robot() {
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
    pinMode(LR_SENSOR_SIDE_FRONT_RIGHT, INPUT);
    pinMode(LR_SENSOR_SIDE_BACK_RIGHT, INPUT);
    pinMode(LR_SENSOR_SIDE_FRONT_LEFT, INPUT);
    pinMode(LR_SENSOR_SIDE_BACK_LEFT, INPUT);

    // initialize reset button
    pinMode(RESET_BUTTON, INPUT);

    thermometer.begin();
    servo.attach(53);

    // set initial state
    reset();
}

void Robot::motor(int l, int r) {
    analogWrite(MOTOR_A_SPEED, min(abs(l), MOTOR_MAX_SPEED));
    digitalWrite(MOTOR_A_FORWARD, (l < 0) ? LOW : HIGH);
    digitalWrite(MOTOR_A_BACKWARDS, (l > 0) ? LOW : HIGH);

    analogWrite(MOTOR_B_SPEED, min(abs(r), MOTOR_MAX_SPEED));
    digitalWrite(MOTOR_B_FORWARD, (r < 0) ? LOW : HIGH);
    digitalWrite(MOTOR_B_BACKWARDS, (r > 0) ? LOW : HIGH);
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

        int state;
        if (abs(diff) > 100) {
            state = 1;
        } else if (abs(diff) > 50) {
            state = 2;
        } else if (abs(diff) > 15) {
            state = 3;
        } else if (abs(diff) > 1){
            state = 4;
        } else {
            break;
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

    } while (abs(diff) > 1 && digitalRead(RESET_BUTTON) == LOW);
}

void Robot::dropKit() {
    motor(0, 0);
    servo.write(SERVO_OUT_POSITION);
    delay(1000);
    servo.write(SERVO_DEFAULT_POSITION);
    for (int i = 0; i < 20; ++i) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
    }
}

void Robot::onUpdate() {
    // update the onboard led

    if (digitalRead(RESET_BUTTON)) {
        motor(0, 0);
        Serial.println("Waiting for reset button to be released");
        while (digitalRead(RESET_BUTTON));
        Serial.println("Wait for reset button to be pressed");
        while (!digitalRead(RESET_BUTTON));
        reset();
        return;
    }

    digitalWrite(LED_BUILTIN, LOW);
    turnTo(curDir);
    compass.read();
    if (compass.getPitch() > 10) {
        motor(255, 255);
    } else {
        motor(100, 100);
    }

    if (getDist(LR_SENSOR_FRONT) < 8) {
        curDir = ((int) curDir + (analogRead(GC_SENSOR_FRONT) % 2 == 0 ? 90 : -90)) % 360;
    } else if (analogRead(GC_SENSOR_FRONT) < 200) {
        motor(-100, -100);
        delay(500);
        curDir = ((int) curDir + (random() % 2 == 0 ? 90 : -90)) % 360;
    } else if (thermometer.getObjectTempCelsius() - thermometer.getAmbientTempCelsius() > 5) {
        turnTo(((int) curDir - 90) % 360);
        dropKit();
        turnTo(curDir);
        motor(100, 100);
        delay(600);
    }

}

void Robot::reset() {
    curDir = 0;
    compass.read();
    startOrientation = compass.getAngle();
    analogWrite(LED_BUILTIN, HIGH);
    delay(1000);
    analogWrite(LED_BUILTIN, LOW);
}

void Robot::debug() {
    compass.read();
    Serial.print("Servo: ");
    Serial.print(servo.read());
    Serial.print("\tLRS: ");
    Serial.print(getDist(LR_SENSOR_FRONT));
    Serial.print("\tcompass: ");
    Serial.print(compass.getPitch());
    Serial.print("\tGC_Front:");
    Serial.print(analogRead(GC_SENSOR_FRONT));
    Serial.print("\tthermometer:");
    Serial.println(thermometer.getObjectTempCelsius() - thermometer.getAmbientTempCelsius());
}
