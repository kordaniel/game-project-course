#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "Geometry.hpp"
#include "Renderer.hpp"
#include "Timetools.hpp"
#include "Transform.hpp"
#include "Physics.hpp"

#include <memory>


class GameObject;


class InputComponent
{
public:
    InputComponent(void) = default;
    virtual ~InputComponent(void) = default; // TODO: = delete; // and implement derived class
private:
};

class GraphicsComponent
{
public:
    GraphicsComponent(const GameObject* parent);
    GraphicsComponent(const GraphicsComponent& other) = delete;
    GraphicsComponent(GraphicsComponent&& other)      = delete;
    ~GraphicsComponent(void) = default;

    void Draw(const Renderer& renderer, Timestep it) const;

private:
    const GameObject* _parent;

};

class GameObject : public DrawableObject
{
    friend void GraphicsComponent::Draw(const Renderer&, Timestep) const;
public:
    static std::unique_ptr<GameObject> CreatePlayer(float posX, float posY, float radius);

public:
    GameObject(float posX, float posY, float radius);
    GameObject(const GameObject& other) = delete;
    GameObject(GameObject&& other)      = delete;
    ~GameObject(void) = default;

    bool  IsAlive(void)                   const;
    float GetRadius(void)                 const;
    const glm::vec4& GetPosition(void)    const;
    const glm::vec4& GetVelocity(void)    const;

    void SetRadius(float radius);
    void UpdateRadius(float factor);

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
    InputComponent    _input;
    GraphicsComponent _graphics;
    Transform         _transform;

    float             _radius;

};

#endif // GAMEOBJECT_HPP
