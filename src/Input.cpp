#include "Input.hpp"
#include "Logger.hpp"

#include <cassert>


Input::Input(void)
    : _mousePos({ 0, 0 })
    , _mouseButtons(0)
    , _keyStatus()
    , _keyPressCallbacks()
    , _keyReleaseCallbacks()
{
    //
}

Point2D
Input::HandleEvent(SDL_Event* event)
{
    assert(event->type == SDL_MOUSEMOTION ||
           event->type == SDL_MOUSEBUTTONDOWN ||
           event->type == SDL_MOUSEBUTTONUP ||
           event->type == SDL_KEYDOWN ||
           event->type == SDL_KEYUP);
    
    switch (event->type)
    {
        case SDL_KEYDOWN:
            setKeyPressed(event, true);
            callCallables(event);
            if (!_objectCallbacks.expired())
            {
                // weak_ptr::lock() returns shared ptr
                // => increases ref cnt of the shared_ptr
                // => not thread safe. If the obj the callbacks reference to is destroyed while
                // this if-stmt is executed, the shared_ptr will live and all the callbacks will
                // point to garbage.
                _objectCallbacks.lock()->CallKeyCallback(static_cast<KeyCode>(event->key.keysym.sym));
            }
            break;
        case SDL_KEYUP:
            setKeyPressed(event, false);
            callCallables(event);
            break;
        default: // Mouse
            _mouseButtons = SDL_GetMouseState(&_mousePos.x, &_mousePos.y);
            break;
    }

    return GetMousePos();
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
Input::UseObjectCallbacks(std::shared_ptr<ObjectMappedInputCallbacks> callbacks)
{
    _objectCallbacks = callbacks;
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

bool
Input::IsPressed(Input::MouseButton button) const
{
    return (_mouseButtons & static_cast<Uint32>(SDL_BUTTON(static_cast<Uint32>(button)))) != 0;
}

Point2D
Input::GetMousePos(void) const
{
    return { _mousePos.x, _mousePos.y };
}

void
Input::setKeyPressed(SDL_Event* e, bool isPressed)
{
    assert(e->type == SDL_KEYDOWN || e->type == SDL_KEYUP);
    _keyStatus[static_cast<KeyCode>(e->key.keysym.sym)] = isPressed;
}

void
Input::callCallables(SDL_Event* e) const
{
    assert(e->type == SDL_KEYDOWN || e->type == SDL_KEYUP);
    const std::unordered_map<KeyCode, std::vector<KeyCallback>>*
        callbacks = nullptr;

    switch (e->type)
    {
        case SDL_KEYDOWN: callbacks = &_keyPressCallbacks; break;
        case SDL_KEYUP: callbacks = &_keyReleaseCallbacks; break;
        default: return;
    }

    auto it = callbacks->find(static_cast<KeyCode>(e->key.keysym.sym));

    if (it == callbacks->end()) {
        return;
    }

    for (const auto& callable : it->second) {
        callable();
    }
}

ObjectMappedInputCallbacks::ObjectMappedInputCallbacks(void)
{
    //Logger Ctor called
}

ObjectMappedInputCallbacks::~ObjectMappedInputCallbacks(void)
{
    //Logger Dtor called
}

void
ObjectMappedInputCallbacks::AddKeyCallback(Input::KeyCode keyCode, const Callback callback)
{
    _callbacks.insert_or_assign(keyCode, callback);
}

void
ObjectMappedInputCallbacks::CallKeyCallback(Input::KeyCode keyCode) const
{
    auto it = _callbacks.find(keyCode);
    if (it != _callbacks.end()) {
        it->second();
    }
}
