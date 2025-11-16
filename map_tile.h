#ifndef MAP_TILE_H
#define MAP_TILE_H

#include "ship.h"

class Map_tile
{
private:
    bool _ship_is_on_tile;
    Ship* _ship_on_tile;
public:
    Map_tile();

    // setters
    void ship(Ship& ship) { _ship_on_tile = &ship; _ship_is_on_tile = true; };
    void ship(Ship* ship) { _ship_on_tile = ship; _ship_is_on_tile = true; };

    // getters
    Ship* ship() { return _ship_on_tile; };
    bool ship_is_on_tile() { return _ship_is_on_tile; };

    // simple functions
    void clear_tile() { _ship_is_on_tile = false; _ship_on_tile = nullptr; };
};


#endif