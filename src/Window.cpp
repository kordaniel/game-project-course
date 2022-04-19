#include "Window.hpp"
#include "Logger.hpp"

#include <cassert>


// TODO: Use Dimension2D for width&height
Window::Window(const std::string& title, int width, int height, bool fullscreen, bool opengl)
  : _window(SDL_CreateWindow(title.c_str(),
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             width, height, Window::combineWindowFlags(fullscreen, opengl)))
{
    if (_window != nullptr) {
        Logger::Debug("Window \"{}\" created!", title);
    } else {
        Logger::Critical("Failed to create window \"{}\": {}", title, SDL_GetError());
    }
}

Window::~Window(void)
{    
    Logger::Debug("Window \"{}\" destroyed!", GetWindowTitle());
    SDL_DestroyWindow(_window);
    _window = nullptr;
}

SDL_Window*
Window::GetSdlWindow(void) const
{
    assert(_window != nullptr);
    return _window;
}

std::string_view
Window::GetWindowTitle(void) const
{
    return SDL_GetWindowTitle(_window);
}

void
Window::Update(void) const
{
    if (SDL_UpdateWindowSurface(GetSdlWindow()) != 0) {
        Logger::Critical("Error updating window \"{}\": {}", GetWindowTitle(), SDL_GetError());
    }
}

void
Window::ToggleFullscreen(void) const
{
    int result = SDL_SetWindowFullscreen(
        _window,
        (IsFullscreen() ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP)
    );
    if (result != 0) {
        Logger::Critical("Error toggling fullscreen for window \"{}\": {}", GetWindowTitle(), SDL_GetError());
    }
}

bool
Window::IsFullscreen(void) const
{
    return SDL_GetWindowFlags(_window) & SDL_WINDOW_FULLSCREEN_DESKTOP;
}

Uint32
Window::combineWindowFlags(bool fullscreen, bool opengl, bool borderless,
                           bool resizable, bool inputGrabbed)
{ // Static function
    // SDL 2.0.1+ supports the following additional flags
    // SDL_WINDOW_FULLSCREEN, _VULKAN, _METAL, _HIDDEN, _MINIMIZED, _MAXIMIXED, _ALLOW_HIGHDPI

    Uint32 flags = 0;

    if (fullscreen)   flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    if (opengl)       flags |= SDL_WINDOW_OPENGL;
    if (borderless)   flags |= SDL_WINDOW_BORDERLESS;
    if (resizable)    flags |= SDL_WINDOW_RESIZABLE;
    if (inputGrabbed) flags |= SDL_WINDOW_INPUT_GRABBED;

    if (flags & SDL_WINDOW_HIDDEN) {
        return flags;
    }

    return flags | SDL_WINDOW_SHOWN;
}
