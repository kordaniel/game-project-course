#ifndef GAMELEVEL_HPP
#define GAMELEVEL_HPP

#include "Sdl2.hpp"
#include "Timetools.hpp"
#include "Geometry.hpp"
#include "Physics.hpp"
#include "GameObject.hpp"
#include "Renderer.hpp"

#include <memory>


class GameLevel
{
public:
    static std::unique_ptr<GameLevel> CreateLevel(Sdl2& sdl2, Dimensions2D arenaSize,
                                                  float gravity, float friction,
                                                  GameObject* player);

public:
    GameLevel(Sdl2& sdl2,
              Dimensions2D arenaSize,
              float gravity, float friction,
              GameObject* player);
    GameLevel(const GameLevel& other) = delete;
    GameLevel(GameLevel&& other)      = delete;
    ~GameLevel(void) = default;

    Dimensions2DF GetArenaSize(void) const;

    void HandleInput(void);
    void Update(Timestep dt);
    void Draw(const Renderer& renderer, Timestep it) const;

private:
    Sdl2&            _sdl2;
    Dimensions2D     _arenaSize;
    Physics          _physics;

    GameObject*      _player;
    // entitiesList
};

#endif // GAMELEVEL_HPP
