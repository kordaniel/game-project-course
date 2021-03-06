#include "Game.hpp"
#include "Logger.hpp"
#include "Menu.hpp"
#include "Constants.hpp"
#include "Mixer.hpp"

#include <functional>
#include <cassert>


Game::Game(Sdl2& sdl, ResourceManager& resourceManager)
    : _targetFPS(60)
    , _targetUPS(120)
    , _maxDt(0.2) // Max amount of deltatime to consume per loop iteration
    , _state(State::QUIT)
    , _sdl(sdl)
    , _resMgr(resourceManager)
    , _glt(_targetFPS, _targetUPS, _maxDt)
    , _callbacks(std::make_shared<ObjectMappedInputCallbacks>())
    , _player(nullptr)
    , _currentLevel(nullptr)
{
    _sdl.RegisterQuitEventCallback(std::bind(&Game::handleQuitEvent, this));

    _sdl.GetInput().RegisterKeyCallback(
        Input::KeyCode::f,
        Input::EventType::KEYDOWN,
        std::bind(&Renderer::ToggleFullscreen, &sdl.GetRenderer())
    );
    _sdl.GetInput().RegisterKeyCallback(
        Input::KeyCode::m,
        Input::EventType::KEYDOWN,
        std::bind(&Mixer::ToggleMusicPaused, &_sdl.GetMixer())
    );

    _callbacks->AddKeyCallback(Input::KeyCode::p,      std::bind(&Game::setGameState, this, State::PAUSED));
    _callbacks->AddKeyCallback(Input::KeyCode::ESCAPE, std::bind(&Game::setGameState, this, State::PAUSED));

    loadMainMenu();
}

Game::~Game(void)
{
    //
}

void
Game::Run(void)
{
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
Game::loadMainMenu(void)
{
    _sdl.GetMixer().SetMusic(Constants::Musics::GROOVY_BOOTY);
    _sdl.GetMixer().SetMusicVolume(1.0);
    _sdl.GetMixer().PlayMusicFadeIn(2000);

    setGameState(State::MENU);
}

void
Game::loadLevel(void)
{
    _sdl.GetMixer().SetMusic(Constants::Musics::BEATS_D);
    _sdl.GetMixer().SetMusicVolume(0.5);
    _sdl.GetMixer().PlayMusicFadeIn(2000);

    int levelNumber = 1;
    Dimensions2D levelDimensions = { 100 * 1280, Constants::RENDER_SIZE.H };
    float gravity  = 100.0f;
    float friction = 0.9f;
    double initialTime = 300.0;

    _player = GameObject::CreatePlayer(
        _sdl.GetInput(),
        75.0f, // xPos
        75.0f, // yPos
        75.0f, // standard speed
        30.0f,  // radius
        _resMgr.GetSound(Constants::Sounds::JUMP),
        Constants::Colors::LIGHT
    );
    _currentLevel = GameLevel::CreateLevel(
        _sdl, _resMgr, levelNumber, levelDimensions, Constants::Tilesets::FPT::BG,
        gravity, friction, initialTime, _player.get()
    );

    setGameState(State::RUNNING);
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

    Input& input               = _sdl.GetInput();
    const Renderer& renderer   = _sdl.GetRenderer();
    const Font& fontTitle      = _resMgr.GetFont(Constants::Fonts::TTF::PERMANENTMARKER, 128);
    const Font& fontLabels     = _resMgr.GetFont(Constants::Fonts::TTF::PERMANENTMARKER, 64);
    const Font& helpFontTitle  = _resMgr.GetFont(Constants::Fonts::TTF::PERMANENTMARKER, 64);
    const Font& helpFontLabels = _resMgr.GetFont(Constants::Fonts::TTF::PERMANENTMARKER, 32);
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

    Menu helpMenu(
        helpFontTitle, Constants::Colors::LIGHT,
        helpFontLabels, Constants::Colors::LIGHTEST,
        _resMgr.GetImage(Constants::Images::PIXNIO_DARK_BLUERED),
        "Stay on top of life.."
    );

    Menu* activeMenu = &mainMenu;

    mainMenu.AddLabel("New Game", std::bind(&Game::loadLevel, this));
    mainMenu.AddLabel("Settings", [&activeMenu, &settingsMenu, &input](){
        activeMenu = &settingsMenu;
        settingsMenu.ActivateCallbacks(input);
    });
    mainMenu.AddLabel("Help", [&activeMenu, &helpMenu, &input](){
        activeMenu = &helpMenu;
        helpMenu.ActivateCallbacks(input);
    });
    mainMenu.AddLabel("Quit", std::bind(&Game::handleQuitEvent, this));
    mainMenu.ActivateCallbacks(input);

    helpMenu.AddLabel("..And make sure to never fall!");
    helpMenu.AddLabel("Inputs:");
    helpMenu.AddLabel("P|ESC => pause");
    helpMenu.AddLabel("C|V => toggle VSYNC");
    helpMenu.AddLabel("M => Mute music");
    helpMenu.AddLabel("SPACE => Jump");
    helpMenu.AddLabel("Arrow keys for moving!");
    helpMenu.AddLabel("Enjoy (Back)!", [&activeMenu, &mainMenu, &input]() {
        activeMenu = &mainMenu;
        mainMenu.ActivateCallbacks(input);
    });

    settingsMenu.AddLabel("Keys (NOT implemented)");
    settingsMenu.AddLabel("Gfxs (NOT implemented)");
    settingsMenu.AddLabel("Back", [&activeMenu, &mainMenu, &input]() {
        activeMenu = &mainMenu;
        mainMenu.ActivateCallbacks(input);
    });

    mainMenu.UpdateTextures(renderer);
    helpMenu.UpdateTextures(renderer);
    settingsMenu.UpdateTextures(renderer);

    while (_state == State::MENU)
    {
        _mousePos = _sdl.PollEvents();
        if (input.IsPressed(Input::KeyCode::DOWN)) {
            //activeMenu->MoveSelection(Menu::SelectionDirectory::DOWN);
            activeMenu->UpdateTextures(renderer);
        }
        else if (input.IsPressed(Input::KeyCode::UP)) {
            //activeMenu->MoveSelection(Menu::SelectionDirectory::UP);
            activeMenu->UpdateTextures(renderer);
        }
        // else if (input.IsPressed(Input::KeyCode::RETURN)) {
        //    activeMenu->ActivateSelection();
        //}

        activeMenu->Render(renderer);
        renderer.RenderPresent(true); // Clears the swapped buffer
    }
}

void
Game::handleGame(void)
{
    assert(_state == State::RUNNING);

    Input& input = _sdl.GetInput();
    input.UseObjectCallbacks(_callbacks);

    static TimeEstimate sleepEst(0.003, 0.003);
    //Sound& sndJump = _resMgr.GetSound(Constants::Sounds::JUMP);

    _glt.ResetFields();
    while (_state == State::RUNNING)
    {
        IF_LOG_INIT();

        _glt.InitIteration();

        _mousePos = _sdl.PollEvents();

        IF_LOG_TIME(_currentLevel->HandleInput(), "Input handling");

        if (input.IsPressed(Input::KeyCode::v)) {
            _sdl.GetRenderer().SetVsync(true);
            Logger::Info("Vsync turned ON. Renderer vsync status: {}", _sdl.GetRenderer().GetIsVsyncced());
        } else if (input.IsPressed(Input::KeyCode::c)) {
            _sdl.GetRenderer().SetVsync(false);
            Logger::Info("Vsync turned OFF. Renderer vsync status: {}", _sdl.GetRenderer().GetIsVsyncced());
        }

        //if (input.IsPressed(Input::KeyCode::w)) {
        //    ball->UpdateRadius(1.1f);
        //} else if (input.IsPressed(Input::KeyCode::s)) {
        //    ball->UpdateRadius(1.0f/1.1f);
        //}

        while (_glt.ShouldDoUpdates()) {
            IF_LOG_TIME(_currentLevel->Update(_glt.GetUpdateDeltaTime()), "Physic updates");
            IF_LOG_TIME(_currentLevel->HandleCollisions(), "Handle collisions");
        }

        IF_LOG_TIME(_currentLevel->Draw(_sdl.GetRenderer(), _glt.GetLag()), "Draw to target");

        IF_LOG_TIME(
            _sdl.GetRenderer().SetRenderDrawColor({ Constants::Colors::BLACK });
            _sdl.GetRenderer().RenderPresent(true), "Rendering trgt" // Clears the back buffer with the current color
        );

        IF_LOG_TIME(thread::PreciseSleep(_glt.GetSleeptime(), sleepEst), "Slept for");
        IF_LOG_TOTAL();
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
    pauseMenu.AddLabel("Restart", std::bind(&Game::loadLevel, this));
    pauseMenu.AddLabel("Quit game", std::bind(&Game::loadMainMenu, this));
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
