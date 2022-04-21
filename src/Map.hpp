#ifndef EXTRATERRESTRIAL_MAZE_MAP_HPP
#define EXTRATERRESTRIAL_MAZE_MAP_HPP

#include <math.h>
#include "BoardInfo.hpp"

#define LEFT 1
#define RIGHT 2
#define TOP 3
#define BOTTOM 4

#define TILES_PER_MAP 100

class Tile {
private:

    // The tile's position on the map assuming there's only one floor
    int x = 0, y = 0;

    // pointers for quadruple linked list
    // nullptr if not in use, otherwise points to next tile
    Tile *left = nullptr;
    Tile *right = nullptr;
    Tile *top = nullptr;
    Tile *bottom = nullptr;

    // true if this tile is a checkpoint
    bool checkpoint = false;

public:

    // Constructor
    Tile() = default;

    // constructor for Tile with previous tile. Previous tile is used to link.
    // Direction is used to determine which side of the previous tile this tile is on.
    // The direction is seen from the previous tile's perspective.
    Tile(Tile* prev, int direction);

    // returns pointer to tile in given direction
    Tile *getTile(int direction);

    // adds a tile, that already exists, in the given direction.
    void addExistingTile(int direction, Tile *tile);

    // creates and adds a tile in the given direction.
    void addNewTile(int direction);

    // marks this tile as a checkpoint
    void markCheckpoint();

    // returns true if this tile is a checkpoint
    bool isCheckpoint() const;
};




class Map {
public:
    Map() = default;

    void addTile(Tile *tile, int direction, Tile tile1);
    Tile *getTile(Tile *tile, int direction);
    Tile *getTile(int x, int y);
private:
    int length = 0;
    Tile map[TILES_PER_MAP];

    void correctTile(Tile *tile);
    void correctMap();
};


#endif //EXTRATERRESTRIAL_MAZE_MAP_HPP