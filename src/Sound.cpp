#include "Sound.hpp"
#include "Logger.hpp"


Sound::Sound(std::string filepath)
    : _filepath(filepath)
    , _sound(Mix_LoadWAV(_filepath.c_str()))
    , _soundChannel(-2)
{
    if (_sound == nullptr) {
        Logger::Critical("Unable to load sound chunk \"{}\": {}", _filepath, Mix_GetError());
    } else {
        Logger::Debug("Sound chunk \"{}\" loaded.", _filepath);
    }
}

Sound::~Sound(void)
{
    // TODO: Assert chunk is not beeing played before freeing!!!!!
    Mix_FreeChunk(_sound);
    _sound = nullptr;
    Logger::Debug("Sound chunk \"{}\" unloaded.", _filepath);
}

void
Sound::SetVolume(double level) const
{
    Mix_VolumeChunk(_sound, static_cast<int>(level * MIX_MAX_VOLUME));
}

const Mix_Chunk*
Sound::GetChunk(void) const { return _sound; }

void
Sound::Play(int loopTimes)
{
    _soundChannel = Mix_PlayChannel(-1, _sound, loopTimes);
    if (_soundChannel == -1) {
        Logger::Debug("Unable to play sound chunk (consider allocating more channels?): {}", Mix_GetError());
    }
}
