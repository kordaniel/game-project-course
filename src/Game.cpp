#include "Game.hpp"
#include "Logger.hpp"
#include "Menu.hpp"
#include "Constants.hpp"
#include "Geometry.hpp"

// TEMP (game running state = sleeping)
#include <chrono>
#include <thread>
// TEMP

#include <functional>
#include <cassert>


Game::Game(Sdl2& sdl, ResourceManager& resourceManager)
    : _state(State::QUIT)
    , _sdl(sdl)
    , _resMgr(resourceManager)
{
    sdl.RegisterQuitEventCallback(std::bind(&Game::handleQuitEvent, this));
    sdl.GetInput().RegisterKeyCallback(
        Input::KeyCode::f,
        Input::EventType::KEYDOWN,
        std::bind(&Window::ToggleFullscreen, &sdl.GetWindow())
    );
}

Game::~Game(void)
{
    //
}

void
Game::Run(void)
{
    setGameState(State::MENU);

    while (_state != State::QUIT)
    {
        _mousePos = _sdl.PollEvents();
        switch (_state)
        {
            case State::QUIT:
                assert(false); // should never end up here
                break;
            case State::MENU:
                handleMenu();
                break;
            case State::RUNNING:
                handleGame();
                break;
            case State::PAUSED:
                handlePaused();
                break;
        }
    }
}

// PRIVATE methods

void
Game::setGameState(State state)
{
    _state = state;
}

void
Game::handleQuitEvent(void)
{
    setGameState(State::QUIT);
}

void
Game::handleMenu(void)
{
    assert(_state == State::MENU);

    Input& input             = _sdl.GetInput();
    const Renderer& renderer = _sdl.GetRenderer();
    const Font& fontTitle    = _resMgr.GetFont(Constants::Fonts::TTF::PERMANENTMARKER, 128);
    const Font& fontLabels   = _resMgr.GetFont(Constants::Fonts::TTF::PERMANENTMARKER, 64);

    Menu mainMenu(
        fontTitle, Color::WithAlpha(Constants::Colors::LIGHT, 180),
        fontLabels, Color::WithAlpha(Constants::Colors::LIGHTEST, 150),
        _resMgr.GetImage(Constants::Images::PIXNIO_DARK_BLUERED),
        "Main menu"
    );

    Menu settingsMenu(
        fontTitle, Color::WithAlpha(Constants::Colors::LIGHT, 180),
        fontLabels, Color::WithAlpha(Constants::Colors::LIGHTEST, 150),
        _resMgr.GetImage(Constants::Images::PIXNIO_BLUE),
        "Settings"
    );

    Menu* activeMenu = &mainMenu;

    mainMenu.AddLabel("New Game", std::bind(&Game::setGameState, this, State::RUNNING));
    mainMenu.AddLabel("Settings", [&activeMenu, &settingsMenu](){ activeMenu = &settingsMenu; });
    mainMenu.AddLabel("Help (NOT implemented)", [](){ Logger::Debug("Help"); });
    mainMenu.AddLabel("Quit", std::bind(&Game::handleQuitEvent, this));

    settingsMenu.AddLabel("Keys (NOT implemented)");
    settingsMenu.AddLabel("Gfxs (NOT implemented)");
    settingsMenu.AddLabel("Back", [&activeMenu, &mainMenu]() { activeMenu = &mainMenu; });

    mainMenu.UpdateTextures(renderer);
    settingsMenu.UpdateTextures(renderer);

    while (_state == State::MENU)
    {
        _mousePos = _sdl.PollEvents();
        if (input.IsPressed(Input::KeyCode::DOWN)) {
            activeMenu->MoveSelection(Menu::SelectionDirectory::DOWN);
            activeMenu->UpdateTextures(renderer);
        }
        else if (input.IsPressed(Input::KeyCode::UP)) {
            activeMenu->MoveSelection(Menu::SelectionDirectory::UP);
            activeMenu->UpdateTextures(renderer);
        } else if (input.IsPressed(Input::KeyCode::RETURN)) {
            activeMenu->ActivateSelection();
        }

        activeMenu->Render(renderer);
        renderer.RenderPresent(true); // Clears the swapped buffer
    }
}

void
Game::handleGame(void)
{
    assert(_state == State::RUNNING);

    while (_state == State::RUNNING)
    // Handle user input
    // Update all objects
    // Render
    {
        _mousePos = _sdl.PollEvents();
        if (_sdl.GetInput().IsPressed(Input::KeyCode::p)) {
            setGameState(State::PAUSED);
        }
        if (_sdl.GetInput().IsPressed(Input::KeyCode::q)) {
            setGameState(State::QUIT);
        }
        Logger::Debug("Game ON!!");
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}

void
Game::handlePaused(void)
{
    assert(_state == State::PAUSED);

    Input& input             = _sdl.GetInput();
    const Renderer& renderer = _sdl.GetRenderer();
    const Font& fontTitle    = _resMgr.GetFont(Constants::Fonts::TTF::RUBIKBUBBLES, 128);
    const Font& fontLabels   = _resMgr.GetFont(Constants::Fonts::TTF::RUBIKBUBBLES, 64);

    Menu pauseMenu(
        fontTitle, Color::WithAlpha(Constants::Colors::RED, 180),
        fontLabels, Color::WithAlpha(Constants::Colors::RED, 150),
        _resMgr.GetImage(Constants::Images::PIXNIO_RED),
        "Game Paused !"
    );

    pauseMenu.AddLabel("Continue", std::bind(&Game::setGameState, this, State::RUNNING));
    pauseMenu.AddLabel("Restart (NOT implemented)");
    pauseMenu.AddLabel("Quit game", std::bind(&Game::setGameState, this, State::MENU));
    pauseMenu.UpdateTextures(renderer);

    while (_state == State::PAUSED)
    {
        _mousePos = _sdl.PollEvents();
        if (input.IsPressed(Input::KeyCode::DOWN))
        {
            pauseMenu.MoveSelection(Menu::SelectionDirectory::DOWN);
            pauseMenu.UpdateTextures(renderer);
        }
        else if (input.IsPressed(Input::KeyCode::UP))
        {
            pauseMenu.MoveSelection(Menu::SelectionDirectory::UP);
            pauseMenu.UpdateTextures(renderer);
        }
        else if (input.IsPressed(Input::KeyCode::RETURN)) {
            pauseMenu.ActivateSelection();
        }

        pauseMenu.Render(renderer);
        renderer.RenderPresent(true); // Clears the swapped buffer
    }
}
