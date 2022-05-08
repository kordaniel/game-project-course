#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "Geometry.hpp"
#include "Renderer.hpp"
#include "Timetools.hpp"
#include "Transform.hpp"
#include "Physics.hpp"
#include "Input.hpp"

#include <memory>


class GameObject;


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
    std::unique_ptr<Command> _buttonUp;
    std::unique_ptr<Command> _buttonDown;
    std::unique_ptr<Command> _buttonLeft;
    std::unique_ptr<Command> _buttonRight;
    std::unique_ptr<Command> _buttonSpace;

};


class GraphicsComponent
{
public:
    GraphicsComponent(const GameObject& parent);
    GraphicsComponent(const GraphicsComponent& other) = delete;
    GraphicsComponent(GraphicsComponent&& other)      = delete;
    ~GraphicsComponent(void) = default;

    void Draw(const Renderer& renderer, Timestep it) const;

private:
    const GameObject& _parent;

};


class GameObject : public DrawableObject
{
public:
    static std::unique_ptr<GameObject> CreatePlayer(Input& input, float posX, float posY, float moveSpeed, float radius);

public:
    GameObject(Input& input, float posX, float posY, float moveSpeed, float radius);
    GameObject(const GameObject& other) = delete;
    GameObject(GameObject&& other)      = delete;
    ~GameObject(void) = default;

    bool  IsAlive(void)                   const;
    float GetRadius(void)                 const;
    const glm::vec4& GetPosition(void)    const;
    const glm::vec4& GetVelocity(void)    const;
    const Transform& GetTransform(void)   const;

    void SetPosition(float xPos, float yPos);
    void SetRadius(float radius);
    void UpdateRadius(float factor);

    void HandleInput(void);
    /// Update the status of the object.
    /// @param physics Physics engine to use.
    /// @param boundaries 2D rectangle specifying the min/max boundaries for the position one the XY-plane.
    /// @param dt The deltatime length for the update.
    void Update(const Physics& physics, Dimensions2D boundaries, Timestep dt);
    void ApplyForce(Physics::Direction direction, float force);
    void ApplyForce(float angleDegrees, float force);

    /// Draws the object on the renderers current target buffer
    /// @param renderer Renderer to use.
    /// @param it Interpolation timestep for correcting position between updates.
    virtual void Draw(const Renderer& renderer, Timestep it) const override;

private:
    InputComponent    _inputComponent;
    GraphicsComponent _graphics;
    Transform         _transform;

    float             _radius;

};

#endif // GAMEOBJECT_HPP
