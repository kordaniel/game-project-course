#ifndef GAMELEVEL_HPP
#define GAMELEVEL_HPP

#include "Background.hpp"
#include "Camera.hpp"
#include "GameObject.hpp"
#include "Geometry.hpp"
#include "Overlays.hpp"
#include "Physics.hpp"
#include "Renderer.hpp"
#include "ResourceManager.hpp"
#include "Sdl2.hpp"
#include "Timetools.hpp"

#include <memory>
#include <vector>
#include <string>


class GameLevel
{
public:
    static std::unique_ptr<GameLevel> CreateLevel(Sdl2& sdl2, ResourceManager& resMgr,
                                                  int levelNumber, Dimensions2D arenaSize,
                                                  const std::string& backgroundFilepath,
                                                  float gravity, float friction, double initialTime,
                                                  PlayerObject* player);

public:
    GameLevel(Sdl2& sdl2, ResourceManager& resMgr,
              int levelNumber, Dimensions2D arenaSize,
              const std::string& backgroundFilepath,
              float gravity, float friction, double initialTime,
              PlayerObject* player);
    GameLevel(const GameLevel& other) = delete;
    GameLevel(GameLevel&& other)      = delete;
    ~GameLevel(void) = default;

    Dimensions2DF GetArenaSize(void) const;

    void HandleInput(void);
    void Update(Timestep dt);
    void HandleCollisions(void);
    void Draw(const Renderer& renderer, Timestep it) const;

private:
    void initLevelObjects(void);

private:
    Sdl2&            _sdl2;
    ResourceManager& _resMgr;
    Dimensions2D     _arenaSize;
    Background       _background;
    Physics          _physics;

    PlayerObject*    _player;
    Camera           _camera;

    std::vector<std::unique_ptr<GameObject>> _levelObjects;
    LevelTimer       _timeLeft;
    GameHUD          _gameHUD;

};

#endif // GAMELEVEL_HPP
