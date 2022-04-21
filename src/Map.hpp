#ifndef EXTRATERRESTRIAL_MAZE_MAP_HPP
#define EXTRATERRESTRIAL_MAZE_MAP_HPP

#include <math.h>
#include "BoardInfo.hpp"
#include <vector>

#define EAST 1
#define NORTH 2
#define WEST 3
#define SOUTH 4

#define LEFT 1
#define RIGHT 2
#define TOP 3
#define BOTTOM 4

#define TILES_PER_MAP 100

class Tile {
public:
    // pointers for quadruple linked list
    //nullptr - undef; obj wall - wall
    Tile *left = nullptr;
    Tile *right = nullptr;
    Tile *top = nullptr;
    Tile *bottom = nullptr;


    // int heatedVictims = 0;
    // int visualVictims = 0;

    bool checkpoint = false; 
    bool black = false; // TODO: Add better name for tile that shouldnt be passed

    static void addDirectionalAttribute(int *ptr, int direction);

    Tile(Tile *prev, int dir);
    void addTile(Tile *_new, int dir);
};

Tile::Tile(Tile *prev, int dir){
    //set prev dir
    switch (dir)
    {
    case LEFT:
        left = prev;
        break;
    case RIGHT:
        right = prev;
        break;
    case TOP:
        top = prev;
        break;
    case BOTTOM:
        bottom = prev;
        break;
    }
}

void Tile::addTile(Tile *_new, int dir){
    switch (dir)
    {
    case LEFT:
        left = _new;
        break;
    case RIGHT:
        right = _new;
        break;
    case TOP:
        top = _new;
        break;
    case BOTTOM:
        bottom = _new;
        break;
    }
}




class Map {
public:
    Map() = default;

    void addTile(Tile *tile, int direction, Tile tile1);
    Tile *getTile(Tile *tile, int direction);
    Tile *getTile(int x, int y);
private:
    int length = 0;
    Tile map[TILES_PER_MAP];
    
    std::vector<Tile*> checkpoints;



    void correctTile(Tile *tile);
    void correctMap();
};


#endif //EXTRATERRESTRIAL_MAZE_MAP_HPP