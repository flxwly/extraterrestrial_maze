#ifndef EXTRATERRESTRIAL_MAZE_MAP_HPP
#define EXTRATERRESTRIAL_MAZE_MAP_HPP

#include <math.h>

#define EAST 1
#define NORTH 2
#define WEST 3
#define SOUTH 4

#define TILES_PER_MAP 100

class Tile {
public:
    // pointers for quadruple linked list
    class Tile *left = nullptr;
    class Tile *right = nullptr;
    class Tile *top = nullptr;
    class Tile *bottom = nullptr;

    int x = 0;
    int y = 0;

    int heatedVictims = 0;
    int visualVictims = 0;
    int exits = 0;

    bool checkpoint = false;
    bool black = false; // TODO: Add better name for tile that shouldnt be passed

    static void addDirectionalAttribute(int *ptr, int direction);
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