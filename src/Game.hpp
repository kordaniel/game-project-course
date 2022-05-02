#ifndef GAME_HPP
#define GAME_HPP

#include "Sdl2.hpp"
#include "ResourceManager.hpp"
#include "Geometry.hpp"


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
    void handleQuitEvent(void);
    void handleMenu(void);
    void handleGame(void);
    void handlePaused(void);

private:
    Dimensions2D     _arenaSize;
    State            _state;
    Sdl2&            _sdl;
    ResourceManager& _resMgr;
    Point2D          _mousePos;

};

#endif // GAME_HPP
