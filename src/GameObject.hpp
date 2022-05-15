#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "Camera.hpp"
#include "Constants.hpp"
#include "Geometry.hpp"
#include "Renderer.hpp"
#include "Timetools.hpp"
#include "Transform.hpp"
#include "Physics.hpp"
#include "Input.hpp"
#include "Sound.hpp"

#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

#define DRAW_COLLIDERS // Define to draw collision rectangles.

class GameObject;
class InputComponent;
class PlayerObject;
class FallingState;
class JumpingState;
class OnGroundState;


class GameObjectState
{
public:
    //enum class STATES : size_t { FALLING = 0, JUMPING, ON_GROUND };

    GameObjectState(void) = default;
    GameObjectState(const GameObjectState& other) = delete;
    GameObjectState(GameObjectState&& other)      = delete;
    virtual ~GameObjectState(void) = default;

    virtual GameObjectState* HandleUpdate(PlayerObject* parent, const Physics& physics, Dimensions2D boundaries, Timestep dt) = 0;
    virtual GameObjectState* HandleInput(InputComponent& inputCmp) = 0;
    virtual GameObjectState* HandleCollisions(PlayerObject* parent, GameObject* obj) = 0;

protected:
    static FallingState  s_falling;
    static JumpingState  s_jumping;
    static OnGroundState s_onGround;

};

class FallingState : public GameObjectState
{
public:
    //FallingState(void);
    //FallingState(const FallingState& other) = delete;
    //FallingState(FallingState&& other)      = delete;
    ~FallingState(void) = default;
    virtual GameObjectState* HandleUpdate(PlayerObject* parent, const Physics& physics, Dimensions2D boundaries, Timestep dt) override;
    virtual GameObjectState* HandleInput(InputComponent& inputCmp) override;
    virtual GameObjectState* HandleCollisions(PlayerObject* parent, GameObject* obj) override;

private:

};

class JumpingState : public GameObjectState
{
public:
    JumpingState(size_t jumpsCount);
    //JumpingState(const JumpingState& other) = delete;
    //JumpingState(JumpingState&& other)      = delete;
    ~JumpingState(void) = default;
    virtual GameObjectState* HandleUpdate(PlayerObject* parent, const Physics& physics, Dimensions2D boundaries, Timestep dt) override;
    virtual GameObjectState* HandleInput(InputComponent& inputCmp) override;
    virtual GameObjectState* HandleCollisions(PlayerObject* parent, GameObject* obj) override;

private:
    size_t _jumpsLeft;

};

class OnGroundState : public GameObjectState
{
public:
    OnGroundState(float yBound, float xBound, float xWidth);
    //OnGroundState(const OnGroundState& other) = delete;
    //OnGroundState(OnGroundState&& other)      = delete;
    ~OnGroundState(void) = default;
    virtual GameObjectState* HandleUpdate(PlayerObject* parent, const Physics& physics, Dimensions2D boundaries, Timestep dt) override;
    virtual GameObjectState* HandleInput(InputComponent& inputCmp) override;
    virtual GameObjectState* HandleCollisions(PlayerObject* parent, GameObject* obj) override;

private:
    float _yBound; // The "height" of the ground under the player.
    float _xBound;
    float _xWidth;

};


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
    InputComponent(const Input& input);
    ~InputComponent(void) = default;

    void SetParent(GameObject* parent);
    /// @return true if space was pressed, false otherwise (TODO: Refactor to take a list of keys to report)
    bool Handle(const std::vector<Input::KeyCode>& keys);

    // TODO: Implement methods to bind commands to keys

private:
    const Input&             _input;
    GameObject*              _parent;
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
    void Draw(const Renderer& renderer, const Camera& camera, Timestep it) const;

private:
    const GameObject* _parent;

};


/// Base class for all entities in the game.
class GameObject : public DrawableObject
{
public:
    static std::unique_ptr<PlayerObject> CreatePlayer(Input& input, float posX, float posY, float moveSpeed, float radius, Sound& jumpSound, Color color);
    static std::unique_ptr<GameObject>   CreateBox(Input& input, float moveSpeed, Point2DF position, Dimensions2DF size);

public:
    GameObject(Input& input, float posX, float posY, float moveSpeed, Color color);
    GameObject(const GameObject& other) = delete;
    GameObject(GameObject&& other)      = delete;
    virtual ~GameObject(void);

    bool  IsAlive(void)                 const;
    const glm::vec4& GetPosition(void)  const;
    const glm::vec4& GetVelocity(void)  const;
    const Transform& GetTransform(void) const;

    Transform& GetMutableTransform(void);

    void SetYVelocityStopped(void);
    void SetPosition(float xPos, float yPos);

    void  SetColor(Color color);
    Color GetColor(void) const;

    virtual void HandleInput(void) = 0;

    /// Update the status of the object.
    /// @param physics Physics engine to use.
    /// @param boundaries 2D rectangle specifying the min/max boundaries for the position one the XY-plane.
    /// @param dt The deltatime length for the update.
    virtual void Update(const Physics& physics, Dimensions2D boundaries, Timestep dt) = 0;

    virtual RectangleF GetCollissionRect(void) const = 0;

    void ApplyForce(Physics::Direction direction, float force);
    void ApplyForce(float angleDegrees, float force);

    /// Draws the object on the renderers current target buffer
    /// @param renderer Renderer to use.
    /// @param it Interpolation timestep for correcting position between updates.
    virtual void Draw(const Renderer& renderer, const Camera& camera, Timestep it) const override;

protected:
    InputComponent    _inputComponent;
    GraphicsComponent _graphicsComponent;
    Transform         _transform;
    GameObjectState*  _state;
    Color             _color;

};


class PlayerObject : public GameObject
{
public:
    PlayerObject(Input& input, float posX, float posY, float moveSpeed, float radius, Sound& jumpSound, Color color);
    PlayerObject(const PlayerObject& other) = delete;
    PlayerObject(PlayerObject&& other) = delete;
    ~PlayerObject(void) = default;

    float GetRadius(void) const;
    void  SetRadius(float radius);
    void  UpdateRadius(float factor);

    /// Check if this PlayerObject colission rectangle overlaps with the arguments one.
    /// It there was an overlap, invert y velocity for bouncing effect.
    /// @return true if there was a hit, false otherwise.
    bool CheckHitAndBounce(GameObject* obj);

    void BounceYAxis(void);
    void PlayJumpingSound(void);

    virtual void HandleInput(void) override;

    virtual void Update(const Physics& physics, Dimensions2D boundaries, Timestep dt) override;
    virtual RectangleF GetCollissionRect(void) const override;

private:
    float _radius;
    Sound& _soundJump;

};


class BoxObject : public GameObject
{
public:
    BoxObject(Input& input, Point2DF position, Dimensions2DF size, float moveSpeed, Color color = Constants::Colors::DARK);
    BoxObject(const BoxObject& other) = delete;
    BoxObject(BoxObject&& other) = delete;
    ~BoxObject(void) = default;

    Dimensions2DF GetSize(void) const;

    virtual void HandleInput(void) override;
    virtual void Update(const Physics& physics, Dimensions2D boundaries, Timestep dt) override;
    virtual RectangleF GetCollissionRect(void) const override;

private:
    Dimensions2DF _size;

};

#endif // GAMEOBJECT_HPP
