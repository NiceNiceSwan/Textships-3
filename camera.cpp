#include "camera.h"

Camera::Camera()
{
    _position = Position(0, 0);
    _view_width = WINDOW_WIDTH;
    _view_height = WINDOW_HEIGHT;
}

Camera::Camera(Position position, int width, int height)
{
    _position = position;
    _view_width = width;
    _view_height = height;
}

Camera::~Camera()
{
}

void Camera::reset_position()
{
    _position = Position(0, 0);
    _view_width = WINDOW_WIDTH;
    _view_height = WINDOW_HEIGHT;
}

SDL_FRect Camera::offset_position(SDL_FRect texture)
{
    SDL_FRect offset_texture = texture;
    offset_texture.x += position().x;
    offset_texture.y += position().y;
    return offset_texture;
}

Position Camera::offset_position(Position _position)
{
    Position offset_position = _position;
    offset_position.x += position().x;
    offset_position.y += position().y;
    return offset_position;
}
