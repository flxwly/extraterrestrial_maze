#include "Map.hpp"

void Tile::addDirectionalAttribute(int *ptr, int direction) {
    // Check if there already is an exit
    if ((*ptr & (int) pow(2, direction)) >= 1) {
        return;
    }
    // Add exit in binary
    *ptr += (int) pow(2, direction);
}

void Map::addTile(Map *map, Tile *tile, int direction, Tile tile1) {
    if (map->length >= TILES_PER_MAP - 1) {
        // Map is full
        return;
    }
    map->length += 1;
    map->map[map->length] = tile1;

    switch (direction) {
        case EAST:
            tile->right = &map->map[map->length];
            map->map[map->length].left = tile;
            map->map[map->length].x = tile->x + 1;
            map->map[map->length].y = tile->y;
            break;
        case NORTH:
            tile->top = &map->map[map->length];
            map->map[map->length].bottom = tile;
            map->map[map->length].x = tile->x;
            map->map[map->length].y = tile->y + 1;
            break;
        case WEST:
            tile->left = &map->map[map->length];
            map->map[map->length].right = tile;
            map->map[map->length].x = tile->x - 1;
            map->map[map->length].y = tile->y;
            break;
        case SOUTH:
            tile->bottom = &map->map[map->length];
            map->map[map->length].top = tile;
            map->map[map->length].x = tile->x;
            map->map[map->length].y = tile->y - 1;
            break;
        default:
            break;
    }
    correctTile(map, &map->map[map->length]);
}

void Map::correctTile(Map *map, Tile *tile) {
    for (int i = 0; i < map->length; ++i) {
        if (!tile->right && map->map[i].x == tile->x + 1 && map->map[i].y == tile->y) {
            tile->right = &map->map[i];
            map->map[i].left = tile;
        } else if (!tile->top && map->map[i].x == tile->x && map->map[i].y == tile->y + 1) {
            tile->top = &map->map[i];
            map->map[i].bottom = tile;
        } else if (!tile->left && map->map[i].x == tile->x - 1 && map->map[i].y == tile->y) {
            tile->left = &map->map[i];
            map->map[i].right = tile;
        } else if (!tile->bottom && map->map[i].x == tile->x && map->map[i].y == tile->y - 1) {
            tile->bottom = &map->map[i];
            map->map[i].top = tile;
        }
    }
}

void Map::correctMap(Map *map) {
    Tile *cur = nullptr;
    for (int i = 0; i < map->length; ++i) {
        cur = &map->map[i];
        for (int j = i + 1; j < map->length; ++j) {
            if (!cur->right && map->map[j].x == cur->x + 1 && map->map[j].y == cur->y) {
                cur->right = &map->map[j];
                map->map[j].left = cur;
            } else if (!cur->top && map->map[j].x == cur->x && map->map[j].y == cur->y + 1) {
                cur->top = &map->map[j];
                map->map[j].bottom = cur;
            } else if (!cur->left && map->map[j].x == cur->x - 1 && map->map[j].y == cur->y) {
                cur->left = &map->map[j];
                map->map[j].right = cur;
            } else if (!cur->bottom && map->map[j].x == cur->x && map->map[j].y == cur->y - 1) {
                cur->bottom = &map->map[j];
                map->map[j].top = cur;
            }
        }
    }
}