#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "Geometry.hpp"
#include "Renderer.hpp"

#include <memory>


class InputComponent
{
public:
    virtual ~InputComponent(void); // TODO: = delete; // and implement derived class
private:
};

class GraphicsComponent
{
public:
    GraphicsComponent(void);
    GraphicsComponent(const GraphicsComponent& other) = delete;
    GraphicsComponent(GraphicsComponent&& other)      = delete;
    ~GraphicsComponent(void) = default;
private:
};

class GameObject
{
public:
    static std::unique_ptr<GameObject> CreatePlayer(void);
    GameObject(void);
    GameObject(const GameObject& other) = delete;
    GameObject(GameObject&& other)      = delete;
    ~GameObject(void) = default;

    bool IsAlive(void) const;
    void Update(void)  const;
    void Render(void)  const;

private:
    class Transform
    {
    public:
        Transform(GameObject& componentOwner);
        Transform(const Transform& other) = delete;
        Transform(Transform&& other)      = delete;
        ~Transform(void) = default;

        double GetX(void)         const;
        double GetY(void)         const;
        Point2D GetPosition(void) const;

        void Update(void);

    private:
        GameObject& _owner;
        double _X, _Y, _V;
    };

private:
    GraphicsComponent               _cmpGfx;
    Transform                       _cmpTrsfrm;
    std::unique_ptr<InputComponent> _cmpInpPtr; // TODO: Remove pointer(?)
    

};

#endif // GAMEOBJECT_HPP
