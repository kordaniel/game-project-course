#include "GameObject.hpp"
#include "Logger.hpp"
#include "Constants.hpp"


GraphicsComponent::GraphicsComponent(const GameObject* parent)
    : _parent(parent)
{
    assert(_parent != nullptr);
}

void
GraphicsComponent::Draw(const Renderer& renderer, Timestep it) const
{
    renderer.SetRenderDrawColor({ Constants::Colors::RED });
    renderer.DrawCircleFilled(
         _parent->_transform.GetScreenCoords(it),
         static_cast<int>(_parent->_radius + 0.5f)
    );
}

std::unique_ptr<GameObject>
GameObject::CreatePlayer(float posX, float posY, float radius)
{ // static function
    return std::make_unique<GameObject>(posX, posY, radius);
}

GameObject::GameObject(float posX, float posY, float radius)
    : _input()
    , _graphics(this)
    , _transform(posX, posY)
    , _radius(radius)
{
    //
}

bool
GameObject::IsAlive(void) const { return true; }

float
GameObject::GetRadius(void) const { return _radius; }

const glm::vec4&
GameObject::GetPosition(void) const { return _transform.GetPosition(); }

const glm::vec4&
GameObject::GetVelocity(void) const { return _transform.GetVelocity(); }

void
GameObject::SetRadius(float radius)
{
    _radius = radius;
}

void
GameObject::UpdateRadius(float factor)
{
    _radius *= factor;
}

void
GameObject::Update(const Physics& physics, Dimensions2D boundaries, Timestep dt)
{
    _transform.UpdatePhysics(
        physics,
        { _radius, _radius, boundaries.W - _radius, boundaries.H - _radius },
        dt
    );
}

void
GameObject::ApplyForce(Physics::Direction direction, float force)
{
    _transform.ApplyForce(direction, force);
}

void
GameObject::ApplyForce(float angleDegrees, float force)
{
    _transform.ApplyForce(angleDegrees, force);
}

void
GameObject::Draw(const Renderer& renderer, Timestep it) const
{ // virtual override member from DrawableObject
    _graphics.Draw(renderer, it);
}
