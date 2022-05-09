#ifndef GAME_HPP
#define GAME_HPP

#include "Sdl2.hpp"
#include "ResourceManager.hpp"
#include "Geometry.hpp"
#include "GameObject.hpp"
#include "GameLevel.hpp"
#include "Timetools.hpp"
#include "Input.hpp"

#include <memory>

// TODO: Move definition into cmake scripts
//#define LOG_LOOP

// If defined, time and log all IF_LOG_TIME wrapped function calls, otherwise exec them normally
#ifdef LOG_LOOP
    #define IF_LOG_INIT() Timer t(false);                                                   \
                          Logger::Critical("Gameloop timing, all times in miscroseconds:"); \
                          int64_t totalTime = 0; int64_t currentTime = 0
    #define IF_LOG_TIME(cmds, MSG)                            \
        t.Reset();                                            \
        cmds;                                                 \
        currentTime = t.Elapsed<std::chrono::microseconds>(); \
        totalTime += currentTime;                             \
        Logger::Info(MSG " : {}", currentTime);               \
        currentTime = 0
    #define IF_LOG_TOTAL() Logger::Info("Total time: {}!\n", totalTime)
#else
    #define IF_LOG_INIT()          void(0)
    #define IF_LOG_TIME(cmds, MSG) cmds
    #define IF_LOG_TOTAL()         void(0)
#endif


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
    ResourceManager& _resMgr; // TODO: Delete after refactoring(?)
    GameloopTimer    _glt;
    Point2D          _mousePos;
    std::shared_ptr<ObjectMappedInputCallbacks> _callbacks;

    std::unique_ptr<GameObject> _player;
    std::unique_ptr<GameLevel>  _currentLevel;

};

#endif // GAME_HPP
