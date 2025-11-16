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

/**
 * @brief offsets the position of the given rectangle by camera's position, 
 * assuring that the rect is drawn in the correct spot.
 * 
 * @param rectangle rectangle which positions will be changed
 * @return SDL_FRect the same rectangle, offset by camera's position
 */
SDL_FRect Camera::offset_position(SDL_FRect rectangle)
{
    SDL_FRect offset_texture = rectangle;
    offset_texture.x += position().x;
    offset_texture.y += position().y;
    return offset_texture;
}

/**
 * @brief offsets the given position by camera's position, 
 * assuring that it is located in the right spot in the world
 * 
 * @param _position position that we want changed
 * @return Position position offset by camera's position
 */
Position Camera::offset_position(Position _position)
{
    Position offset_position = _position;
    offset_position.x += position().x;
    offset_position.y += position().y;
    return offset_position;
}
