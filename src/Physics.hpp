#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "Timetools.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <string_view>


class PhysicsObject;


/// Physics engine. Computes gravity & friction and updates objects that implement
/// the interface PhysicsObject based on deltaTime.
/// NOTE: Currently all operations are defined only for the XY 2D-plane even if all
/// data structures are chosen with 3D in mind.
/// Definitions:
/// - (0, 0) is the upper left corner
/// - All directions defaults to north, so all angles are computed from this direction
class Physics
{
public:
    enum class Direction { WEST, NORTH, EAST, SOUTH };

    static void LogMat4(const glm::mat4& m, const std::string_view message = "");

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

    void Update(PhysicsObject& transform, Timestep dt) const;

    void SetGravity(float gravityY);
    void SetGravity(float gravityX, float gravityY);
    void SetFriction(float friction);
    void SetFriction(float frictionX, float frictionY, float frictionZ);

public:
    // Basis vectors, must be unit vectors with length = 1. This is not asserted!
    constexpr inline static glm::vec3 basisXAxis = glm::vec3(1.0f, 0.0f, 0.0f); // Rotates on YZ-plane
    constexpr inline static glm::vec3 basisYAxis = glm::vec3(0.0f, 1.0f, 0.0f); // Rotates on XZ-plane
    constexpr inline static glm::vec3 basisZAxis = glm::vec3(0.0f, 0.0f, 1.0f); // Rotates on XY-plane

private:
    glm::vec3 _gravity;
    glm::vec3 _friction; // values in [0,1]

};



class PhysicsObject
{
    friend void Physics::Update(PhysicsObject&, Timestep) const;

public:
    PhysicsObject(const glm::vec3& position, const glm::vec3& velocity);
    PhysicsObject(const PhysicsObject& other) = delete;
    PhysicsObject(PhysicsObject&& other)      = delete;
    virtual ~PhysicsObject(void) = default;

    virtual void UpdatePhysics(const Physics& physics, Timestep dt) = 0;

    void ApplyForce(Physics::Direction direction, float force, Timestep dt);
    void ApplyForce(float angleDegrees, float force, Timestep dt);

    const glm::vec4& GetPosition(void) const;
    const glm::vec4& GetVelocity(void) const;

protected:
    glm::vec4 _position;
    glm::vec4 _velocity;

};

#endif // PHYSICS_HPP
