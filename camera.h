#ifndef CAMERA_H
#define CAMERA_H

#include "position.h"
#include <SDL3/SDL.h>

extern const short int WINDOW_WIDTH;
extern const short int WINDOW_HEIGHT;

class Camera
{
private:
    Position _position;
    int _view_width;
    int _view_height;
    double _scale;

public:
    // constructors
    Camera();
    Camera(Position position, int width, int height);
    ~Camera();

    // getters
    Position position() { return _position; }
    int view_width() { return _view_width; }
    int view_height() { return _view_height; }
    double scale() { return _scale; }

    // setters
    void position(Position position) { _position = position; }
    void view_width(int width) { _view_width = width; }
    void view_height(int height) { _view_height = height; }
    void scale(double scale) { _scale = scale; }

    // simple functions

    // functions
    void reset_position();
    SDL_FRect offset_position(SDL_FRect rectangle);
    Position offset_position(Position position);
    /// @brief  doesn't exist for now
    SDL_FRect offset_position_reverse(SDL_FRect rectangle);
    Position offset_position_reverse(Position position);
    void adjust_scale(double multiplier);
    void scroll_scale(int scroll_amount);
};

#endif