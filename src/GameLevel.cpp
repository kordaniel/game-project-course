#include "GameLevel.hpp"
#include "Logger.hpp"
#include "Helpers.hpp"

#include <cassert>


std::unique_ptr<GameLevel>
GameLevel::CreateLevel(Sdl2& sdl2, ResourceManager& resMgr, int levelNumber, Dimensions2D arenaSize,
                       const std::string& backgroundFilepath,
                       float gravity, float friction, double initialTime, PlayerObject* player)
{ // Static function
    return std::make_unique<GameLevel>(sdl2, resMgr, levelNumber, arenaSize, backgroundFilepath, gravity, friction, initialTime, player);
}


GameLevel::GameLevel(Sdl2& sdl2, ResourceManager& resMgr, int levelNumber, Dimensions2D arenaSize,
                     const std::string& backgroundFilepath,
                     float gravity, float friction, double initialTime, PlayerObject* player)
    : _sdl2(sdl2)
    , _resMgr(resMgr)
    , _arenaSize(arenaSize)
    , _background(backgroundFilepath)
    , _physics(gravity, friction)
    , _player(player)
    , _camera()
    , _levelObjects()
    , _timeLeft(initialTime)
    , _gameHUD(
        _resMgr.GetFont(Constants::Fonts::TTF::PERMANENTMARKER, 32),
        _sdl2.GetRenderer(),
        levelNumber, 180
    )
{
    assert(player != nullptr);
    player->SetPosition(2.0f * player->GetRadius(), 2.0f * _player->GetRadius());
    _background.UpdateTexture(_sdl2.GetRenderer());
    _camera.SetCenterPosition(_player->GetPosition());
    _camera.SetDimensions(_sdl2.GetRenderer().GetLogicalSize());

    initLevelObjects();

    Logger::Info("Level {} loaded!", levelNumber);
}

Dimensions2DF
GameLevel::GetArenaSize(void) const
{
    return {
        static_cast<float>(_arenaSize.W),
        static_cast<float>(_arenaSize.H)
    };
}

void
GameLevel::HandleInput(void)
{
    _player->HandleInput();
}

void
GameLevel::Update(Timestep dt)
{
    _player->Update(_physics, _arenaSize, dt);
    _camera.TrackPosition(_player->GetPosition(), 0.1f);

    for (auto& o : _levelObjects) {
        o->Update(_physics, _arenaSize, dt);
    }

    _timeLeft.DeductTime(dt);

    static int score = 0; // TODO: Remove fakescore
    _gameHUD.Update(
        _timeLeft.GetTimeLeft().GetWholeSeconds(),
        (Helpers::random::FloatInRange(0.0f, 1.0f) >= 0.95f ? (score += 10) : score) // TODO: Remove fakescore
    );
}

void
GameLevel::HandleCollisions(void)
{
    for (auto& o : _levelObjects) {
        if (_player->CheckHitAndBounce(o.get())) {
            break;
        }
    }
}

void
GameLevel::Draw(const Renderer& renderer, Timestep it) const
{
    _background.Draw(renderer, _camera, it);

    for (const auto& o : _levelObjects) {
        if (_camera.RectangleIsInViewport(o->GetCollissionRect())) {
            o->Draw(renderer, _camera, it);
        }
    }

    _player->Draw(renderer, _camera, it);

    _gameHUD.Draw(renderer);
}

void
GameLevel::initLevelObjects(void)
{
    // Seed random number generator so we always generate the same level.
    // NOTE: The actual generated level will depend on the implementation of the
    //       random number generator => might not produce the same level on different platforms!
    Helpers::random::Seed(1337);

    constexpr float minWidth   =  60.0f;
    constexpr float maxWidth   = 250.0f;
    constexpr float minHeight  =  30.0f;
    constexpr float maxHeigth  = 400.0f; // The algorithm will always subtract a portion
                                         // of this depending on the width of the block.
    constexpr float minSpacing = 100.0f;
    constexpr float maxSpacing = 400.0f;

    const float levelWidth  = static_cast<float>(_arenaSize.W);
    const float levelHeight = static_cast<float>(_arenaSize.H);

    for (float xPos = 0.0f;
         xPos < levelWidth;
         xPos += Helpers::random::FloatInRange(minSpacing, maxSpacing))
    {
        float blockWidth = Helpers::random::FloatInRange(minWidth, maxWidth);
        assert(minWidth <= blockWidth && blockWidth <= maxWidth);

        float blockHeight = Helpers::random::FloatInRange(minHeight, maxHeigth - (0.5f * blockWidth));

        _levelObjects.push_back(GameObject::CreateBox(
            _sdl2.GetInput(), 0.0f,
            { xPos , levelHeight - (0.5f * blockHeight) },
            { blockWidth, blockHeight }
        ));

        xPos += blockWidth;
    }
}
