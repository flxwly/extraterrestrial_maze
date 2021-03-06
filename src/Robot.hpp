#include <Arduino.h>
#include <DFRobot_MLX90614.h>
#include "Compass.hpp"
#include "Map.hpp"
#include "LaserRangeSensor.hpp"
#include "BoardInfo.hpp"
#include "Servo.h"

#define SERVO_DEFAULT_POSITION 52
#define SERVO_RETREAT_POSITION 90
#define KIT_DROP_COOLDOWN_TIME 10
#define PROBABILITY_OF_TURNING_INTO_OPEN_SPACE 30
#define MIN_SHORT_DISTANCE_TO_RECOGNIZE_OPEN_SPACE 40
#define MIN_DURATION_AFTER_SHORT_DISTANCE_TO_TURN_INTO_OPEN_SPACE 10000
#define MINIMAL_TEMPERATURE_DIFFERENCE_FOR_HEATED_OBJECT 5

#define MIN_DIST_FOR_WALL 15

class Robot {

private:

    DFRobot_MLX90614_IIC thermometer;
    Servo servo;
    Compass compass;

    double startOrientation = 0;
    int targetedOrientation = 0;
    int turnState = 0;


    Tile *curTile = new Tile();
    int lastLRSensorReadingLeft = 0;
    int lastLRSensorReadingRight = 0;
    unsigned int lastAngle = 0;

    // time of last kit drop
    unsigned long lastKitDrop = 0;
    unsigned long lastShortDistanceReading = 0;

public:
    Robot() = default;

    /**
     * Initializes the robot.
     * @note this method must be called before any other method.
     */
    void init();

    /**
     * Sets the motors to the given speed.
     * @param left left motor speed
     * @param right right motor speed
     * @note speed is in range [-MOTOR_MAX_SPEED, MOTOR_MAX_SPEED]
     */
    static void motor(int left, int right);


    /**
     * Turns the robot to the given direction.
     * @param dir direction to turn to
     * @note dir is in range [-180, 180]
     */
    void turnTo(int dir);

    void scanForHeatedObject();

    void scanForOpenSpace();

    /**
     * Drops a rescue kit and blinks the onboard LED.
     */
    void dropKit();

    void checkForNewTiles();

    /**
     * Update loop for the robot.
     */
    void onUpdate();

    /**
     * resets the robots starting orientation.
     */
    void reset();

    /**
     * Outputs various information about the robot to the serial monitor.
     */
    void debug();

    void avoidCollisions();
};

