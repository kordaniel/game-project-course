#ifndef MUSIC_HPP
#define MUSIC_HPP

#include <SDL_mixer.h>
#include <string>


/// Abstraction of SDL_Mixer Mix_Music objects. These can be WAVE, MOD, MIDI, OGG, MP3, FLAC
/// files if the Mixer is built, configured and initialized with the support for any or all of these.
class Music
{
public:
    Music(std::string filepath);
    Music(const Music& other) = delete;
    Music(Music&& other)      = delete;
    ~Music(void);

    const Mix_Music* GetMixMusic(void) const;

    /// Plays this music object.
    /// @param loopTimes How many times to play the music, a value of -1 plays forever.
    void Play(int loopTimes = -1) const;

    /// Plays this music object, fades in over ms milliseconds of time.
    /// @param loopTimes How many times to play the music, a value of -1 plays forever.
    /// @param ms The time in milliseconds to fade in.
    void PlayFadeIn(int ms, int loopTimes = -1) const;

private:
    const std::string _filepath;
    Mix_Music*        _musicPtr;
};

#endif // MUSIC_HPP
