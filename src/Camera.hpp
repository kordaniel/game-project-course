#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Geometry.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


class Camera
{
public:
    Camera(void);
    Camera(const Camera& other) = delete;
    Camera(Camera&& other)      = delete;
    ~Camera(void) = default;

    void SetCenterPosition(const glm::vec3& targetPos);
    void SetCenterPosition(const glm::vec4& targetPos);

    /// Moves the camera towards targetPos by doing a linear blend of the current
    /// and target positions with the value of alpha. That is, computes:
    /// curPos * (1.0 - alpha) + targetPos * a, basically:  "(targetPos - curPos) * alpha".
    /// @param alpha A value in the range [0,1].
    void TrackPosition(const glm::vec3& targetPos, float alpha);
    void TrackPosition(const glm::vec4& targetPos, float alpha);
    void SetDimensions(Dimensions2D widthHeight);

    Point2D      Transform(Point2D screenCoords) const;
    Point2D      GetCenterPosition(void)  const;
    Point2D      GetTopLeftPosition(void) const;
    Dimensions2D GetDimensions(void)      const;
    Rectangle    GetRectangle(void)       const;

    /// @return leftmost X
    int          GetX(void)               const;

    /// @return Topmost Y
    int          GetY(void)               const;

    int          GetWidth(void)           const;
    int          GetHeight(void)          const;

private:
    glm::vec3    _position;   // Center pos
    Dimensions2D _dimensions; // Total width and height

};

#endif // CAMERA_HPP
