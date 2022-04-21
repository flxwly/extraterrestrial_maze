#include "Map.hpp"

#define MIN_DIST_FOR_WALL 15


class Robot
{
private:
    Tile *Pos = nullptr;
    
    // Map map = Map();
    

public:
    Robot(Tile *start);
    void changePos(Tile *cur);
    bool isWall(int dir);

    //sensors
    double getDist(int adress);
    
};

Robot::Robot(Tile *start)
{
    Pos = start;
}


///Sensors
//TODO IR-Sensor
double Robot::getDist(int adress){
    int k = 0;
    return k;
}



///
void Robot::changePos(Tile *cur){
    Pos = cur;
}

