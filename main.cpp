#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_mouse.h>
#include <iostream>

#include "camera.h"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 560

SDL_Window* window;
SDL_Renderer* renderer;
static SDL_Texture *texture = NULL;
static int texture_width = 0;
static int texture_height = 0;
Position position(0, 0);
Camera camera(position, WINDOW_WIDTH, WINDOW_HEIGHT);

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
    // create a window
    // window = SDL_CreateWindow("Hello, Triangle!", 960, 540, SDL_WINDOW_RESIZABLE);
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("funny dog", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    SDL_SetRenderDrawColor(renderer, 0, 64, 128, SDL_ALPHA_OPAQUE);  /* dark cyan, full alpha */
    SDL_RenderClear(renderer);  /* start with a blank canvas. */
    SDL_RenderPresent(renderer);

    SDL_Surface *surface = NULL;
    char* BMP_path = nullptr;

    SDL_asprintf(&BMP_path, "%sfunny_dog.BMP", SDL_GetBasePath());  /* allocate a string of the full file path */
    surface = SDL_LoadBMP(BMP_path);
    if (!surface) {
        SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_free(BMP_path);

    texture_width = surface->w;
    texture_height = surface->h;

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_DestroySurface(surface);  /* done with this, the texture has a copy of the pixels now. */

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_SetRenderDrawColor(renderer, 0, 64, 128, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_FRect rect = { 0, 0, (float) texture_width, (float) texture_height };;
    // SDL_Rect viewport;

    // rect.x = (WINDOW_WIDTH - texture_width) / 2;
    // rect.y = (WINDOW_HEIGHT - texture_height) / 2;
    // rect.h = texture_height;
    // rect.w = texture_width;

    SDL_Rect viewport;
    viewport.x = camera.position().x;
    viewport.y = camera.position().y;
    viewport.w = camera.view_width();
    viewport.h = camera.view_height();
    SDL_SetRenderViewport(renderer, &viewport);
    SDL_RenderTexture(renderer, texture, NULL, &rect);
    // viewport.x = 0;
    // viewport.y = 0;
    // viewport.w = WINDOW_WIDTH / 2;
    // viewport.h = WINDOW_HEIGHT / 2;
    // SDL_SetRenderViewport(renderer, NULL);  /* NULL means "use the whole window" */
    // SDL_RenderTexture(renderer, texture, NULL, &rect);


    // viewport.x = WINDOW_WIDTH / 2;
    // viewport.y = WINDOW_HEIGHT / 4;
    // viewport.w = texture_width / 2;
    // viewport.h = texture_height / 2;
    // SDL_SetRenderViewport(renderer, &viewport);  /* NULL means "use the whole window" */
    // SDL_RenderTexture(renderer, texture, NULL, &rect);

    // viewport.x = (texture_width + WINDOW_WIDTH) / 2;
    // viewport.y = WINDOW_HEIGHT / 4;
    // viewport.w = texture_width / 2;
    // viewport.h = texture_height / 2;
    // SDL_SetRenderViewport(renderer, &viewport);  /* NULL means "use the whole window" */
    // SDL_RenderTexture(renderer, texture, NULL, &rect);

    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    static int mouse_press = 0;
    // close the window on request
    if (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
    {
        return SDL_APP_SUCCESS;
    }
    if (event->type == SDL_EVENT_MOUSE_MOTION)
    {
        float x;
        float y;
        SDL_GetMouseState(&x, &y);
        // SDL_GetGlobalMouseState(&x, &y);
        std::cout << "mouse position x: " << x << ", position y: " << y << std::endl;
    }
    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        if (event->key.key == SDLK_A)
        {
            position = camera.position();
            position.x -= 100;
            camera.position(position);
        }
        if (event->key.key == SDLK_D)
        {
            position = camera.position();
            position.x += 100;
            camera.position(position);
        }
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    SDL_DestroyTexture(texture);
    // destroy the window
    SDL_DestroyWindow(window);
}