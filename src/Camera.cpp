#include "Camera.hpp"
#include "Logger.hpp"

#include <glm/common.hpp>


Camera::Camera(void)
    : _position(glm::vec3(0.0f))
    , _dimensions{0, 0}
{
    //
}

void
Camera::SetCenterPosition(const glm::vec3& targetPos)
{
    _position = targetPos;
}

void
Camera::SetCenterPosition(const glm::vec4& targetPos)
{
    _position = glm::vec3(targetPos);
}

void
Camera::TrackPosition(const glm::vec3& targetPos, float alpha)
{
    _position = glm::mix(_position, targetPos, alpha);
}

void
Camera::TrackPosition(const glm::vec4& targetPos, float alpha)
{
    TrackPosition(glm::vec3(targetPos), alpha);
}

void
Camera::SetDimensions(Dimensions2D widthHeight)
{
    _dimensions = widthHeight;
}

Point2D
Camera::Transform(Point2D screenCoords) const
{
    screenCoords.X -= GetX();
    //screenCoords.Y -= GetY();
    return screenCoords;
}

Point2D
Camera::GetCenterPosition(void) const
{
    return {
        static_cast<int>(_position.x + 0.5f),
        static_cast<int>(_position.y + 0.5f)
    };
}

Point2D
Camera::GetTopLeftPosition(void) const { return { GetX(), GetY() }; }

Dimensions2D
Camera::GetDimensions(void) const { return _dimensions; }

Rectangle
Camera::GetRectangle(void) const
{
    const Point2D centerPos = GetCenterPosition();

    return {
        GetX(), GetY(),
        centerPos.X + (_dimensions.W / 2),
        centerPos.Y + (_dimensions.H / 2)
    };
}

int
Camera::GetX(void) const { return static_cast<int>(_position.x + 0.5f) - (GetWidth() / 2); }

int
Camera::GetY(void) const { return static_cast<int>(_position.y + 0.5f) - (GetHeight() / 2); }

int
Camera::GetWidth(void) const { return _dimensions.W; }

int
Camera::GetHeight(void) const { return _dimensions.H; }
