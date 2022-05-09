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
    , _keymaps()
{
    _keymaps.emplace(Input::KeyCode::UP,    std::make_unique<MoveCommand>(Physics::Direction::NORTH));
    _keymaps.emplace(Input::KeyCode::DOWN,  std::make_unique<MoveCommand>(Physics::Direction::SOUTH));
    _keymaps.emplace(Input::KeyCode::LEFT,  std::make_unique<MoveCommand>(Physics::Direction::WEST));
    _keymaps.emplace(Input::KeyCode::RIGHT, std::make_unique<MoveCommand>(Physics::Direction::EAST));
    _keymaps.emplace(Input::KeyCode::SPACE, std::make_unique<JumpCommand>());
}

void
InputComponent::Handle(void)
{
    Transform& transform = const_cast<Transform&>(_parent.GetTransform());
    for (auto const& [keyCode, command] : _keymaps) {
        if (_input.IsPressed(keyCode)) {
            command.get()->ExecuteMovement(transform);
        }
    }
}


void
GraphicsComponent::SetParent(const GameObject* parent)
{
    assert(parent != nullptr);
    _parent = parent;
}

void
GraphicsComponent::Draw(const Renderer& renderer, Timestep it) const
{
    assert(_parent != nullptr);

    if (const PlayerObject* ptr = dynamic_cast<const PlayerObject*>(_parent))
    {
        renderer.SetRenderDrawColor({ Constants::Colors::RED });
        renderer.DrawCircleFilled(
             ptr->GetTransform().GetScreenCoords(it),
             static_cast<int>(ptr->GetRadius() + 0.5f)
        );
    } else {
        Logger::Debug("Dynamic casting of GraphicsComponent owner failed");
        assert(false);
    }
}


std::unique_ptr<PlayerObject>
GameObject::CreatePlayer(Input& input, float posX, float posY, float moveSpeed, float radius)
{ // Static function
    return std::make_unique<PlayerObject>(input, posX, posY, moveSpeed, radius);
}

GameObject::GameObject(Input& input, float posX, float posY, float moveSpeed)
    : _inputComponent(input, *this)
    , _graphicsComponent()
    , _transform(posX, posY, moveSpeed)
{
    //
}

bool
GameObject::IsAlive(void) const { return true; }

const glm::vec4&
GameObject::GetPosition(void) const { return _transform.GetPosition(); }

const glm::vec4&
GameObject::GetVelocity(void) const { return _transform.GetVelocity(); }

const Transform&
GameObject::GetTransform(void) const { return _transform; }

void
GameObject::SetPosition(float xPos, float yPos)
{
    _transform.SetPosition(glm::vec3(xPos, yPos, 0.0f));
}

void
GameObject::HandleInput(void)
{
    _inputComponent.Handle();
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
    _graphicsComponent.Draw(renderer, it);
}


PlayerObject::PlayerObject(Input& input, float posX, float posY, float moveSpeed, float radius)
    : GameObject(input, posX, posY, moveSpeed)
    , _radius(radius)
{
    _graphicsComponent.SetParent(this);
}

float
PlayerObject::GetRadius(void) const { return _radius; }

void
PlayerObject::SetRadius(float radius)
{
    _radius = radius;
}

void
PlayerObject::UpdateRadius(float factor)
{
    _radius *= factor;
}

void
PlayerObject::Update(const Physics& physics, Dimensions2D boundaries, Timestep dt)
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
