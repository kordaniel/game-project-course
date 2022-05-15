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

Rectangle
Camera::TransformRectangle(RectangleF rect) const
{
    Rectangle transform = {
        static_cast<int>(rect.X + 0.5f),
        static_cast<int>(rect.Y + 0.5f),
        static_cast<int>(rect.W + 0.5f),
        static_cast<int>(rect.H + 0.5f),
    };
    transform.X -= GetX();
    return transform;
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

RectangleF
Camera::GetRectangleF(void) const
{
    return {
        _position.x - 0.5f * static_cast<float>(_dimensions.W),
        _position.y - 0.5f * static_cast<float>(_dimensions.H),
        static_cast<float>(_dimensions.W),
        static_cast<float>(_dimensions.H)
    };
}

bool
Camera::RectangleIsInViewport(const RectangleF& rect) const
{
    const RectangleF camRect = GetRectangleF();

    // Check if argument rect is inside the vieport on the x-axis
    if (!GetRectangleF().OverlapsX(rect)) {
        return false;
    }

    // Same for y-axis
    if (!GetRectangleF().OverlapsY(rect)) {
        return false;
    }

    return true;
}

int
Camera::GetX(void) const { return static_cast<int>(_position.x + 0.5f) - (GetWidth() / 2); }

int
Camera::GetY(void) const { return static_cast<int>(_position.y + 0.5f) - (GetHeight() / 2); }

int
Camera::GetWidth(void) const { return _dimensions.W; }

int
Camera::GetHeight(void) const { return _dimensions.H; }
