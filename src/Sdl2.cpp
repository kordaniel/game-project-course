#include "Sdl2.hpp"
#include "Logger.hpp"

#include <cstdlib>     // std::atexit
#include <cstdint>
#include <type_traits> // std::is_same
#include <cassert>


bool
Sdl2::Initialize(void)
{ // Static function
#ifndef NDEBUG
    Logger::Debug("Initializing SDL2 Subsystem!");
#endif

    // Do static assertions that the SDL Typedefs are compatible with the types provided
    // by the current platform that we use internally, outside the SDL-functions.
    static_assert(
        std::is_same<SDL_Keycode, int32_t>(),
        "Type of enum Input::Keycode is not compatible with the type of SDL_Keycode."
        " SDL_Keycode is a typedef to Sint32, which is a signed 32b integer."
    );
    static_assert(
        std::is_same<std::underlying_type_t<SDL_EventType>, uint32_t>(),
        "Type of enum Input::EventType is not compatible with the type of SDL_EventType."
        " SDL_EventType is Uint32, an unsigned 32b integer."
    );

    static_assert(
        std::is_same<Uint8, uint8_t>(),
        "Mismatching types between SDL and cstdint defined types"
    );
    static_assert(
        std::is_same<Uint16, uint16_t>(),
        "Mismatching types between SDL and cstdint defined types"
    );
    static_assert(
        std::is_same<Uint32, uint32_t>(),
        "Mismatching types between SDL and cstdint defined types"
    );
    static_assert(
        std::is_same<Uint64, uint64_t>(),
        "Mismatching types between SDL and cstdint defined types"
    );
    static_assert(
        std::is_same<Sint8, int8_t>(),
        "Mismatching types between SDL and cstdint defined types"
    );
    static_assert(
        std::is_same<Sint16, int16_t>(),
        "Mismatching types between SDL and cstdint defined types"
    );
    static_assert(
        std::is_same<Sint32, int32_t>(),
        "Mismatching types between SDL and cstdint defined types"
    );
    static_assert(
        std::is_same<Sint64, int64_t>(),
        "Mismatching types between SDL and cstdint defined types"
    );

    assert(s_isInitialized == false);
    assert(s_ttfIsInitialized == false);
    assert(s_imageIsInitialized == false);
    assert(s_mixerIsInitialized == false);

    // sdlFlags can be set to 0 if we want to initialize the needed subsystems individually
    Uint32 sdlFlags = SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_AUDIO;
    int imgFlags    = IMG_INIT_JPG | IMG_INIT_PNG;  // | IMG_INIT_TIF;
    int mixFlags    = MIX_INIT_FLAC | MIX_INIT_OGG; // | MIX_INIT_MP3;

    s_isInitialized = SDL_Init(sdlFlags) == 0;
    if (!IsInitialized()) {
        Logger::Critical("Unable to initialize SDL2 Subsystem: {}!", SDL_GetError());
        return false;
    }
    Logger::Debug("SDL2 Subsystem initialized!");

    std::atexit(Sdl2::Destruct);

    s_ttfIsInitialized = TTF_Init() == 0;
    if (!TtfIsInitialized()) {
        Logger::Critical("Unable to initialize SDL2 TTF font Subsystem: {}!", TTF_GetError());
        return false;
    }
    Logger::Debug("SDL2 TTF font Subsystem initialized!");

    int inittedImgFlgs = IMG_Init(imgFlags); // Returns 0 on error and a bitmask of the initalized formats on success
    if ( (inittedImgFlgs & imgFlags) != imgFlags)
    {
        Logger::Critical("Unable to initialize SDL2 Image Subsystem: {}", IMG_GetError());
        if ( (inittedImgFlgs & IMG_INIT_JPG) == 0) {
            Logger::Debug("Failed to init JPG");
        }
        if ( (inittedImgFlgs & IMG_INIT_PNG) == 0) {
            Logger::Debug("Failed to init PNG");
        }
        //if ( (inittedFlgs & IMG_INIT_TIF) == 0) {
        //    Logger::Debug("Failed to init TIF");
        //}
        return false;
    }
    s_imageIsInitialized = true;
    Logger::Debug("SDL2 Image Subsystem initialized!");

    int inittedMixFlags = Mix_Init(mixFlags); // Mix_Quit must be called if this succeeds => isInitialized == true!
    if ( (inittedMixFlags & mixFlags) != mixFlags)
    {
        Logger::Critical("Unable to initialize SDL2 Mixer Subsystem: {}", Mix_GetError());
        if ( (inittedMixFlags & MIX_INIT_FLAC) == 0) {
            Logger::Debug("Failed to init flac");
        }
        if ( (inittedMixFlags & MIX_INIT_OGG) == 0) {
            Logger::Debug("Failed to init ogg");
        }
        //if ( (inittedMixFlags & MIX_INIT_MP3) == 0) {
        //    Logger::Debug("Failed to init mp3");
        //}
        return false;
    }
    s_mixerIsInitialized = true;

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) != 0)
    {
        Logger::Debug("SDL2 Mixer was unable to open audio: {}", Mix_GetError());
        return false;
    }
    Logger::Debug("SDL2 Mixer Subsystem initialized");

    return true;
}

void
Sdl2::Destruct(void)
{ // Static function
    if (MixerIsInitialized())
    {
        s_mixerIsInitialized = false;
        Mix_Quit();
        Logger::Debug("SDL2 Mixer Subsystem destructed");
    }
    else {
        Logger::Debug("Unable to destruct uninitialized SDL2 Mixer Subsystem");
    }

    if (ImageIsInitialized())
    {
        s_imageIsInitialized = false;
        IMG_Quit();
        Logger::Debug("SDL2 Image Subsystem destructed");
    }
    else {
        Logger::Debug("Unable to destruct uninitialized SDL2 Image Subsystem");
    }

    if (TtfIsInitialized())
    {
        s_ttfIsInitialized = false;
        TTF_Quit();
        Logger::Debug("SDL2 TTF font Subsystem destructed");
    }
    else {
        Logger::Debug("Unable to destruct uninitialized SDL2 TTF Subsystem!");
    }

    if (IsInitialized())
    {
        s_isInitialized = false;
        SDL_Quit();
        Logger::Debug("SDL2 Subsystem destructed");
    }
    else {
        Logger::Debug("Unable to destruct uninitialized SDL2 Subsystem!");
    }
}

bool
Sdl2::IsInitialized(void)
{ // Static function
    return s_isInitialized;
}

bool
Sdl2::TtfIsInitialized(void)
{ // Static function
    return s_ttfIsInitialized;
}

bool
Sdl2::ImageIsInitialized(void)
{ // Static function
    return s_imageIsInitialized;
}

bool
Sdl2::MixerIsInitialized(void)
{ // Static function
    return s_mixerIsInitialized;
}
void
Sdl2::LogVersion(void)
{ // Static function
    SDL_version compiled;
    SDL_version linked;
    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    Logger::Debug("Compiled against SDL version: {}.{}.{}", compiled.major, compiled.minor, compiled.patch);
    Logger::Debug("Linked against SDL version:   {}.{}.{}", linked.major, linked.minor, linked.patch);
}

void
Sdl2::LogTtfVersion(void)
{ // Static function
    SDL_version compiled;
    const SDL_version* linked = TTF_Linked_Version();
    TTF_VERSION(&compiled);
    Logger::Debug("Compiled against SDL_ttf version: {}.{}.{}", compiled.major, compiled.minor, compiled.patch);
    Logger::Debug("Linked against SDL_ttf version:   {}.{}.{}", linked->major, linked->minor, linked->patch);
}

void
Sdl2::LogImageVersion(void)
{ // Static function
    SDL_version compiled;
    const SDL_version* linked = IMG_Linked_Version();
    SDL_IMAGE_VERSION(&compiled);
    Logger::Debug("Compiled against SDL_Image version: {}.{}.{}", compiled.major, compiled.minor, compiled.patch);
    Logger::Debug("Linked against SDL_Image version:   {}.{}.{}", linked->major, linked->minor, linked->patch);
}

void
Sdl2::LogMixerVersion(void)
{ // Static function
    SDL_version compiled;
    const SDL_version* linked = Mix_Linked_Version();
    SDL_MIXER_VERSION(&compiled);
    Logger::Debug("Compiled against SDL_Mixer version: {}.{}.{}", compiled.major, compiled.minor, compiled.patch);
    Logger::Debug("Linked against SDL_Mixer version:  {}.{}.{}", linked->major, linked->minor, linked->patch);
}

Sdl2::Sdl2(const std::string& windowTitle, Dimensions2D renderAndWindowSize, ResourceManager& ResourceManager)
    : _input()
    , _window(windowTitle, renderAndWindowSize)
    , _renderer(_window)
    , _mixer(ResourceManager)
    , _quitEventCallback(nullptr)
{
    assert(IsInitialized()); // Sdl2::Initialize();
    _renderer.SetLogicalSize(renderAndWindowSize);
}

Sdl2::~Sdl2(void)
{
    //
}

void
Sdl2::RegisterQuitEventCallback(const EventCallback quitCallback)
{
    _quitEventCallback = quitCallback;
}

Point2D
Sdl2::PollEvents(void)
{
    while (SDL_PollEvent(&_event))
    {
        switch (_event.type)
        {
            case SDL_MOUSEMOTION:     // Fall through
            case SDL_MOUSEBUTTONDOWN: // Fall through
            case SDL_MOUSEBUTTONUP:   // Fall through
            case SDL_KEYDOWN:         // Fall through, handle all input events in case SDL_KEYUP
            case SDL_KEYUP:
                _input.HandleEvent(&_event);
                break;
            case SDL_QUIT:
                if (_quitEventCallback) { _quitEventCallback(); }
#ifndef NDEBUG
                else { Logger::Debug("Quit handler not registered"); }
#endif
                break;
            case SDL_RENDER_DEVICE_RESET:
                Logger::Debug("Event: RENDER_DEVICE_RESET");
                break;
            case SDL_RENDER_TARGETS_RESET:
                Logger::Debug("Event: RENDER_TARGETS_RESET");
                break;
            case SDL_WINDOWEVENT: // Triggered when toggling fullscreen
                switch (_event.window.event)
                {
/*
                    case ::SDL_WINDOWEVENT_SHOWN:
                        Logger::Info("Window shown");
                        break;
                    case ::SDL_WINDOWEVENT_HIDDEN:
                        Logger::Info("Window hidden");
                        break;
                    case ::SDL_WINDOWEVENT_EXPOSED:
                        Logger::Info("Window exposed");
                        break;
                    case ::SDL_WINDOWEVENT_MOVED:
                        Logger::Info("Window moved");
                        break;
*/
                    case ::SDL_WINDOWEVENT_RESIZED: // Docs: This event is always preceeded by ::SDL_WINDOWEVENT_SIZE_CHANGED
                        break;
                    case ::SDL_WINDOWEVENT_SIZE_CHANGED:
                        //_renderer.SetViewport();
                        //_renderer.SetRenderTarget();
                        break;
/*
                    case ::SDL_WINDOWEVENT_MINIMIZED:
                        Logger::Info("Window minimized");
                        break;
                    case ::SDL_WINDOWEVENT_MAXIMIZED:
                        Logger::Info("Window maximized");
                        break;
                    case ::SDL_WINDOWEVENT_RESTORED:
                        Logger::Info("Window restored");
                        break;
                    case ::SDL_WINDOWEVENT_ENTER:
                        Logger::Info("Mouse entered");
                        break;
                    case ::SDL_WINDOWEVENT_CLOSE:
                        Logger::Info("Window closed");
                        break;
*/
                    default:
                        //Logger::Info("Unhandled windowevent occured");
                        break;
                }
                break;
            default:
                //Logger::Debug("Unhandled event occured");
                break;
        }
    }

    return _input.GetMousePos();
}

Input&
Sdl2::GetInput(void)
{
    return _input;
}

const Window&
Sdl2::GetWindow(void) const
{
    return _window;
}

SDL_Window*
Sdl2::GetSdlWindow(void) const
{
    return _window.GetSdlWindow();
}

const Renderer&
Sdl2::GetRenderer(void) const
{
    return _renderer;
}

SDL_Renderer*
Sdl2::GetSdlRenderer(void) const
{
    return _renderer.GetSdlRenderer();
}

Mixer&
Sdl2::GetMixer(void)
{
    return _mixer;
}
