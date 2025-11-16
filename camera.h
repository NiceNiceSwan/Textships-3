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

public:
    // constructors
    Camera();
    Camera(Position position, int width, int height);
    ~Camera();

    // getters
    Position position() { return _position; }
    int view_width() { return _view_width; }
    int view_height() { return _view_height; }

    // setters
    void position(Position position) { _position = position; }
    void view_width(int width) { _view_width = width; }
    void view_height(int height) { _view_height = height; }

    // functions
    void reset_position();
    SDL_FRect offset_position(SDL_FRect rectangle);
    Position offset_position(Position position);
};

#endif