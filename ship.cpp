#include "ship.h"

std::vector<SDL_Texture*> Ship::_texture_cache;

/**
 * @brief sets the ship up as a destroyer
 * 
 * @param team team to which the ship belongs to
 * @return Ship configured as a destroyer
 */
Ship Ship::Destroyer(int team)
{
    Ship new_ship(team);
    new_ship._name = "Destroyer";
    new_ship._type = Ship_class::destroyer;
    new_ship._hp = 10;
    new_ship._attack = 4;
    new_ship._defense = 1;
    new_ship._range = 4;
    new_ship._probable_detection_range = 10;
    new_ship._certain_detection_range = 8;
    new_ship._movement_range = 7;
    new_ship._remaining_movement = new_ship._movement_range;
    new_ship._team = team;
    new_ship._texture = _texture_cache[Ship_class::destroyer];
    return new_ship;
}

/**
 * @brief sets the ship up as a cruiser
 * 
 * @param team team to which the ship belongs to
 * @return Ship configured as a cruiser
 */
Ship Ship::Cruiser(int team)
{
    Ship new_ship(team);
    new_ship._name = "Cruiser";
    new_ship._type = Ship_class::cruiser;
    new_ship._hp = 15;
    new_ship._attack = 7;
    new_ship._defense = 1;
    new_ship._range = 8;
    new_ship._probable_detection_range = 7;
    new_ship._certain_detection_range = 5;
    new_ship._movement_range = 5;
    new_ship._remaining_movement = new_ship._movement_range;
    new_ship._team = team;
    new_ship._texture = _texture_cache[Ship_class::cruiser];
    return new_ship;
}

/**
 * @brief sets the ship up as a battleship
 * 
 * @param team team to which the ship belongs to
 * @return Ship configured as a battleship
 */
Ship Ship::Battleship(int team)
{
    Ship new_ship(team);
    new_ship._name = "Battleship";
    new_ship._type = Ship_class::battleship;
    new_ship._hp = 25;
    new_ship._attack = 12;
    new_ship._defense = 1;
    new_ship._range = 12;
    new_ship._probable_detection_range = 5;
    new_ship._certain_detection_range = 3;
    new_ship._movement_range = 3;
    new_ship._remaining_movement = new_ship._movement_range;
    new_ship._team = team;
    new_ship._texture = _texture_cache[Ship_class::battleship];
    return new_ship;
}

/// @brief not implemented right now
/// @return a new ship configured as an aircraft carrier
Ship Ship::Carrier(int team)
{
    Ship new_ship;
    return new_ship;
}

/**
 * @brief not implemented right now
 * 
 * @return Ship configured as a submarine
 */
Ship Ship::Submarine(int team)
{
    Ship new_ship;
    return new_ship;
}

Ship::Ship()
{
    _can_fire = true;
    _visible = false;
}

Ship::Ship(int team)
{
    _can_fire = true;
    _team = team;
    _visible = false;
}

void Ship::refresh_data()
{
    _can_fire = true;
    _remaining_movement = _movement_range;
    _visible = false;
}

void Ship::initialize_texture_cache(SDL_Renderer* renderer)
{
    SDL_Texture* texture = NULL;
    std::vector<std::string> texture_paths;
    texture_paths.push_back("images/destroyer.bmp");
    texture_paths.push_back("images/cruiser.bmp");
    texture_paths.push_back("images/battleship.bmp");
    for (size_t texture_index = 0; texture_index < texture_paths.size(); texture_index++)
    {
        texture = initialize_texture(texture_paths[texture_index], renderer);
        if (!texture)
        {
            throw std::runtime_error("Error loading textures!");
        }
        Ship::_texture_cache.push_back(texture);
    }
}

SDL_Texture* Ship::initialize_texture(std::string path, SDL_Renderer* renderer)
{
    SDL_Surface *surface = NULL;
    char* BMP_path = nullptr;

    path = "%s" + path;

    SDL_asprintf(&BMP_path, path.c_str(), SDL_GetBasePath());
    surface = SDL_LoadBMP(BMP_path);
    if (!surface) {
        SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
        return NULL;
    }

    SDL_free(BMP_path);

    SDL_Texture* texture = NULL;

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureScaleMode(texture, SDL_ScaleMode::SDL_SCALEMODE_NEAREST);
    if (!texture) {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        return NULL;
    }

    SDL_DestroySurface(surface);

    return texture;
}