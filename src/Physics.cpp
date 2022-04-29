#include "Physics.hpp"
#include "Helpers.hpp"
#include "Logger.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/exponential.hpp>
#include <glm/gtx/string_cast.hpp>

#include <cassert>


void
Physics::LogMat4(const glm::mat4& m, const std::string_view message)
{ // Static function
    std::string matStr = glm::to_string(m);
    Helpers::ReplaceAll(matStr, "), ", ")\n");
    Helpers::ReplaceAll(matStr, "((", "(\n(");
    if (message.empty()) {
        Logger::Debug("{}", matStr);
    } else {
        Logger::Debug("{}:\n{}", message, matStr);
    }
}

Physics::Physics(float gravityY, float friction)
    : Physics(0.0f, gravityY, friction)
{
    //
}

Physics::Physics(float gravityX, float gravityY, float friction)
    : _gravity(gravityX, gravityY, 0.0f)
    , _friction(1.0f - friction, 1.0f - friction, 1.0f - friction)
{
    assert(0.0f <= friction);
    assert(friction <= 1.0f);
}

void
Physics::Update(PhysicsObject& physicsOb, Timestep dt) const
{
    glm::mat4 gravity = glm::translate(
        glm::mat4(1.0f),
        static_cast<float>(dt) * _gravity
    );

    // TODO: Vectorize or use cached friction & gravity mat4, update only when args change
    glm::mat4 friction = glm::mat4(1.0f);
    friction[0].x = glm::pow(_friction.x, static_cast<float>(dt));
    friction[1].y = glm::pow(_friction.y, static_cast<float>(dt));
    friction[2].z = glm::pow(_friction.z, static_cast<float>(dt));

    physicsOb._velocity = gravity * friction * physicsOb._velocity;
}

void
Physics::SetGravity(float gravityY)
{
    SetGravity(0.0f, gravityY);
}

void
Physics::SetGravity(float gravityX, float gravityY)
{
    _gravity = glm::vec3(gravityX, gravityY, 0.0f);
}

void
Physics::SetFriction(float friction)
{
    SetFriction(friction, friction, friction);
}

void
Physics::SetFriction(float frictionX, float frictionY, float frictionZ)
{
    assert(0 <= frictionX); assert(frictionX <= 1.0f);
    assert(0 <= frictionY); assert(frictionY <= 1.0f);
    assert(0 <= frictionZ); assert(frictionZ <= 1.0f);

    _friction = glm::vec3(1.0f - frictionX, 1.0f - frictionY, 1.0f - frictionZ);
}


PhysicsObject::PhysicsObject(const glm::vec3& position, const glm::vec3& velocity)
    : _position(glm::vec4(position, 1.0f))
    , _velocity(glm::vec4(velocity, 1.0f))
{
    //
}

void
PhysicsObject::ApplyForce(Physics::Direction direction, float force, Timestep dt)
{
    force *= static_cast<float>(dt);
    switch (direction)
    {
        case Physics::Direction::WEST:  _velocity.x -= force; break;
        case Physics::Direction::NORTH: _velocity.y -= force; break;
        case Physics::Direction::EAST:  _velocity.x += force; break;
        case Physics::Direction::SOUTH: _velocity.y += force; break;
    }
}

void
PhysicsObject::ApplyForce(float angleDegrees, float force, Timestep dt)
{
    glm::mat4 transpose(1.0f);

    // last argument (&v) defines the plane we want to rotate around and it must be a unit vector.
    transpose = glm::rotate(transpose, glm::radians(angleDegrees), Physics::basisZAxis);
    _velocity += transpose * glm::vec4(0.0f, -static_cast<float>(dt) * force, 0.0f, 0.0f);
}

const glm::vec4&
PhysicsObject::GetPosition(void) const { return _position; }

const glm::vec4&
PhysicsObject::GetVelocity(void) const { return _velocity; }
