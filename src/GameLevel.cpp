#include "GameLevel.hpp"
#include "Logger.hpp"

#include <cassert>


std::unique_ptr<GameLevel>
GameLevel::CreateLevel(Sdl2& sdl2, ResourceManager& resMgr, Dimensions2D arenaSize,
                       const std::string& backgroundFilepath,
                       float gravity, float friction, PlayerObject* player)
{ // Static function
    return std::make_unique<GameLevel>(sdl2, resMgr, arenaSize, backgroundFilepath, gravity, friction, player);
}


GameLevel::GameLevel(Sdl2& sdl2, ResourceManager& resMgr, Dimensions2D arenaSize,
                     const std::string& backgroundFilepath,
                     float gravity, float friction, PlayerObject* player)
    : _sdl2(sdl2)
    , _resMgr(resMgr)
    , _arenaSize(arenaSize)
    , _background(backgroundFilepath)
    , _physics(gravity, friction)
    , _player(player)
    , _camera()
{
    assert(player != nullptr);
    player->SetPosition(2.0f * player->GetRadius(), 2.0f * _player->GetRadius());
    _background.UpdateTexture(_sdl2.GetRenderer());
    _camera.SetCenterPosition(_player->GetPosition());
    _camera.SetDimensions(_arenaSize);
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
    _camera.TrackPosition(_player->GetPosition(), 0.15f);
    _background.SetPosition(_camera);
}

void
GameLevel::Draw(const Renderer& renderer, Timestep it) const
{
    _background.Draw(renderer, true);
    _player->Draw(renderer, _camera, it);
}
