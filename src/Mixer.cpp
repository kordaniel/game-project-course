#include "Mixer.hpp"
#include "Logger.hpp"


Mixer::Mixer(ResourceManager& resourceManager)
    : _musicMuted(false)
    , _soundsMuted(false)
    , _music(nullptr)
    , _resMgr(resourceManager)
{
    Logger::Debug("Mixer loaded succesfully");
    const int musicDecodersN = Mix_GetNumMusicDecoders();
    const int chunkDecodersN = Mix_GetNumChunkDecoders();
    for (int i = 0; i < musicDecodersN; i++) {
        Logger::Debug("\tSupported music decoder: [{}]", Mix_GetMusicDecoder(i));
    }
    for (int i = 0; i < chunkDecodersN; i++) {
        Logger::Debug("\tSupported chunk decoder: [{}]", Mix_GetChunkDecoder(i));
    }
}

void
Mixer::SetMusic(const std::string& musicFilepath)
{
    _music = &(_resMgr.GetMusic(musicFilepath));
}

void
Mixer::PlayMusic(int loopTimes) const
{
    if (_music == nullptr) {
        return;
    }
    _music->Play(loopTimes);
}

void
Mixer::PlayMusicFadeIn(int ms, int loopTimes) const
{
    if (_music == nullptr) {
        return;
    }
    _music->PlayFadeIn(ms, loopTimes);
}

void
Mixer::PauseMusic(void) const
{
    if (GetMusicIsPlaying()) {
        Mix_PauseMusic();
    }
}

void
Mixer::ResumeMusic(void) const
{
    // NOTE: Docs: This is safe to use on halted, paused, and already playing music.
    Mix_ResumeMusic();
}

void
Mixer::HaltMusic(void) const
{
    Mix_HaltMusic(); // Docs: Always returns zero.
}

void
Mixer::SetMusicVolume(double level) const
{
    Mix_VolumeMusic(static_cast<int>(level * MIX_MAX_VOLUME));
}

double
Mixer::GetMusicVolume(void) const
{
    return Mix_VolumeMusic(-1) / static_cast<double>(MIX_MAX_VOLUME);
}

bool
Mixer::GetMusicIsPlaying(void) const { return Mix_PlayingMusic() == 1; }

bool
Mixer::GetMusicIsPaused(void) const { return Mix_PausedMusic() == 1; }

bool
Mixer::GetMusicIsFading(void) const
{
    // Mix_FadingMusic never returns an error. Possible enum vals:
    // MIX_NO_FADING, MIX_FADING_OUT, MIX_FADING_IN
    return Mix_FadingMusic() != MIX_NO_FADING;
}
