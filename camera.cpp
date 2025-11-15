#include "camera.h"

Camera::Camera()
{
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