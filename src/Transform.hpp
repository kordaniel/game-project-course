#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Physics.hpp"
#include "Timetools.hpp"
#include "Geometry.hpp"


class Transform : public PhysicsObject
{
public:
    Transform(void);
    Transform(float posX, float posY, float moveForce);
    Transform(const glm::vec3& position, float moveForce);
    Transform(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& acceleration, float moveForce);
    Transform(const Transform& other) = delete;
    Transform(Transform&& other)      = delete;
    ~Transform(void) = default;

    virtual void UpdatePhysics(const Physics& physics, RectangleF boundaries, Timestep dt) override;

    Point2D GetScreenCoords(Timestep it) const;
    float   GetMoveForce(void)           const;

private:
    float _moveForce; // The basic "speed" that is used to apply motion to this object
};

#endif // TRANSFORM_HPP
