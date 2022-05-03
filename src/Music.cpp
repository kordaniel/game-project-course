#include "Music.hpp"
#include "Logger.hpp"


Music::Music(std::string filepath)
    : _filepath(filepath)
    , _musicPtr(Mix_LoadMUS(_filepath.c_str()))
{
    if (_musicPtr == nullptr) {
        Logger::Critical("Unable to load music \"{}\": {}", _filepath, Mix_GetError());
    } else {
        Logger::Debug("Music \"{}\" loaded.", _filepath);
    }
}

Music::~Music(void)
{
    Mix_FreeMusic(_musicPtr);
    _musicPtr = nullptr;
    Logger::Debug("Music \"{}\" unloaded.", _filepath);
}

const Mix_Music*
Music::GetMixMusic(void) const { return _musicPtr; }

void
Music::Play(int loopTimes) const
{
    assert(_musicPtr != nullptr);
    if (Mix_PlayMusic(_musicPtr, loopTimes) != 0) {
        Logger::Debug("Unable to play music: {}", Mix_GetError());
    }
}

void
Music::PlayFadeIn(int ms, int loopTimes) const
{
    assert(_musicPtr != nullptr);
    if (Mix_FadeInMusic(_musicPtr, loopTimes, ms) != 0) {
        Logger::Debug("Unable to play music (with fade in of {} ms):", ms, Mix_GetError());
    }
}
