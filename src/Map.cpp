#include "Map.hpp"

void Tile::addDirectionalAttribute(int *ptr, int direction) {
    // Check if there already is an exit
    if ((*ptr & (int) pow(2, direction)) >= 1) {
        return;
    }
    // Add exit in binary
    *ptr += (int) pow(2, direction);
}

void Map::addTile(Tile *tile, int direction, Tile tile1) {
    if (length >= TILES_PER_MAP - 1) {
        // Map is full
        return;
    }
    length += 1;
    map[length] = tile1;

    switch (direction) {
        case EAST:
            tile->right = &map[length];
            map[length].left = tile;
            map[length].x = tile->x + 1;
            map[length].y = tile->y;
            break;
        case NORTH:
            tile->top = &map[length];
            map[length].bottom = tile;
            map[length].x = tile->x;
            map[length].y = tile->y + 1;
            break;
        case WEST:
            tile->left = &map[length];
            map[length].right = tile;
            map[length].x = tile->x - 1;
            map[length].y = tile->y;
            break;
        case SOUTH:
            tile->bottom = &map[length];
            map[length].top = tile;
            map[length].x = tile->x;
            map[length].y = tile->y - 1;
            break;
        default:
            break;
    }
    correctTile(&map[length]);
}

void Map::correctTile(Tile *tile) {
    for (int i = 0; i < length; ++i) {
        if (!tile->right && map[i].x == tile->x + 1 && map[i].y == tile->y) {
            tile->right = &map[i];
            map[i].left = tile;
        } else if (!tile->top && map[i].x == tile->x && map[i].y == tile->y + 1) {
            tile->top = &map[i];
            map[i].bottom = tile;
        } else if (!tile->left && map[i].x == tile->x - 1 && map[i].y == tile->y) {
            tile->left = &map[i];
            map[i].right = tile;
        } else if (!tile->bottom && map[i].x == tile->x && map[i].y == tile->y - 1) {
            tile->bottom = &map[i];
            map[i].top = tile;
        }
    }
}

void Map::correctMap() {
    Tile *cur = nullptr;
    for (int i = 0; i < length; ++i) {
        cur = &map[i];
        for (int j = i + 1; j < length; ++j) {
            if (!cur->right && map[j].x == cur->x + 1 && map[j].y == cur->y) {
                cur->right = &map[j];
                map[j].left = cur;
            } else if (!cur->top && map[j].x == cur->x && map[j].y == cur->y + 1) {
                cur->top = &map[j];
                map[j].bottom = cur;
            } else if (!cur->left && map[j].x == cur->x - 1 && map[j].y == cur->y) {
                cur->left = &map[j];
                map[j].right = cur;
            } else if (!cur->bottom && map[j].x == cur->x && map[j].y == cur->y - 1) {
                cur->bottom = &map[j];
                map[j].top = cur;
            }
        }
    }
}

Tile *Map::getTile(Tile *tile, int direction) {
    switch (direction) {
        case EAST:
            return tile->right;
        case NORTH:
            return tile->top;
        case WEST:
            return tile->left;
        case SOUTH:
            return tile->bottom;
        default:
            return nullptr;
    }
}

Tile *Map::getTile(int x, int y) {
    for (int i = 0; i < length; ++i) {
        if (map[i].x == x and map[i].y == y) {
            return &map[i];
        }
    }
    return nullptr;
}
