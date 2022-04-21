#include "Map.hpp"


Tile *Tile::getTile(int direction) {
    switch (direction) {
        case TOP:
            return this->top;
        case RIGHT:
            return this->right;
        case BOTTOM:
            return this->bottom;
        case LEFT:
            return this->left;
        default:
            return nullptr;
    }
}

void Tile::addExistingTile(int direction, Tile *tile) {
    switch (direction) {
        case TOP:
            this->top = tile;
            break;
        case RIGHT:
            this->right = tile;
            break;
        case BOTTOM:
            this->bottom = tile;
            break;
        case LEFT:
            this->left = tile;
            break;
        default:
            break;
    }
}

void Tile::addNewTile(int direction) {
    switch (direction) {
        case TOP:
            this->top = new Tile(this, direction);
            break;
        case RIGHT:
            this->right = new Tile(this, direction);
            break;
        case BOTTOM:
            this->bottom = new Tile(this, direction);
            break;
        case LEFT:
            this->left = new Tile(this, direction);
            break;
        default:
            break;
    }
}

Tile::Tile(Tile *prev, int direction) {
    switch (direction) {
        case TOP:
            this->top = prev;
            this->x = prev->x;
            this->y = prev->y + 1;
            break;
        case RIGHT:
            this->right = prev;
            this->x = prev->x + 1;
            this->y = prev->y;
            break;
        case BOTTOM:
            this->bottom = prev;
            this->x = prev->x;
            this->y = prev->y - 1;
            break;
        case LEFT:
            this->left = prev;
            this->x = prev->x - 1;
            this->y = prev->y;
            break;
        default:
            break;
    }
}

void Tile::markCheckpoint() {
    this->checkpoint = true;
}

bool Tile::isCheckpoint() const {
    return this->checkpoint;
}