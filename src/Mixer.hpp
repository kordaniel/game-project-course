#ifndef MIXER_HPP
#define MIXER_HPP

#include "Sound.hpp"
#include "Music.hpp"

#include <SDL_mixer.h>


class Mixer
{
public:
    Mixer(void);
    Mixer(Music& music);
    Mixer(const Mixer& other) = delete;
    Mixer(Mixer&& other)      = delete;
    ~Mixer(void) = default;

    void SetMusic(Music& music);

    /// Plays the current loaded music.
    /// @param loopTimes How many times to play the music. Defaults to -1 which plays forever.
    void PlayMusic(int loopTimes = -1) const;

    /// Plays the current loaded music. Fades it in over ms milliseconds of time.
    /// @param loopTimes How many times to play the music, a value of -1 plays forever.
    /// @param ms The time in milliseconds to fade in.
    void PlayMusicFadeIn(int ms, int loopTimes = -1) const;
    void PauseMusic(void) const;
    void ResumeMusic(void) const;

    /// Halt playback of music. Interrupts music fader effects.
    void HaltMusic(void)   const;

    /// Set the volume level for music.
    /// @param level [0,1], 0 = mute, i = max
    void SetMusicVolume(double level) const;

    /// Get the current volume level for music.
    /// @return The current volume in the range [0,1]
    double GetMusicVolume(void)  const;

    /// Note - Docs: Does not check if the channel has been paused.
    /// @return true if music is playing, false if it is not playing.
    bool GetMusicIsPlaying(void) const;

    /// Note - Docs: "Does not check if the music was been halted after it was paused,
    /// which may seem a little weird."
    /// @return true if music is paused, false if it is not playing.
    bool GetMusicIsPaused(void)  const;

    bool GetMusicIsFading(void)  const;

private:
    bool   _musicMuted;
    bool   _soundsMuted;
    Music* _music;

};

#endif // MIXER_HPP
