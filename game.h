#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <vector>
#include <random>

#include "ship.h"
#include "map_tile.h"
#include "camera.h"
#include "position.h"

extern std::mt19937_64 random_number_generator;

extern const short int WINDOW_WIDTH;
extern const short int WINDOW_HEIGHT;
extern const short int MAP_SIZE_X;
extern const short int MAP_SIZE_Y;
extern const short int PIXEL_SIZE;
extern const short int TEAM_1;
extern const short int TEAM_2;
extern const short int SHIPS_IN_TEAM;

typedef enum Game_state
{
    OVER,       // game is concluding
    TURN,       // a turn is in progress
    WAITING     // waiting for turn change
} Game_state;

class Game
{
private:
    std::vector<std::vector<Ship>> _ships;
    std::vector<std::vector<Map_tile>> _map;

    bool _quit_game;

    Game_state _game_state;
    Camera _camera;

    SDL_FRect _selection_rect;
    Position _selection_rect_position;
    bool _has_selection_rect;


    bool _selected_a_ship;
    Ship* _selected_ship;

    int _player;

    // functions
    void wait_screen(SDL_Renderer* renderer);
    void draw_map(SDL_Renderer* renderer);
    void draw_ships(SDL_Renderer* renderer);
    void draw_border_lines(SDL_Renderer* renderer);
    void draw_coordinate_lines(SDL_Renderer* renderer);
    void draw_selection_rect(SDL_Renderer* renderer);

public:
    Game();
    void initialize();

    Game_state event_handler(SDL_Event event);
    void waiting_event(SDL_Event event);
    void turn_event(SDL_Event event);
    void mouse_input(SDL_Event event);

    // getters
    Game_state game_state() { return _game_state; }

    // setters

    // functions
    void draw(SDL_Renderer* renderer);
    bool end_game();
    void play();
    void turn();
    void mouse_input();
    void select_ship(Position map_position, Position mouse_position);
    void move_ship(Position clicked_map_tile);
    void mark_visible_ships();
    void refresh_ships();
    bool hit_a_ship(Ship* target_ship);
    void kill_a_ship(int team_of_dead_ship);
    void generate_spawning_positions(std::uniform_int_distribution<int> spawning_positions, int ship_count, int team);
};


#endif