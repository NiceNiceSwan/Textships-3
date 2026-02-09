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
    _scale = 1;
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
    offset_texture.x += _position.x;
    offset_texture.x *= _scale;
    offset_texture.y += _position.y;
    offset_texture.y *= _scale;
    offset_texture.h *= _scale;
    offset_texture.w *= _scale;
    return offset_texture;
}

/**
 * @brief offsets the given position by camera's position, 
 * assuring that it is located in the right spot in the world
 * 
 * @param _position position that we want changed
 * @return Position position offset by camera's position
 */
Position Camera::offset_position(Position position)
{
    Position offset_position = position;
    offset_position.x += _position.x;
    offset_position.x *= _scale;
    offset_position.y += _position.y;
    offset_position.y *= _scale;
    return offset_position;
}

/**
 * @brief offsets the given position by camera's position, 
 * assuring that it is located in the right spot in the world.
 * This one operates in reverse to the normal one, where instead
 * of adding the position and multiplying by the scale, it divides by the scale and THEN
 * subtracts the position. Used in the selection rect calculations
 * 
 * @param _position position that we want changed
 * @return Position position offset by camera's position
 */
Position Camera::offset_position_reverse(Position position)
{
    Position offset_position = position;
    offset_position.x /= _scale;
    offset_position.x -= _position.x;
    offset_position.y /= _scale;
    offset_position.y -= _position.y;
    return offset_position;
}

void Camera::adjust_scale(double multiplier)
{
    _scale *= multiplier;
    _position.x -= _view_width / (2 * _scale);
    _position.y -= _view_height / (2 * _scale);
}

void Camera::scroll_scale(int scroll_amount)
{
    double scaling_factor;
    if (scroll_amount > 0)
    {
        scaling_factor = 2;
        _scale *= scaling_factor;
        _position.x -= _view_width / (2 * _scale);
        _position.y -= _view_height / (2 * _scale);
        _position.x -= (int)_position.x % (int)(32 * _scale);
        _position.y -= (int)_position.y % (int)(32 * _scale);
        return;
    }
    if (scroll_amount < 0)
    {
        scaling_factor = 0.5;
        _position.x += _view_width / (2 * _scale);
        _position.y += _view_height / (2 * _scale);
        _position.x -= (int)_position.x % (int)(32 * _scale);
        _position.y -= (int)_position.y % (int)(32 * _scale);
        _scale *= scaling_factor;
        return;
    }
    
}
