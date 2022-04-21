#include <Arduino.h>
#include "Map.hpp"
#include "BoardInfo.hpp"

#define MIN_DIST_FOR_WALL 15


class Robot {

private:
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


    void onUpdate();

    //    void changePos(Tile *cur);
    //    bool isWall(int dir);

};

