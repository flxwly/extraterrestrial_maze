#include "Map.hpp"

void Tile::addDirectionalAttribute(int *ptr, int direction) {
    // Check if there already is an exit
    if ((*ptr & (int) pow(2, direction)) >= 1) {
        return;
    }
    // Add exit in binary
    *ptr += (int) pow(2,direction);
}

void Map::addTile(Map *map, Tile *tile, int direction, Tile tile1) {
    if (map->index >= TILES_PER_MAP - 1) {
        // Map is full
        return;
    }
    map->index += 1;
    map->map[map->index] = tile1;

    switch (direction) {
        case EAST:
            tile->right = &map->map[map->index];
            map->map[map->index].left = tile;
            break;
        case NORTH:
            tile->top = &map->map[map->index];
            map->map[map->index].bottom = tile;
            break;
        case WEST:
            tile->left = &map->map[map->index];
            map->map[map->index].right = tile;
            break;
        case SOUTH:
            tile->bottom = &map->map[map->index];
            map->map[map->index].top = tile;
            break;
        default:
            break;
    }
}
