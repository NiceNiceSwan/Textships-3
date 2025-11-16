#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include <string>
#include <stdexcept>

#include <SDL3/SDL.h>

#include "position.h"

enum Ship_class
{
    destroyer = 0,
    cruiser = 1,
    battleship = 2,
    carrier = 3,
    submarine = 4
};

class Ship
{
private:
    int _team;
    std::string _name;
    char _type;
    bool _visible;
    int _hp;
    int _attack;
    int _defense;
    int _range;
    bool _can_fire;
    int _probable_detection_range;
    int _certain_detection_range;
    int _movement_range;
    int _remaining_movement;

    SDL_Texture* _texture;

    static std::vector<SDL_Texture*> _texture_cache;

    Position _position;

    static SDL_Texture* initialize_texture(std::string path, SDL_Renderer* renderer);
public:
    // Constructors
    Ship();
    Ship(int team);
    static Ship Destroyer(int team);
    static Ship Cruiser(int team);
    static Ship Battleship(int team);
    static Ship Carrier(int team);
    static Ship Submarine(int team);

    // getters
    int team() { return _team; };
    std::string name() { return _name; };
    char type() { return _type; };
    bool visible() { return _visible; };
    int hp() { return _hp; };
    int attack() { return _attack; };
    int defense() { return _defense; };
    int range() { return _range; };
    bool can_fire() { return _can_fire; };
    int probable_detection_range() { return _probable_detection_range; };
    int certain_detection_range() { return _certain_detection_range; };
    int movement_range() { return _movement_range; };
    int remaining_movement() { return _remaining_movement; };
    Position position() { return _position; };

    // setters
    void visible(bool visible) { _visible = visible; };
    void can_fire(bool can_fire) { _can_fire = can_fire; };
    void position(Position position) { _position = position; };
    void position(int x, int y) { _position = Position(x, y); };

    // simple functions
    void add_remaining_movement(int amount) { _remaining_movement += amount; };
    void add_hp(int amount) { _hp += amount; };

    // functions
    void refresh_data();
    static void initialize_texture_cache(SDL_Renderer* renderer);
};


#endif