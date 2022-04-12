#include "Input.hpp"
#include "Logger.hpp"

#include <type_traits> // std::is_same
#include <cassert>


Input::Input(void)
    : Input(nullptr)
{
    //
}

Input::Input(const KeyCallback quitHandler)
    : _event()
    , _keyStatus()
    , _keyPressCallbacks()
    , _keyReleaseCallbacks()
    , _sdlQuitCallback(quitHandler)
{
    //
}

void
Input::PollEvents(void)
{
    while (SDL_PollEvent(&_event))
    {
        switch (_event.type)
        {
            case SDL_KEYDOWN:
                setKeyPressed(true);
                callCallables();
                break;
            case SDL_KEYUP:
                setKeyPressed(false);
                callCallables();
                break;
            //case SDL_MOUSEMOTION:
            //case SDL_MOUSEBUTTONDOWN:
            //case SDL_MOUSEBUTTONUP:
            case SDL_QUIT:
                if (_sdlQuitCallback) {
                    _sdlQuitCallback();
                }
#ifndef NDEBUG
                 else {
                    Logger::Debug("Quit handler not registered..");
                }
#endif
                break;
            default:
                //Logger::Debug("Unhandled event occured!");
                break;
        }
    }
}

void
Input::RegisterKeyCallback(Input::KeyCode key,
                           Input::EventType action, 
                           const KeyCallback callback)
{
    assert(action == EventType::KEYDOWN || action == EventType::KEYUP);
    switch (action)
    {
        case EventType::KEYDOWN:
            _keyPressCallbacks[key].push_back(callback);
            break;
        case EventType::KEYUP:
            _keyReleaseCallbacks[key].push_back(callback);
            break;
        default:
            assert(false);
            break;
    }
}

void
Input::RegisterQuitEventCallback(const KeyCallback callback)
{
    _sdlQuitCallback = callback;
}

bool
Input::IsPressed(Input::KeyCode key) const
{
    auto it = _keyStatus.find(key);
    if (it != _keyStatus.end()) {
        return it->second;
    }
    return false;
}

void
Input::setKeyPressed(bool isPressed)
{
    assert(_event.type == SDL_KEYDOWN || _event.type == SDL_KEYUP);
    _keyStatus[static_cast<KeyCode>(_event.key.keysym.sym)] = isPressed;
}

void
Input::callCallables(void) const
{
    assert(_event.type == SDL_KEYDOWN || _event.type == SDL_KEYUP);
    const std::unordered_map<KeyCode, std::vector<KeyCallback>>*
        callbacks = nullptr;

    switch (_event.type)
    {
        case SDL_KEYDOWN: callbacks = &_keyPressCallbacks; break;
        case SDL_KEYUP: callbacks = &_keyReleaseCallbacks; break;
        default: return;
    }

    auto it = callbacks->find(static_cast<KeyCode>(_event.key.keysym.sym));

    if (it == callbacks->end()) {
        return;
    }

    for (const auto& callable : it->second) {
        callable();
    }
}
