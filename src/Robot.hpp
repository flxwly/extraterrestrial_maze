#include <Arduino.h>
#include <DFRobot_MLX90614.h>
#include "Compass.hpp"
#include "Map.hpp"
#include "LaserRangeSensor.hpp"
#include "BoardInfo.hpp"
#include "Servo.h"

#define SERVO_DEFAULT_POSITION 150
#define SERVO_OUT_POSITION 100

#define MIN_DIST_FOR_WALL 15


class Robot {

private:

    DFRobot_MLX90614_IIC thermometer;
    Servo servo;
    Compass compass;

    double startOrientation = 0;
    double curDir = 0;

    Tile startingTile{};
    double x = 0, y = 0;

public:
    Robot();

    /**
     * Sets the motors to the given speed.
     * @param left left motor speed
     * @param right right motor speed
     * @note speed is in range [-MOTOR_MAX_SPEED, MOTOR_MAX_SPEED]
     */
    static void motor(int left, int right);


    void turnTo(double angle);
    void dropKit();

    void onUpdate();
    void reset();
    void debug();
};

