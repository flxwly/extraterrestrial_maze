#include "Robot.hpp"

Robot::Robot(double orientation) : startOrientation(orientation) {

}
/**
 * Sets the motors to the given speed.
 * @param l left motor speed
 * @param r right motor speed
 * @note speed is in range [-MOTOR_MAX_SPEED, MOTOR_MAX_SPEED]
 */
void Robot::motor(int l, int r) {
    analogWrite(MOTOR_A_SPEED, min(abs(l), MOTOR_MAX_SPEED));
    digitalWrite(MOTOR_A_FORWARD, (l < 0) ? LOW : HIGH);
    digitalWrite(MOTOR_A_BACKWARDS, (l > 0) ? LOW : HIGH);

    analogWrite(MOTOR_B_SPEED, min(abs(r), MOTOR_MAX_SPEED));
    digitalWrite(MOTOR_B_FORWARD, (r < 0) ? LOW : HIGH);
    digitalWrite(MOTOR_B_BACKWARDS, (r > 0) ? LOW : HIGH);
}
