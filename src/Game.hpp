#ifndef GAME_HPP
#define GAME_HPP

#include "Sdl2.hpp"
#include "ResourceManager.hpp"
#include "Geometry.hpp"
#include "GameObject.hpp"
#include "GameLevel.hpp"
#include "Timetools.hpp"

#include <memory>


class Game
{
public:
    Game(Sdl2& sdl, ResourceManager& resourceManager, int width = 1280, int height = 720);
    Game(const Game& other) = delete; // Copy constructor
    Game(Game&& other)      = delete; // Move constructor
    ~Game(void);

    void Run(void);

private:
    enum class State { QUIT, MENU, RUNNING, PAUSED };

    void setGameState(State state);
    void loadMainMenu(void);
    void loadLevel(void);
    void handleQuitEvent(void);
    void handleMenu(void);
    void handleGame(void);
    void handlePaused(void);

private:
    Dimensions2D     _arenaSize;
    State            _state;
    Sdl2&            _sdl;
    ResourceManager& _resMgr;
    GameloopTimer    _glt;
    Point2D          _mousePos;

    std::unique_ptr<GameObject> _player;
    std::unique_ptr<GameLevel>  _currentLevel;

};

#endif // GAME_HPP
