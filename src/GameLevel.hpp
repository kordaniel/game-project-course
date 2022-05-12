#ifndef GAMELEVEL_HPP
#define GAMELEVEL_HPP

#include "Sdl2.hpp"
#include "ResourceManager.hpp"
#include "Timetools.hpp"
#include "Geometry.hpp"
#include "Physics.hpp"
#include "GameObject.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"
#include "Background.hpp"

#include <memory>


class GameLevel
{
public:
    static std::unique_ptr<GameLevel> CreateLevel(Sdl2& sdl2, ResourceManager& resMgr,
                                                  Dimensions2D arenaSize,
                                                  const std::string& backgroundFilepath,
                                                  float gravity, float friction,
                                                  PlayerObject* player);

public:
    GameLevel(Sdl2& sdl2, ResourceManager& resMgr,
              Dimensions2D arenaSize,
              const std::string& backgroundFilepath,
              float gravity, float friction,
              PlayerObject* player);
    GameLevel(const GameLevel& other) = delete;
    GameLevel(GameLevel&& other)      = delete;
    ~GameLevel(void) = default;

    Dimensions2DF GetArenaSize(void) const;

    void HandleInput(void);
    void Update(Timestep dt);
    void Draw(const Renderer& renderer, Timestep it) const;

private:
    Sdl2&            _sdl2;
    ResourceManager& _resMgr;
    Dimensions2D     _arenaSize;
    Background       _background;
    Physics          _physics;

    PlayerObject*    _player;
    Camera           _camera;
    // entitiesList
};

#endif // GAMELEVEL_HPP
