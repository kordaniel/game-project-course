#include "GameObject.hpp"
#include "Logger.hpp"
#include "Constants.hpp"
#include "Physics.hpp"

#include <cmath>


GameObjectState*
FallingState::HandleUpdate(PlayerObject* parent, const Physics& physics, Dimensions2D boundaries, Timestep dt)
{
    float r = parent->GetRadius();
    parent->GetMutableTransform().UpdatePhysics(
        physics,
        {
            r, r,
            static_cast<float>(boundaries.W) - r,
            static_cast<float>(boundaries.H) - r
        },
        dt
    );
    return nullptr;
}

GameObjectState*
FallingState::HandleInput(InputComponent& inputCmp)
{
    inputCmp.Handle({
        Input::KeyCode::UP,
        Input::KeyCode::DOWN,
        Input::KeyCode::LEFT,
        Input::KeyCode::RIGHT
    });
    return nullptr;
}

GameObjectState*
FallingState::HandleCollisions(PlayerObject* parent, GameObject* obj)
{
    if (parent->GetCollissionRect().Overlaps(obj->GetCollissionRect())) {
        obj->SetColor(Constants::Colors::GREEN);
        if (std::abs(parent->GetTransform().GetVelocity().y) < 1.0f) {
            parent->SetYVelocityStopped();
            float yBounds = parent->GetPosition().y;
            float xBounds = obj->GetCollissionRect().X;
            float xWidth  = xBounds + obj->GetCollissionRect().W;
            //xBounds += 0.5f * parent->GetRadius();
            //xWidth  -= 0.5f * parent->GetRadius();
            return new OnGroundState(yBounds, xBounds, xWidth);
        }

        parent->BounceYAxis();
        return new JumpingState(2);
    }
    return nullptr;
}


JumpingState::JumpingState(size_t jumpsCount)
    : _jumpsLeft(jumpsCount)
{
    //
}

GameObjectState*
JumpingState::HandleUpdate(PlayerObject* parent, const Physics& physics, Dimensions2D boundaries, Timestep dt)
{
    float r = parent->GetRadius();
    parent->GetMutableTransform().UpdatePhysics(
        physics,
        {
            r, r,
            static_cast<float>(boundaries.W) - r,
            static_cast<float>(boundaries.H) - r
        },
        dt
    );

    if (parent->GetVelocity().y > 0.0f) {
        return new FallingState();
    }

    return nullptr;
}

GameObjectState*
JumpingState::HandleInput(InputComponent& inputCmp)
{
    std::vector<Input::KeyCode> activeKeys = {
        Input::KeyCode::LEFT,
        Input::KeyCode::RIGHT,
        Input::KeyCode::DOWN
    };

    if (_jumpsLeft > 0) {
        activeKeys.push_back(Input::KeyCode::UP);
        activeKeys.push_back(Input::KeyCode::SPACE);
    }

    bool spacePressed = inputCmp.Handle(activeKeys);
    if (_jumpsLeft > 0 && spacePressed) {
        _jumpsLeft--;
    }

    return nullptr;
}

GameObjectState*
JumpingState::HandleCollisions([[maybe_unused]] PlayerObject* parent, [[maybe_unused]] GameObject* obj)
{
    // Do nothing
    return nullptr;
}

OnGroundState::OnGroundState(float yBound, float xBound, float xWidth)
    : _yBound(yBound)
    , _xBound(xBound)
    , _xWidth(xWidth)
{
    //Logger::Debug("Player state changed to OnGround");
}

GameObjectState*
OnGroundState::HandleUpdate(PlayerObject* parent, const Physics& physics, Dimensions2D boundaries, Timestep dt)
{
    float r = parent->GetRadius();
    parent->GetMutableTransform().UpdatePhysics(
        physics,
        {
            r, r,
            static_cast<float>(boundaries.W) - r,
            _yBound
        },
        dt
    );

    RectangleF pr = parent->GetCollissionRect();
    if (pr.X + pr.W < _xBound || pr.X > _xWidth) {
        return new FallingState();
    }

    return nullptr;
}

GameObjectState*
OnGroundState::HandleInput(InputComponent& inputCmp)
{
    bool spacePressed = inputCmp.Handle({
        Input::KeyCode::UP,
        Input::KeyCode::DOWN,
        Input::KeyCode::LEFT,
        Input::KeyCode::RIGHT,
        Input::KeyCode::SPACE
    });

    if (spacePressed) {
        return new JumpingState(2);
    }

    return nullptr;
}

GameObjectState*
OnGroundState::HandleCollisions([[maybe_unused]] PlayerObject* parent, [[maybe_unused]] GameObject* obj)
{
    // Do nothing
    return nullptr;
}

void
NullCommand::ExecuteMovement([[maybe_unused]] Transform& transform) const
{
    // Does nothing, command is not bound to anything.
}

void
JumpCommand::ExecuteMovement(Transform& transform) const
{
    transform.ApplyForce(Physics::Direction::NORTH, 12.5f * transform.GetMoveForce());
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


InputComponent::InputComponent(const Input& input)
    : _input(input)
    , _parent(nullptr)
    , _keymaps()
{
    _keymaps.emplace(Input::KeyCode::UP,    std::make_unique<MoveCommand>(Physics::Direction::NORTH));
    _keymaps.emplace(Input::KeyCode::DOWN,  std::make_unique<MoveCommand>(Physics::Direction::SOUTH));
    _keymaps.emplace(Input::KeyCode::LEFT,  std::make_unique<MoveCommand>(Physics::Direction::WEST));
    _keymaps.emplace(Input::KeyCode::RIGHT, std::make_unique<MoveCommand>(Physics::Direction::EAST));
    _keymaps.emplace(Input::KeyCode::SPACE, std::make_unique<JumpCommand>());
}

void
InputComponent::SetParent(GameObject* parent)
{
    _parent = parent;
}

bool
InputComponent::Handle(const std::vector<Input::KeyCode>& keys)
{
    assert(_parent != nullptr);

    bool spacePressed = false;
    Transform& transform = const_cast<Transform&>(_parent->GetTransform());

    for (const auto& keyCode : keys)
    {
        if (_input.IsPressed(keyCode))
        {
            auto cmd = _keymaps.find(keyCode);
            if (cmd != _keymaps.end())
            {
                cmd->second->ExecuteMovement(transform);
                if (keyCode == Input::KeyCode::SPACE)
                {
                    spacePressed = true;
                    dynamic_cast<PlayerObject*>(_parent)->PlayJumpingSound(); // TODO: Quick hack, get rid of this dangerous cast
                }
            }
        }
    }

    return spacePressed;
}


void
GraphicsComponent::SetParent(const GameObject* parent)
{
    assert(parent != nullptr);
    _parent = parent;
}

void
GraphicsComponent::Draw(const Renderer& renderer, const Camera& camera, Timestep it) const
{
    assert(_parent != nullptr);

    if (const PlayerObject* playerPtr = dynamic_cast<const PlayerObject*>(_parent))
    {
        renderer.SetRenderDrawColor(playerPtr->GetColor());
        renderer.DrawCircleFilled(
             camera.Transform(playerPtr->GetTransform().GetScreenCoords(it)),
             static_cast<int>(playerPtr->GetRadius() + 0.5f)
        );

#ifdef DRAW_COLLIDERS
        Rectangle playerRect = camera.TransformRectangle(playerPtr->GetCollissionRect());
        renderer.SetRenderDrawColor({ Constants::Colors::WHITE });
        renderer.DrawRectangle(&playerRect);
#endif

    }
    else if (const BoxObject* ptr = dynamic_cast<const BoxObject*>(_parent))
    {
        renderer.SetRenderDrawColor(ptr->GetColor());
        renderer.DrawFilledRectangle(
            camera.Transform(ptr->GetTransform().GetScreenCoords(it)),
            {
                static_cast<int>(ptr->GetSize().W + 0.5f),
                static_cast<int>(ptr->GetSize().H + 0.5f)
            }
        );

#ifdef DRAW_COLLIDERS
        Rectangle boxRect = camera.TransformRectangle(ptr->GetCollissionRect());
        renderer.SetRenderDrawColor({ Constants::Colors::WHITE });
        renderer.DrawRectangle(&boxRect);
#endif

    }
    else
    {
        Logger::Debug("Dynamic casting of GraphicsComponent owner failed");
        assert(false);
    }
}


std::unique_ptr<PlayerObject>
GameObject::CreatePlayer(Input& input, float posX, float posY, float moveSpeed, float radius, Sound& jumpSound, Color color)
{ // Static function
    return std::make_unique<PlayerObject>(input, posX, posY, moveSpeed, radius, jumpSound, color);
}

std::unique_ptr<GameObject>
GameObject::CreateBox(Input& input, float moveSpeed, Point2DF position, Dimensions2DF size)
{ // Static function
    return std::make_unique<BoxObject>(input, position, size, moveSpeed);
}

GameObject::GameObject(Input& input, float posX, float posY, float moveSpeed, Color color)
    : _inputComponent(input)
    , _graphicsComponent()
    , _transform(posX, posY, moveSpeed)
    , _state(new FallingState())
    , _color(color)
{
    // Logger::Debug("Gameobject Constructed");
}

GameObject::~GameObject(void)
{
    // Logger::Debug("GameObject Destructed");
    if (_state != nullptr) {
        delete _state;
    }
}

bool
GameObject::IsAlive(void) const { return true; }

const glm::vec4&
GameObject::GetPosition(void) const { return _transform.GetPosition(); }

const glm::vec4&
GameObject::GetVelocity(void) const { return _transform.GetVelocity(); }

const Transform&
GameObject::GetTransform(void) const { return _transform; }

Transform&
GameObject::GetMutableTransform(void) { return _transform; }

void
GameObject::SetYVelocityStopped(void)
{
    _transform.SetYVelocityZero();
}

void
GameObject::SetPosition(float xPos, float yPos)
{
    _transform.SetPosition(glm::vec3(xPos, yPos, 0.0f));
}

void
GameObject::SetColor(Color color)
{
    _color = color;
}

Color
GameObject::GetColor(void) const { return _color; }

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
GameObject::Draw(const Renderer& renderer, const Camera& camera, Timestep it) const
{ // virtual override member from DrawableObject
    _graphicsComponent.Draw(renderer, camera, it);
}


PlayerObject::PlayerObject(Input& input, float posX, float posY, float moveSpeed, float radius, Sound& jumpSound, Color color)
    : GameObject(input, posX, posY, moveSpeed, color)
    , _radius(radius)
    , _soundJump(jumpSound)
{
    _inputComponent.SetParent(this);
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

bool
PlayerObject::CheckHitAndBounce(GameObject* obj)
{
    BoxObject* boxObj = dynamic_cast<BoxObject*>(obj);
    if (boxObj == nullptr) {
        Logger::Debug("PlayerObject::CheckHitAndBounce : Unable to cast GameObject to derived type");
        return false;
    }

    GameObjectState* newState = _state->HandleCollisions(this, boxObj);
    if (newState == nullptr) {
        return false;
    }

    delete _state;
    _state = newState;

    return true;

    //if (GetCollissionRect().Overlaps(boxObj->GetCollissionRect()))
    //{
    //    _transform.BounceYAxis();
    //    return true;
    //}

    //return false;
}

void
PlayerObject::BounceYAxis(void)
{
    _transform.BounceYAxis();
}

void
PlayerObject::PlayJumpingSound(void)
{
    _soundJump.Play();
}

void
PlayerObject::HandleInput(void)
{
    GameObjectState* newState = _state->HandleInput(_inputComponent);
    if (newState == nullptr) {
        return;
    }

    delete _state;
    _state = newState;
}

void
PlayerObject::Update(const Physics& physics, Dimensions2D boundaries, Timestep dt)
{
    GameObjectState* newState = _state->HandleUpdate(this, physics, boundaries, dt);
    if (newState == nullptr) {
        return;
    }

    delete _state;
    _state = newState;
}

RectangleF
PlayerObject::GetCollissionRect(void) const
{
    return {
        _transform.GetPosition().x  - _radius,
        _transform.GetPosition().y  - _radius,
        2.0f * _radius,
        2.0f * _radius
    };
}

BoxObject::BoxObject(Input& input, Point2DF position, Dimensions2DF size, float moveSpeed, Color color)
    : GameObject(input, position.X, position.Y, moveSpeed, color)
    , _size(size)
{
    _graphicsComponent.SetParent(this);
}

Dimensions2DF
BoxObject::GetSize(void) const { return _size; }

RectangleF
BoxObject::GetCollissionRect(void) const
{
    return {
        _transform.GetPosition().x - (_size.W / 2),
        _transform.GetPosition().y - (_size.H / 2),
        _size.W,
        _size.H
    };
}

void
BoxObject::HandleInput(void)
{
    // Do nothing
}

void
BoxObject::Update([[maybe_unused]] const Physics& physics, [[maybe_unused]] Dimensions2D boundaries, [[maybe_unused]] Timestep dt)
{
    // Do nothing
}
