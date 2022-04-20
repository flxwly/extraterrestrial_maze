#ifndef EXTRATERRESTRIAL_MAZE_COMPASS_HPP
#define EXTRATERRESTRIAL_MAZE_COMPASS_HPP

#include <Wire.h>

#define CMPS14_ADDRESS 0x60
#define ANGLE_8 1

class Compass {
    unsigned char high_byte{0}, low_byte{0};
    unsigned char angle8{0}, pitch{0}, roll{0};
    unsigned int angle16{0};

public:
    Compass() = default;

    void read() {
        Wire.beginTransmission(CMPS14_ADDRESS);  // Starts communication with the sensor
        Wire.write(ANGLE_8);                     // Sends the register we wish to start reading from
        Wire.endTransmission();

        // Request 5 bytes from the sensor
        // this will give us the 8 bit bearing,
        // both bytes of the 16 bit bearing, pitch and roll
        Wire.requestFrom(CMPS14_ADDRESS, 5);
        while(Wire.available() < 5);        // Wait for all bytes to come back

        angle8 = Wire.read();               // Read back the 5 bytes
        high_byte = Wire.read();
        low_byte = Wire.read();
        pitch = Wire.read();
        roll = Wire.read();

        angle16 = high_byte;                 // Calculate 16 bit angle
        angle16 <<= 8;
        angle16 += low_byte;
    };
    unsigned int getAngle() const {return angle16;};
    int getPitch() const {return pitch;};
    int getRoll() const {return roll;};
};


#endif //EXTRATERRESTRIAL_MAZE_COMPASS_HPP
