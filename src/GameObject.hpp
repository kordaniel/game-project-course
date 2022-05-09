#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "Geometry.hpp"
#include "Renderer.hpp"
#include "Timetools.hpp"
#include "Transform.hpp"
#include "Physics.hpp"
#include "Input.hpp"

#include <memory>
#include <unordered_map>


class GameObject;
class PlayerObject;


class Command
{
public:
    Command(void) = default;
    Command(const Command& other) = delete;
    Command(Command&& other)      = delete;
    virtual ~Command(void) = default;
    virtual void ExecuteMovement(Transform& transform) const = 0;

private:

};

class NullCommand : public Command
{
public:
    virtual void ExecuteMovement(Transform& transform) const override;
private:

};

class JumpCommand : public Command
{
public:
    virtual void ExecuteMovement(Transform& transform) const override;
private:
};

class MoveCommand : public Command
{
public:
    MoveCommand(Physics::Direction direction);
    virtual void ExecuteMovement(Transform& transform) const override;

private:
    Physics::Direction _direction;

};

class InputComponent
{
public:
    InputComponent(const Input& input, GameObject& parent);
    ~InputComponent(void) = default;
    void Handle(void);

    // TODO: Implement methods to bind commands to keys

private:
    const Input&             _input;
    GameObject&              _parent;
    std::unordered_map<Input::KeyCode, std::unique_ptr<Command>> _keymaps;
};


class GraphicsComponent
{
public:
    GraphicsComponent(void) = default;
    GraphicsComponent(const GraphicsComponent& other) = delete;
    GraphicsComponent(GraphicsComponent&& other)      = delete;
    ~GraphicsComponent(void) = default;

    void SetParent(const GameObject* parent);
    void Draw(const Renderer& renderer, Timestep it) const;

private:
    const GameObject* _parent;

};


/// Base class for all entities in the game.
class GameObject : public DrawableObject
{
public:
    static std::unique_ptr<PlayerObject> CreatePlayer(Input& input, float posX, float posY, float moveSpeed, float radius);

public:
    GameObject(Input& input, float posX, float posY, float moveSpeed);
    GameObject(const GameObject& other) = delete;
    GameObject(GameObject&& other)      = delete;
    virtual ~GameObject(void) = default;

    bool  IsAlive(void)                 const;
    const glm::vec4& GetPosition(void)  const;
    const glm::vec4& GetVelocity(void)  const;
    const Transform& GetTransform(void) const;

    void SetPosition(float xPos, float yPos);

    void HandleInput(void);

    /// Update the status of the object.
    /// @param physics Physics engine to use.
    /// @param boundaries 2D rectangle specifying the min/max boundaries for the position one the XY-plane.
    /// @param dt The deltatime length for the update.
    virtual void Update(const Physics& physics, Dimensions2D boundaries, Timestep dt) = 0;

    void ApplyForce(Physics::Direction direction, float force);
    void ApplyForce(float angleDegrees, float force);

    /// Draws the object on the renderers current target buffer
    /// @param renderer Renderer to use.
    /// @param it Interpolation timestep for correcting position between updates.
    virtual void Draw(const Renderer& renderer, Timestep it) const override;

protected:
    InputComponent    _inputComponent;
    GraphicsComponent _graphicsComponent;
    Transform         _transform;

};


class PlayerObject : public GameObject
{
public:
    PlayerObject(Input& input, float posX, float posY, float moveSpeed, float radius);
    PlayerObject(const PlayerObject& other) = delete;
    PlayerObject(PlayerObject&& other) = delete;
    ~PlayerObject(void) = default;

    float GetRadius(void) const;
    void  SetRadius(float radius);
    void  UpdateRadius(float factor);

    virtual void Update(const Physics& physics, Dimensions2D boundaries, Timestep dt) override;

private:
    float _radius;

};

#endif // GAMEOBJECT_HPP
