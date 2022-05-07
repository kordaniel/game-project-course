#include "Game.hpp"
#include "Logger.hpp"
#include "Menu.hpp"
#include "Constants.hpp"
#include "Timetools.hpp"
#include "GameObject.hpp"

#include <functional>
#include <cassert>


Game::Game(Sdl2& sdl, ResourceManager& resourceManager, int width, int height)
    : _arenaSize{width, height}
    , _state(State::QUIT)
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
    _sdl.GetMixer().SetMusic(_resMgr.GetMusic(Constants::Musics::GROOVY_BOOTY));
    _sdl.GetMixer().SetMusicVolume(1.0);
    _sdl.GetMixer().PlayMusicFadeIn(2000);

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

    _sdl.GetMixer().SetMusic(_resMgr.GetMusic(Constants::Musics::BEATS_D));
    _sdl.GetMixer().SetMusicVolume(0.5);
    _sdl.GetMixer().PlayMusicFadeIn(2000);

    Input& input = _sdl.GetInput();
    const Renderer& renderer = _sdl.GetRenderer();

    size_t gameFPS = 60;
    size_t gameUPS = 120;
    double updateTimeMax = 0.2; // seconds, max time to spend updating gamestate/loop iteration.
    TimeEstimate sleepEst(0.003, 0.003);

    std::unique_ptr<GameObject> ball = GameObject::CreatePlayer(
        _sdl.GetInput(),
        0.50f * static_cast<float>(_arenaSize.W),   // xPos
        0.25f * static_cast<float>(_arenaSize.H),   // yPos
        75.0f,  // standard speed
        75.0f   // radius
    );
    //Sound& sndJump = _resMgr.GetSound(Constants::Sounds::JUMP);
    Physics physics(100.0f, 0.9f);

    GameloopTimer glt(gameFPS, gameUPS, updateTimeMax);

    while (_state == State::RUNNING)
    {
        glt.InitIteration();
        _mousePos = _sdl.PollEvents();
        if (input.IsPressed(Input::KeyCode::p) || input.IsPressed(Input::KeyCode::ESCAPE)) {
            setGameState(State::PAUSED);
        }
        ball->HandleInput();

        if (input. IsPressed(Input::KeyCode::m)) {
            Logger::Info("Music paused");
            _sdl.GetMixer().PauseMusic();
        } else if (input.IsPressed(Input::KeyCode::n)) {
            Logger::Info("Music resumed");
            _sdl.GetMixer().ResumeMusic();
        }

        if (input.IsPressed(Input::KeyCode::v)) {
            renderer.SetVsync(true);
            Logger::Info("Vsync turned ON. Renderer vsync status: {}", renderer.GetIsVsyncced());
        } else if (input.IsPressed(Input::KeyCode::c)) {
            renderer.SetVsync(false);
            Logger::Info("Vsync turned OFF. Renderer vsync status: {}", renderer.GetIsVsyncced());
        }

        if (input.IsPressed(Input::KeyCode::w)) {
            ball->UpdateRadius(1.1f);
        } else if (input.IsPressed(Input::KeyCode::s)) {
            ball->UpdateRadius(1.0f/1.1f);
        }

        while (glt.ShouldDoUpdates())
        {
            ball->Update(physics, _arenaSize, glt.GetUpdateDeltaTime());
        }

        // With UPS = 120, lag grows from ~0 -> ~0.007, resets to ~0 and starts growing again
        // Logger::Critical("LAG: {}", static_cast<float>(glt.GetLag()));
        ball->Draw(renderer, glt.GetLag());

        renderer.SetRenderDrawColor({ Constants::Colors::LIGHT });
        renderer.RenderPresent(true); // arg == true => clears the swapped buffer

        Timestep sleepTime = glt.GetSleeptime();
        if (sleepTime.IsNonPositive()) {
            Logger::Debug("Negative sleep time"); // Should happen only one time, at the first loop iteration.
            continue;
        }
        thread::PreciseSleep(sleepTime, sleepEst);
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
