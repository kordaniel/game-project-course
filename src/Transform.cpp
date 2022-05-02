#include "Transform.hpp"
#include "Logger.hpp"


Transform::Transform(void)
    : Transform(glm::vec3(0.0f))
{
    //
}

Transform::Transform(float posX, float posY)
    : Transform(glm::vec3(posX, posY, 0.0f))
{

}

Transform::Transform(const glm::vec3& position)
    : Transform(position, glm::vec3(0.0f), glm::vec3(1.0f))
{
    //
}

Transform::Transform(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& acceleration)
    : PhysicsObject(position, velocity, acceleration)
{
    //
}

void
Transform::UpdatePhysics(const Physics& physicsEngine, RectangleF boundaries, Timestep dt)
{ // virtual override member function from PhysicsObject
#define BOUNCE // TODO: Delete this hack

    const float deltaTime = static_cast<float>(dt);
    physicsEngine.Update(*this);

    // TODO: Vectorize all operations
    _acceleration[0].x = glm::pow(_acceleration[0].x, deltaTime); // Diagonal (friction)
    _acceleration[1].y = glm::pow(_acceleration[1].y, deltaTime); // Diagonal (friction)
    _acceleration[2].z = glm::pow(_acceleration[2].z, deltaTime); // Diagonal (friction)

    _acceleration[3].x *= deltaTime; // Transform (move)
    _acceleration[3].y *= deltaTime; // Transform (move)
    _acceleration[3].z *= deltaTime; // Transform (move)

    _velocity = _acceleration * _velocity;

    if (_position.x + _velocity.x < boundaries.X) {
        _velocity.x = boundaries.X - _position.x;
    } else if (_position.x + _velocity.x > boundaries.W) {
        _velocity.x = boundaries.W - _position.x;
    }

    if (_position.y + _velocity.y < boundaries.Y) {
        _velocity.y = boundaries.Y - _position.y;
    }
#ifdef BOUNCE
    else if (_velocity.y > 0.0f && _velocity.y + _position.y > boundaries.H) {
        _velocity.y = boundaries.H - _position.y + _velocity.y;
    }
#undef BOUNCE
#else
    else if (_position.y + _velocity.y > boundaries.H) {
        _velocity.y = boundaries.H - _position.y;
    }
#endif

    _position += _velocity;
}

Point2D
Transform::GetScreenCoords(Timestep it) const
{
    return {
        static_cast<int>(GetPosition().x + static_cast<float>(it) * _velocity.x  + 0.5f),
        static_cast<int>(GetPosition().y + static_cast<float>(it) * _velocity.y  + 0.5f)
    };
}
