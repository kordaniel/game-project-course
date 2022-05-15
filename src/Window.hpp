#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Geometry.hpp"

#include <SDL.h>
#include <string>
#include <string_view>


class Window
{
public:
    Window(const std::string& title, Dimensions2D size, bool fullscren = false, bool opengl = false);
    Window(const Window& other) = delete; // Copy constructor
    Window(Window&& other)      = delete; // Move constructor
    ~Window(void);

    SDL_Window*      GetSdlWindow(void)     const;
    std::string_view GetWindowTitle(void)   const;
    void             Update(void)           const;
    void             ToggleFullscreen(void) const;
    bool             IsFullscreen(void)     const;
    Dimensions2D     GetSize(void)          const;
    size_t           GetRefreshrate(void)   const;

private:
    static Uint32 combineWindowFlags(bool fullscreen, bool opengl,
                                     bool borderless = false, bool resizable = true,
                                     bool inputGrabbed = false);
private:
    SDL_Window*  _window;

};

#endif // WINDOW_HPP
