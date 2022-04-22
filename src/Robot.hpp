#include <Arduino.h>
#include <DFRobot_MLX90614.h>
#include "Compass.hpp"
#include "Map.hpp"
#include "LaserRangeSensor.hpp"
#include "BoardInfo.hpp"
#include "Servo.h"

#define MIN_DIST_FOR_WALL 15


class Robot {

private:

    DFRobot_MLX90614_IIC thermometer;
    Servo servo;
    Compass compass;

    double startOrientation = 0;

    Tile startingTile{};
    double x = 0, y = 0;

public:
    Robot() = default;
    explicit Robot(double orientation);

    /**
     * Sets the motors to the given speed.
     * @param left left motor speed
     * @param right right motor speed
     * @note speed is in range [-MOTOR_MAX_SPEED, MOTOR_MAX_SPEED]
     */
    static void motor(int left, int right);

    static void alignLeft(double maxDif = 0);
    static void alignRight(double maxDif = 0);
    void turnTo(double angle);
    void dropKit();

    void onUpdate();

    //    void changePos(Tile *cur);
    //    bool isWall(int dir);

};

