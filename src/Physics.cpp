#include "Physics.hpp"
#include "Helpers.hpp"
#include "Logger.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/exponential.hpp>

#include <cassert>


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
Physics::Update(PhysicsObject& physicsObject) const
{
    glm::mat4 scaleTrans = glm::scale(
        glm::translate(glm::mat4(1.0f), _gravity),
        _friction
    );
    physicsObject._acceleration *= scaleTrans;
}

const glm::vec3&
Physics::GetGravity(void) const
{
    return _gravity;
}

const glm::vec3&
Physics::GetFriction(void) const
{
    return _friction;
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


PhysicsObject::PhysicsObject(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& acceleration)
    : _position(glm::vec4(position, 1.0f))
    , _velocity(glm::vec4(velocity, 1.0f))
    , _acceleration(glm::translate(glm::mat4(1.0f), acceleration))
{
    //
}

void
PhysicsObject::ApplyForce(Physics::Direction direction, float force)
{
    switch (direction)
    {
        case Physics::Direction::WEST:  _acceleration[3].x -= force; break;
        case Physics::Direction::NORTH: _acceleration[3].y -= force; break;
        case Physics::Direction::EAST:  _acceleration[3].x += force; break;
        case Physics::Direction::SOUTH: _acceleration[3].y += force; break;
    }
}

void
PhysicsObject::ApplyForce(float angleDegrees, float force)
{
    // TODO: Refactor, get rid of "manual" building of translation matrix

    // last argument (&v) defines the plane we want to rotate around and it must be a unit vector.
    glm::mat4 rotation = glm::rotate(Physics::Mat4Id, glm::radians(angleDegrees), Physics::BasisZAxis);
    glm::vec4 transVec = rotation * glm::vec4(0.0f, -force, 0.0f, 1.0f);
    glm::mat4 translation = glm::translate(Physics::Mat4Id, glm::vec3(transVec.x, transVec.y, transVec.z));

    _acceleration *= translation;
}

void
PhysicsObject::SetPosition(const glm::vec3& position)
{
    _position = glm::vec4(position, 1.0f);
}

void
PhysicsObject::SetVelocity(const glm::vec3& velocity)
{
    _velocity = glm::vec4(velocity, 1.0f);
}

const glm::vec4&
PhysicsObject::GetPosition(void) const { return _position; }

const glm::vec4&
PhysicsObject::GetVelocity(void) const { return _velocity; }

const glm::mat4&
PhysicsObject::GetAcceleration(void) const { return _acceleration; }
