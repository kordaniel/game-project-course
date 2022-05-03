#ifndef SOUND_HPP
#define SOUND_HPP

#include <SDL_mixer.h>
#include <string>


/// Abstraction of SDL_Mixer Mix_Chunk Sample (Sound) objects. These can be WAVE, AIFF, RIFF, OGG, and VOC
/// files if the Mixer is built, configured and initialized with the support for any or all of these.
class Sound
{
public:
    Sound(std::string filepath);
    Sound(const Sound& other) = delete;
    Sound(Sound&& other)      = delete;
    ~Sound(void);

    /// Set the volume for the sound
    /// @param level [0,1], 0 = mute, 1 = max
    void SetVolume(double level) const;

    const Mix_Chunk* GetChunk(void) const;

    /// Plays this sound object.
    /// @param loopTimes 0 = plays once (no loop), 1 = twice etc. A value of -1 loops forever.
    void Play(int loopTimes = 0);

private:
    const std::string _filepath;
    Mix_Chunk*        _sound;
    int               _soundChannel; // The channel this sound is set to

};

#endif // SOUND_HPP
