#include "GameObject.hpp"

std::unique_ptr<GameObject>
GameObject::CreatePlayer(void)
{ // static function
    return std::make_unique<GameObject>(
        // args..
    );
}

GraphicsComponent::GraphicsComponent(void)
{

}

GameObject::GameObject(void)
    : _cmpGfx()
    , _cmpTrsfrm(*this)
    , _cmpInpPtr(nullptr)
    //, _velocity(0)
    //, _position{ 0, 0 }
{
    //
}

bool
GameObject::IsAlive(void) const
{
    return true;
}

void
GameObject::Update(void) const
{
    //
}

void
GameObject::Render(void) const
{
    //
}

GameObject::Transform::Transform(GameObject& componentOwner)
    : _owner(componentOwner)
    , _X(0.0)
    , _Y(0.0)
    , _V(0.0)
{
    //
}

double
GameObject::Transform::GetX(void) const { return _X; }

double
GameObject::Transform::GetY(void) const { return _Y; }

Point2D
GameObject::Transform::GetPosition(void) const
{
    return {
        static_cast<int>(_X + 0.5),
        static_cast<int>(_Y + 0.5)
    };
}

void
GameObject::Transform::Update(void)
{
    // TODO: Update
}