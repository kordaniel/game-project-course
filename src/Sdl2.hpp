#ifndef SDL2_HPP
#define SDL2_HPP

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <cstdint>

#include "Input.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "Mixer.hpp"
#include "Geometry.hpp"
#include "ResourceManager.hpp"

/// This class is an abstraction of the C SDL2 library and it provides a C++ interface for it.
/// NOTE: The static function Sdl2::Initialize() must be called before creating any objects of
///       this class. Sdl2::Destruct will be called automagically by std:atexit if initialization
///       succeeds.
class Sdl2
{
public:
    using EventCallback = std::function<void(void)>;

    /// Initializes the SDL2 subsystem. Returns true if succeeded, false otherwise.
    static bool Initialize(void);
    static void Destruct(void);
    static bool IsInitialized(void);
    static bool TtfIsInitialized(void);
    static bool ImageIsInitialized(void);
    static bool MixerIsInitialized(void);
    static void LogVersion(void);
    static void LogTtfVersion(void);
    static void LogImageVersion(void);
    static void LogMixerVersion(void);

public:
    Sdl2(const std::string& windowTitle, Dimensions2D windowSize, ResourceManager& resourceManager);
    ~Sdl2(void);

    void RegisterQuitEventCallback(const EventCallback quitCallback);

    Point2D PollEvents(void);
    Input&  GetInput(void);

    const Window&   GetWindow(void)      const;
    SDL_Window*     GetSdlWindow(void)   const;
    const Renderer& GetRenderer(void)    const;
    SDL_Renderer*   GetSdlRenderer(void) const;
    Mixer&          GetMixer(void);

private:
    inline static bool s_isInitialized      = false;
    inline static bool s_ttfIsInitialized   = false;
    inline static bool s_imageIsInitialized = false;
    inline static bool s_mixerIsInitialized = false;

    SDL_Event     _event;
    Input         _input;
    Window        _window;
    Renderer      _renderer;
    Mixer         _mixer;
    EventCallback _quitEventCallback;

};

#endif // SDL2_HPP
