#include "GameObject.hpp"
#include "Logger.hpp"
#include "Constants.hpp"
#include "Physics.hpp"


void
NullCommand::ExecuteMovement([[maybe_unused]] Transform& transform) const
{
    // Does nothing, command is not bound to anything.
}

void
JumpCommand::ExecuteMovement(Transform& transform) const
{
    transform.ApplyForce(Physics::Direction::NORTH, 5.0f * transform.GetMoveForce());
}

MoveCommand::MoveCommand(Physics::Direction direction)
    : _direction(direction)
{
    //
}

void
MoveCommand::ExecuteMovement(Transform& transform) const
{
    float force = transform.GetMoveForce();
    switch (_direction)
    {
        case Physics::Direction::NORTH: force *= 2.0f; break;
        case Physics::Direction::EAST:  force *= 1.0f; break;
        case Physics::Direction::SOUTH: force *= 0.5f; break;
        case Physics::Direction::WEST:  force *= 1.0f; break;
    }
    transform.ApplyForce(_direction, force);
}

InputComponent::InputComponent(const Input& input, GameObject& parent)
    : _input(input)
    , _parent(parent)
    , _buttonUp(std::make_unique<MoveCommand>(Physics::Direction::NORTH))
    , _buttonDown(std::make_unique<MoveCommand>(Physics::Direction::SOUTH))
    , _buttonLeft(std::make_unique<MoveCommand>(Physics::Direction::WEST))
    , _buttonRight(std::make_unique<MoveCommand>(Physics::Direction::EAST))
    , _buttonSpace(std::make_unique<JumpCommand>())
{
    //
}

void
InputComponent::Handle(void)
{
    Transform& transform = const_cast<Transform&>(_parent.GetTransform());
    if (_input.IsPressed(Input::KeyCode::UP))    { _buttonUp.get()   ->ExecuteMovement(transform); }
    if (_input.IsPressed(Input::KeyCode::DOWN))  { _buttonDown.get() ->ExecuteMovement(transform); }
    if (_input.IsPressed(Input::KeyCode::LEFT))  { _buttonLeft.get() ->ExecuteMovement(transform); }
    if (_input.IsPressed(Input::KeyCode::RIGHT)) { _buttonRight.get()->ExecuteMovement(transform); }
    if (_input.IsPressed(Input::KeyCode::SPACE)) { _buttonSpace.get()->ExecuteMovement(transform); }

    /* Todo, refactor to return functor objects (input class needs to be refactored)
    if (_input.IsPressed(Input::KeyCode::UP))    { return _buttonUp.get(); }
    if (_input.IsPressed(Input::KeyCode::DOWN))  { return _buttonDown.get(); }
    if (_input.IsPressed(Input::KeyCode::LEFT))  { return _buttonLeft.get(); }
    if (_input.IsPressed(Input::KeyCode::RIGHT)) { return _buttonRight.get(); }
    if (_input.IsPressed(Input::KeyCode::SPACE)) { return _buttonSpace.get(); }
    return nullptr;
    */
}

GraphicsComponent::GraphicsComponent(const GameObject& parent)
    : _parent(parent)
{
    //
}

void
GraphicsComponent::Draw(const Renderer& renderer, Timestep it) const
{
    renderer.SetRenderDrawColor({ Constants::Colors::RED });
    renderer.DrawCircleFilled(
         _parent.GetTransform().GetScreenCoords(it),
         static_cast<int>(_parent.GetRadius() + 0.5f)
    );
}

std::unique_ptr<GameObject>
GameObject::CreatePlayer(Input& input, float posX, float posY, float moveSpeed, float radius)
{ // static function
    return std::make_unique<GameObject>(input, posX, posY, moveSpeed, radius);
}

GameObject::GameObject(Input& input, float posX, float posY, float moveSpeed, float radius)
    : _inputComponent(input, *this)
    , _graphics(*this)
    , _transform(posX, posY, moveSpeed)
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

const Transform&
GameObject::GetTransform(void) const
{
    return _transform;
}

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
GameObject::HandleInput(void)
{
    _inputComponent.Handle();
    // TODO: Refactor commands to return functions
    //Command* command = _inputComponent.Handle(); => if (command) command(args...);
}

void
GameObject::Update(const Physics& physics, Dimensions2D boundaries, Timestep dt)
{
    _transform.UpdatePhysics(
        physics,
        {
            _radius,
            _radius,
            static_cast<float>(boundaries.W) - _radius,
            static_cast<float>(boundaries.H) - _radius
        },
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
