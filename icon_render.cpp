#include "icon_renderer.h"

/// @brief generates the app icon
/// @param window window for which the icon is applied, should be the main window
/// @return false if there is a failure, true if it succeeds
bool render_icon(SDL_Window* window)
{
    SDL_Surface *surface = NULL;
    char* BMP_path = nullptr;

    SDL_asprintf(&BMP_path, "%simages/textships_background_filled.bmp", SDL_GetBasePath());
    surface = SDL_LoadBMP(BMP_path);
    if (!surface) {
        SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
        return false;
    }

    SDL_SetWindowIcon(window, surface);

    SDL_free(BMP_path);
    SDL_DestroySurface(surface);

    return true;
}