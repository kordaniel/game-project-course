#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "Timetools.hpp"
#include "Geometry.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>


class PhysicsObject;


/// Physics engine. Computes gravity & friction and updates objects that implement
/// the interface PhysicsObject.
/// NOTE: Currently all operations are defined only for the XY 2D-plane even if all
/// data structures are chosen with a 3-dimensional space in mind.
/// Definitions:
/// - (0, 0) is the upper left corner
/// - All directions defaults to north, so all angles are computed from this direction
class Physics
{
public:
    enum class Direction { WEST, NORTH, EAST, SOUTH };

public:
    /// Constructor
    /// @param gravityY The amount of force that pulls the objects along the Y-axis.
    /// @param friction The friction in the range [0,1] that is applied to all objects. 0 = no friction, 1 = max.
    Physics(float gravityY = 100.0f, float friction = 0.9f);

    /// Constructor
    /// @param gravityX The amount of force that pulls the objects along the X-axis.
    /// @param gravityY The amount of force that pulls the objects along the Y-axis.
    /// @param friction The friction in the range [0,1] that is applied to all objects. 0 = no friction, 1 = max.
    Physics(float gravityX, float gravityY, float friction);
    Physics(const Physics& other) = delete;
    Physics(Physics&& other)      = delete;
    ~Physics(void) = default;

    void Update(PhysicsObject& physicsObject) const;
    const glm::vec3& GetGravity(void)  const;
    const glm::vec3& GetFriction(void) const;
    void SetGravity(float gravityY);
    void SetGravity(float gravityX, float gravityY);
    void SetFriction(float friction);
    void SetFriction(float frictionX, float frictionY, float frictionZ);

public:
    // Basis vectors, must be unit vectors with length = 1. This is not asserted!
    constexpr inline static glm::vec3 BasisXAxis = glm::vec3(1.0f, 0.0f, 0.0f); // Rotates on YZ-plane
    constexpr inline static glm::vec3 BasisYAxis = glm::vec3(0.0f, 1.0f, 0.0f); // Rotates on XZ-plane
    constexpr inline static glm::vec3 BasisZAxis = glm::vec3(0.0f, 0.0f, 1.0f); // Rotates on XY-plane

    // Identity 4x4 matrix
    constexpr inline static glm::mat4 Mat4Id = glm::mat4(1.0f);

private:
    glm::vec3 _gravity;
    glm::vec3 _friction; // values in [0,1]

};



class PhysicsObject
{
    friend void Physics::Update(PhysicsObject&) const;

public:
    PhysicsObject(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& acceleration);
    PhysicsObject(const PhysicsObject& other) = delete;
    PhysicsObject(PhysicsObject&& other)      = delete;
    virtual ~PhysicsObject(void) = default;

    /// @param physicsEngine The physics engine that will update the acceleration vector of this object.
    /// @param boundaries Boundaries for XY coords; .X/.Y = top left, left, .W/.H == bottom right.
    /// @param dt The deltatime length for the update.
    virtual void UpdatePhysics(const Physics& physicsEngine, RectangleF boundaries, Timestep dt) = 0;

    void ApplyForce(Physics::Direction direction, float force);
    void ApplyForce(float angleDegrees, float force);

    void SetPosition(const glm::vec3& position);
    void SetVelocity(const glm::vec3& velocity);
    void SetYVelocityZero(void);

    const glm::vec4& GetPosition(void)     const;
    const glm::vec4& GetVelocity(void)     const;
    const glm::mat4& GetAcceleration(void) const;

protected:
    glm::vec4 _position;
    glm::vec4 _velocity;
    glm::mat4 _acceleration;

};

#endif // PHYSICS_HPP
