#include "game.h"
#include "position.h"



// #include "include/BearLibTerminal.h"

Game::Game()
{
    _game_state = Game_state::WAITING;
    _quit_game = false;
    _player = 0;
    _ships.resize(2);
    _map.resize(MAP_SIZE_X, std::vector<Map_tile>(MAP_SIZE_Y));
}

void Game::initialize()
{
    std::uniform_int_distribution<int> player_1_spawn_positions(0, 9);
    std::uniform_int_distribution<int> player_2_spawn_positions(40, 49);

    // // debug positions to test detection and firing
    // std::uniform_int_distribution<int> player_2_spawn_positions(5, 14);


    for (size_t i = 0; i < _ships.size(); i++)
    {
        _ships[i].push_back(Ship::Destroyer(i));
        _ships[i].push_back(Ship::Destroyer(i));
        _ships[i].push_back(Ship::Cruiser(i));
        _ships[i].push_back(Ship::Cruiser(i));
        _ships[i].push_back(Ship::Battleship(i));
    }
    generate_spawning_positions(player_1_spawn_positions, SHIPS_IN_TEAM, TEAM_1);
    generate_spawning_positions(player_2_spawn_positions, SHIPS_IN_TEAM, TEAM_2);
    
}

void Game::draw(SDL_Renderer* renderer)
{
    switch (_game_state)
    {
    case Game_state::WAITING:
        wait_screen(renderer);
        break;
    case Game_state::TURN:
        draw_map(renderer);
        break;
    default:
        break;
    }
}

/// @brief draws the wait screen
/// @param renderer the renderer which will draw the scene
void Game::wait_screen(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_SetRenderScale(renderer, 2, 2);
    std::string text = "Player's " + std::to_string(_player + 1) + " turn.";
    SDL_RenderDebugText(renderer, WINDOW_WIDTH / 8, WINDOW_HEIGHT / 6, text.c_str());
    text = "Press enter to continue";
    SDL_RenderDebugText(renderer, WINDOW_WIDTH / 8, WINDOW_HEIGHT / 4, text.c_str());
    SDL_SetRenderScale(renderer, 1, 1);
    SDL_RenderPresent(renderer);
}

void Game::draw_map(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 64, 128, 255);
    SDL_RenderClear(renderer);

    draw_border_lines(renderer);
    draw_coordinate_lines(renderer);
    draw_selection_rect(renderer);

    draw_ships(renderer);

    SDL_RenderPresent(renderer);
}

void Game::draw_ships(SDL_Renderer* renderer)
{
    SDL_FRect texture_position;

    for (size_t team_id = 0; team_id < 2; team_id++)
    {
        for (size_t ship_id = 0; ship_id < _ships[team_id].size(); ship_id++)
        {
            Ship* ship = &_ships[team_id][ship_id];
            if (ship->visible())
            {
                texture_position.x = ship->position().x * PIXEL_SIZE;
                texture_position.y = ship->position().y * PIXEL_SIZE;
                texture_position.h = (float)PIXEL_SIZE;
                texture_position.w = (float)PIXEL_SIZE;

                texture_position = _camera.offset_position(texture_position);
                
                SDL_RenderTexture(renderer, ship->texture(), NULL, &texture_position);
            }
            
        }
        
    }
}

void Game::draw_border_lines(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_FRect border_rectangle;
    border_rectangle.x = 0;
    border_rectangle.y = 0;
    border_rectangle.w = 1600;
    border_rectangle.h = 1600;
    border_rectangle = _camera.offset_position(border_rectangle);

    SDL_RenderRect(renderer, &border_rectangle);
}

void Game::draw_coordinate_lines(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    Position start_cords(0, 0);
    Position far_points(MAP_SIZE_X * PIXEL_SIZE, MAP_SIZE_Y * PIXEL_SIZE);

    start_cords = _camera.offset_position(start_cords);
    far_points = _camera.offset_position(far_points);

    Position offset_cords;

    for (size_t index = 1; index < 50; index++)
    {
        offset_cords.x = PIXEL_SIZE * index;
        offset_cords.y = PIXEL_SIZE * index;
        offset_cords = _camera.offset_position(offset_cords);

        SDL_RenderLine(renderer, start_cords.x, offset_cords.y, far_points.x, offset_cords.y);
        SDL_RenderLine(renderer, offset_cords.x, start_cords.y, offset_cords.x, far_points.y);
    }
    
}

void Game::draw_selection_rect(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 32, 64, 255);
    Position selection_rect_position = _camera.offset_position(_selection_rect_position);
    _selection_rect.x = selection_rect_position.x;
    _selection_rect.y = selection_rect_position.y;
    _selection_rect.h = PIXEL_SIZE * _camera.scale();
    _selection_rect.w = PIXEL_SIZE * _camera.scale();
    SDL_RenderFillRect(renderer, &_selection_rect);
}

bool Game::end_game()
{
    if (_quit_game)
    {
        return true;
    }
    
    // multithread this bitch >:)
    for (size_t i = 0; i < _ships.size(); i++)
    {
        if (_ships[i].empty())
        {
            return true;
        }
    }
    
    return false;
}

Game_state Game::event_handler(SDL_Event event)
{
    if (_game_state == Game_state::WAITING)
    {
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_RETURN)
        {
            _game_state = Game_state::TURN;
            refresh_ships();
            mark_visible_ships();
            turn();
        }
        return Game_state::WAITING;
    }
    
    return Game_state::WAITING;
}

void Game::waiting_event(SDL_Event event)
{
    if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_RETURN)
    {
        _game_state = Game_state::TURN;
        refresh_ships();
        mark_visible_ships();
        _camera.reset_position();
    }
}

void Game::turn_event(SDL_Event event)
{
    Position position = _camera.position();
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        if (event.key.key == SDLK_D)
        {
            position.x -= 32;
            _camera.position(position);
        }
        if (event.key.key == SDLK_A)
        {
            position.x += 32;
            _camera.position(position);
        }
        if (event.key.key == SDLK_W)
        {
            position.y += 32;
            _camera.position(position);
        }
        if (event.key.key == SDLK_S)
        {
            position.y -= 32;
            _camera.position(position);
        }
    }
    if (event.type == SDL_EVENT_MOUSE_WHEEL)
    {
        _camera.scroll_scale(event.wheel.y);
    }
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        SDL_MouseButtonEvent mouse_event = event.button;
        if (mouse_event.button == SDL_BUTTON_LEFT)
        {
            float x;
            float y;
            SDL_GetMouseState(&x, &y);

            _selection_rect_position = Position(x, y);

            _selection_rect_position.x -= (int)_selection_rect_position.x % (PIXEL_SIZE * (int)_camera.scale());
            _selection_rect_position.y -= (int)_selection_rect_position.y % (PIXEL_SIZE * (int)_camera.scale());
            
            _selection_rect_position = _camera.offset_position_reverse(_selection_rect_position);

            _selection_rect.h = PIXEL_SIZE;
            _selection_rect.w = PIXEL_SIZE;
        }
        if (mouse_event.button == SDL_BUTTON_RIGHT)
        {
            _selection_rect.h = 0;
            _selection_rect.w = 0;
        }
    }
    
}

void Game::mouse_input(SDL_Event event)
{

}

/// @brief houses the main game loop
void Game::play()
{
    // wait_screen();
    refresh_ships();
    mark_visible_ships();
    turn();
    // _player = (_player + 1) % 2;
    if (_player == TEAM_1)
    {
        _player = TEAM_2;
    }
    else
    {
        _player = TEAM_1;
    }
}

void Game::turn()
{
    // _selected_a_ship = false;
    // Position mouse_position;
    // _terminal_manager.create_interface();
    // _terminal_manager.update_map(_map, _player);
    // while (true)
    // {
    //     if (terminal_has_input())
    //     {
    //         int player_input = terminal_read();
    //         if (player_input == TK_MOUSE_LEFT)
    //         {
    //             player_input = 0;
    //             mouse_input();
    //         }
    //         if (player_input == TK_ENTER)
    //         {
    //             break;
    //         }
    //         if (player_input == TK_CLOSE)
    //         {
    //             _quit_game = true;
    //             break;
    //         }
    //     }
    // }
}

void Game::mouse_input()
{
    // Position mouse_position;
    // mouse_position.x = terminal_state(TK_MOUSE_X);
    // mouse_position.y = terminal_state(TK_MOUSE_Y);
    // Position map_tile;
    // if (mouse_position.x <= LEFT_INFO_PANEL_VERTICAL_LINE_ID
    //  || mouse_position.x >= RIGHT_INFO_PANEL_VERTICAL_LINE_ID
    //  || mouse_position.y <= 0
    //  || mouse_position.y >= TERMINAL_HIGHEST_CELL_ID_Y)
    // {
    //     // if we're here this means that the mouse click occured outside the map, and so as to not 
    //     // do any outside of array bounds fuckery we'll just ignore the click
    //     return;
    // }

    // map_tile.x = mouse_position.x - (LEFT_INFO_PANEL_VERTICAL_LINE_ID + 1);
    // map_tile.y = mouse_position.y - 1;

    // if (_map[map_tile.x][map_tile.y].ship_is_on_tile())
    // {
    //     if (_selected_a_ship && _map[map_tile.x][map_tile.y].ship()->team() != _player)
    //     {
    //         hit_a_ship(_map[map_tile.x][map_tile.y].ship());
    //         return;
    //     }
        
    //     // if (_map[map_tile.x][map_tile.y].ship()->team() == _player)
    //     // {
    //         // select_ship(map_tile, mouse_position);
    //         // return;
    //     // }
        
    //     select_ship(map_tile, mouse_position);
    //     return;
    // }

    // if (_selected_a_ship)
    // {
    //     move_ship(map_tile);
    // }
    
    // _selected_a_ship = false;
    // _selected_ship = nullptr;
    // _terminal_manager.clear_info_tab();
    // _terminal_manager.update_map(_map, _player);
}

void Game::select_ship(Position map_position, Position mouse_position)
{
    // Ship* ship = _map[map_position.x][map_position.y].ship();

    // if (ship->visible())
    // {
    //     _terminal_manager.print_ship_stats(*ship, _player);
    // }

    // if (ship->team() == _player)
    // {
    //     // _terminal_manager.show_movement_area(map_position, ship->remaining_movement());
    //     _terminal_manager.show_ship_ranges(*ship);

    //     // if (terminal_state(TK_MOUSE_CLICKS) >= 2)
    //     // {
    //     //     _terminal_manager.underline_ship(mouse_position);
    //     //     _selected_a_ship = true;
    //     //     _selected_ship = ship;
    //     //     return;
    //     // }

    //     _terminal_manager.underline_ship(mouse_position);
    //     _selected_a_ship = true;
    //     _selected_ship = ship;
    //     return;
    // }
    // _selected_a_ship = false;
    // _selected_ship = nullptr;
}

void Game::move_ship(Position clicked_map_tile)
{
    // Position ship_position = _selected_ship->position();
    // double distance = std::pow(clicked_map_tile.x - ship_position.x, 2) + std::pow(clicked_map_tile.y - ship_position.y, 2);
    // distance = std::sqrt(distance);
    // if (distance <= _selected_ship->remaining_movement())
    // {
    //     distance = ceil(distance);
    //     _selected_ship->add_remaining_movement(-distance);
    //     _selected_ship->position(clicked_map_tile);
    //     _map[ship_position.x][ship_position.y].clear_tile();
    //     _map[clicked_map_tile.x][clicked_map_tile.y].ship(_selected_ship);
    //     _selected_a_ship = false;
    //     _selected_ship = nullptr;
    //     _terminal_manager.update_map(_map, _player);
    // }
    // mark_visible_ships();
    // _terminal_manager.update_map(_map, _player);
}

/**
 * @brief marks all visible ships
 * 
 * goes through all friendly ships and marks them as visible, then runs through
 * all enemy ships and marks them if they have been detected
 */
void Game::mark_visible_ships()
{
    double distance;
    int other_player = (_player + 1) % 2;
    
    for (size_t friendly_index = 0; friendly_index < _ships[_player].size(); friendly_index++)
    {
        // make our ships visible
        Ship* friendly_ship = &_ships[_player][friendly_index];
        friendly_ship->visible(true);

        for (size_t enemy_index = 0; enemy_index < _ships[other_player].size(); enemy_index++)
        {
            Ship* enemy_ship = &_ships[other_player][enemy_index];
            distance = Position::distance(friendly_ship->position(), enemy_ship->position());
            if (distance <= friendly_ship->certain_detection_range())
            {
                enemy_ship->visible(true);
            }
        }
    }
}

/// @brief runs through all ships and forces them to refresh their data
void Game::refresh_ships()
{
    for (size_t team = 0; team < _ships.size(); team++)
    {
        for (size_t j = 0; j < _ships[team].size(); j++)
        {
            _ships[team][j].refresh_data();
        }
    }
}

bool Game::hit_a_ship(Ship* target_ship)
{
    // if (_selected_ship->can_fire() != true)
    // {
    //     return false;
    // }

    // Position shooter_position = _selected_ship->position();
    // Position target_position = target_ship->position();
    
    // if (Position::distance(shooter_position, target_position) > _selected_ship->range())
    // {
    //     return false;
    // }
    
    // target_ship->add_hp(-_selected_ship->attack());
    // _selected_ship->can_fire(false);
    // _terminal_manager.overline_ship(_selected_ship->position());
    // _selected_a_ship = false;
    // _selected_ship = nullptr;
    // _terminal_manager.print_ship_stats(*target_ship, _player);

    // if (target_ship->hp() <= 0)
    // {
    //     kill_a_ship(target_ship->team());
    //     _terminal_manager.update_map(_map, _player);
    //     _terminal_manager.clear_enemy_info();
    //     _selected_a_ship = false;
    //     _selected_ship = nullptr;
    //     return true;
    // }
    // return false;
    return false;
}

/**
 * @brief removes all ships of a given team that have 0 or less HP
 * 
 * @param team the team which ships will be checked and removed
 */
void Game::kill_a_ship(int team)
{
    Position ship_position;
    for (int i = 0; i < _ships[team].size(); i++)
    {
        if (_ships[team][i].hp() <= 0)
        {
            ship_position = _ships[team][i].position();
            _map[ship_position.x][ship_position.y].clear_tile();

            _ships[team][i] = _ships[team].back();
            ship_position = _ships[team].back().position();
            _map[ship_position.x][ship_position.y].clear_tile();

            _ships[team].pop_back();
            // _map[ship_position.x][ship_position.y].ship(&_ships[team][i]);
            i--;
        }
    }
    for (size_t i = 0; i < _ships[team].size(); i++)
    {
        ship_position = _ships[team][i].position();
        _map[ship_position.x][ship_position.y].ship(&_ships[team][i]);
    }
    
}

/**
 * @brief places the ships on the map
 * 
 * @param spawning_positions the range of positions on which we can spawn the ships. This is used as the range of x and y coordinates that the ships can spawn on.
 * @param ship_count the amount of ships we wish to spawn
 * @param team the team to which the ships belong to
 */
void Game::generate_spawning_positions(std::uniform_int_distribution<int> spawning_positions, int ship_count, int team)
{
    Position position;
    for (size_t ship_number = 0; ship_number < ship_count; ship_number++)
    {
        do
        {
            position.x = spawning_positions(random_number_generator);
            position.y = spawning_positions(random_number_generator);
        } while (_map[position.x][position.y].ship_is_on_tile());
        _ships[team][ship_number].position(position);
        _map[position.x][position.y].ship(_ships[team][ship_number]);
    }
}
