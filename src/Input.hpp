#ifndef INPUT_HPP
#define INPUT_HPP

#include "Geometry.hpp"

#include <SDL.h>

#include <unordered_map>
#include <vector>
#include <functional>
#include <cstdint>



class Input
{ // TODO: Subclasses that holds the events for key, mouse etc
public:
    using KeyCallback = std::function<void(void)>;

    enum class KeyCode : int32_t {
        // Abstract SDL_Keycodes to internal keycodes used only in this project
        // SDL Keycodes are defined in SDL_keycode.h
        // Documentation: https://wiki.libsdl.org/SDL_Keycode
        RETURN    = SDLK_RETURN,
        BACKSPACE = SDLK_BACKSPACE,
        ESCAPE    = SDLK_ESCAPE,
        TAB       = SDLK_TAB,
        SPACE     = SDLK_SPACE,
        PLUS      = SDLK_PLUS,
        MINUS     = SDLK_MINUS,
        NUM_0     = SDLK_0,
        NUM_1     = SDLK_1,
        NUM_2     = SDLK_2,
        NUM_3     = SDLK_3,
        NUM_4     = SDLK_4,
        NUM_5     = SDLK_5,
        NUM_6     = SDLK_6,
        NUM_7     = SDLK_7,
        NUM_8     = SDLK_8,
        NUM_9     = SDLK_9,
        a         = SDLK_a,
        b         = SDLK_b,
        c         = SDLK_c,
        d         = SDLK_d,
        e         = SDLK_e,
        f         = SDLK_f,
        g         = SDLK_g,
        h         = SDLK_h,
        i         = SDLK_i,
        j         = SDLK_j,
        k         = SDLK_k,
        l         = SDLK_l,
        m         = SDLK_m,
        n         = SDLK_n,
        o         = SDLK_o,
        p         = SDLK_p,
        q         = SDLK_q,
        r         = SDLK_r,
        s         = SDLK_s,
        t         = SDLK_t,
        u         = SDLK_u,
        v         = SDLK_v,
        w         = SDLK_w,
        x         = SDLK_x,
        y         = SDLK_y,
        z         = SDLK_z,
        LEFT      = SDLK_LEFT,
        RIGHT     = SDLK_RIGHT,
        DOWN      = SDLK_DOWN,
        UP        = SDLK_UP
    };

    enum class MouseButton : uint32_t {
        LEFT   = SDL_BUTTON_LEFT,
        MIDDLE = SDL_BUTTON_MIDDLE,
        RIGHT  = SDL_BUTTON_RIGHT
    };

    enum class EventType : uint32_t {
        // Abstract SDL key and QUIT events
        // https://wiki.libsdl.org/SDL_Event
        KEYDOWN = SDL_KEYDOWN,
        KEYUP   = SDL_KEYUP,
        QUIT    = SDL_QUIT
    };

    Input(void);
    Input(const Input& other) = delete; // Copy constructor
    Input(Input&& other)      = delete; // Move constructor

    /// Handle SDL inputevent, keyboard and mouse
    /// @return Last position of mouse
    Point2D HandleEvent(SDL_Event* event);

    void RegisterKeyCallback(Input::KeyCode key, Input::EventType action, const KeyCallback callback);

    bool IsPressed(Input::KeyCode key)        const;
    bool IsPressed(Input::MouseButton button) const;
    Point2D GetMousePos(void) const;

private:
    void setKeyPressed(SDL_Event* e, bool isPressed);
    void callCallables(SDL_Event* e) const;

private:
    SDL_Point _mousePos;
    Uint32    _mouseButtons;   // Bitmask of the current button state
    std::unordered_map<KeyCode, bool>                     _keyStatus;
    std::unordered_map<KeyCode, std::vector<KeyCallback>> _keyPressCallbacks;
    std::unordered_map<KeyCode, std::vector<KeyCallback>> _keyReleaseCallbacks;

};

#endif // INPUT_HPP
