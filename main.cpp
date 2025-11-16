#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_mouse.h>
#include <time.h>
#include <random>

#include "camera.h"
#include "game.h"
#include "ship.h"
#include "icon_renderer.h"

std::mt19937_64 random_number_generator((unsigned long long int)time(NULL));

const short int WINDOW_WIDTH = 1600;
const short int WINDOW_HEIGHT = 900;

/// @brief grid size of our map
const short int MAP_SIZE_X = 50;
/// @brief grid size of our map
const short int MAP_SIZE_Y = 50;
/// @brief the size of a single grid
const short int PIXEL_SIZE = 32;

const short int TEAM_1 = 0;
const short int TEAM_2 = 1;
const short int SHIPS_IN_TEAM = 5;

SDL_Window* window;
SDL_Renderer* renderer;
static int texture_width = 0;
static int texture_height = 0;
static float scale = 1;
Position position(0, 0);
SDL_FRect selection_rect;

Game game;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Textships", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    if (!render_icon(window))
    {
        return SDL_APP_FAILURE;
    }

    Ship::initialize_texture_cache(renderer);
    game.initialize();
    game.draw(renderer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
    {
        return SDL_APP_SUCCESS;
    }

    switch (game.game_state())
    {
    case Game_state::WAITING:
        game.waiting_event(*event);
        break;
        
    case Game_state::TURN:
        game.turn_event(*event);
        break;

    default:
        break;
    }

    game.draw(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    // SDL_DestroyTexture(texture);
    // destroy the window
    SDL_DestroyWindow(window);
}