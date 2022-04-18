#include "Game.hpp"
#include "Logger.hpp"
#include "Menu.hpp"
#include "Constants.hpp"
#include "Geometry.hpp"

#include <functional>
#include <cassert>


Game::Game(Sdl2& sdl, ResourceManager& resourceManager)
    : _sdl(sdl)
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
    Point2D mousePos; // TODO: Move to member var

    _state = State::MENU;

    while (_state != State::QUIT)
    // Handle user input
    // Update all objects
    // Render
    {
        mousePos = _sdl.PollEvents();
        switch (_state)
        {
            case State::RUNNING: break;
            case State::MENU:
                handleMenu();
                break;
            case State::PAUSED: break;
            case State::QUIT: Logger::Critical("State changed to QUIT"); break;
        } 

    }
}

// PRIVATE methods
void
Game::handleQuitEvent(void)
{
    _state = State::QUIT;
}

void
Game::handleMenu(void)
{
    Input& input             = _sdl.GetInput();
    const Renderer& renderer = _sdl.GetRenderer();
    const Font& fontTitle    = _resMgr.GetFont(Constants::Fonts::TTF::PERMANENTMARKER, 128);
    const Font& fontLabels   = _resMgr.GetFont(Constants::Fonts::TTF::PERMANENTMARKER, 64);

    Point2D mousePos;

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

    mainMenu.AddLabel("New Game", [](){ Logger::Debug("New game!"); });
    mainMenu.AddLabel("Settings", [&activeMenu, &settingsMenu](){
        Logger::Debug("Settings!");
        activeMenu = &settingsMenu;
    });
    mainMenu.AddLabel("Help", [](){ Logger::Debug("Help"); });
    mainMenu.AddLabel("Quit", std::bind(&Game::handleQuitEvent, this));

    settingsMenu.AddLabel("Keys");
    settingsMenu.AddLabel("Gfxs");
    settingsMenu.AddLabel("Back", [&activeMenu, &mainMenu]() {
        Logger::Debug("Back");
        activeMenu = &mainMenu;
    });

    mainMenu.UpdateTextures(renderer);
    settingsMenu.UpdateTextures(renderer);

    while (_state == State::MENU)
    {
        mousePos = _sdl.PollEvents();
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
