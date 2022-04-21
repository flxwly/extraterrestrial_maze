#ifndef EXTRATERRESTRIAL_MAZE_LASERRANGESENSOR_HPP
#define EXTRATERRESTRIAL_MAZE_LASERRANGESENSOR_HPP

#define LASER_RANGE_SENSOR_K_VALUE 0.1090
#define LASER_RANGE_SENSOR_B_VALUE 54.71

/**
 * @param address The address of the sensor.
 * @return the distance of the laser range sensor
 * @note the distance is in cm
 */
double getDist(int address) {
    return LASER_RANGE_SENSOR_B_VALUE - LASER_RANGE_SENSOR_K_VALUE * analogRead(address);
}


#endif //EXTRATERRESTRIAL_MAZE_LASERRANGESENSOR_HPP
