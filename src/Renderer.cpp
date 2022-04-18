#include "Renderer.hpp"
#include "Logger.hpp"


Renderer::Renderer(SDL_Window* window, bool vsync, bool accelerated)
    : _renderer(SDL_CreateRenderer(window, -1,
        Renderer::combineRendererFlags(false, accelerated, vsync, false))
    )
{
    if (_renderer == nullptr) {
        Logger::Debug("Unable to create renderer: {}", SDL_GetError());
    }
    //SDL_RenderSetLogicalSize(_renderer, 1280, 720);
}

Renderer::~Renderer(void)
{
    SDL_DestroyRenderer(_renderer);
    _renderer = nullptr;
    Logger::Debug("Renderer destructed along with the associated textures");
}

uint32_t
Renderer::GetFlags(void) const
{
    return static_cast<uint32_t>(getFlags());
}

Dimensions2D
Renderer::GetLogicalSize(void) const
{
    Dimensions2D size;
    SDL_RenderGetLogicalSize(_renderer, &size.W, &size.H);
    return size;
}

Dimensions2D
Renderer::GetOutputSize(void) const
{
    Dimensions2D size;
    if (SDL_GetRendererOutputSize(_renderer, &size.W, &size.H) != 0) {
        Logger::Debug("Unable to get renderer output size: {}", SDL_GetError());
    }
    return size;
}

bool
Renderer::GetIsVsyncced(void) const
{
    return getFlags() & SDL_RENDERER_PRESENTVSYNC;
}

SDL_Renderer*
Renderer::GetSdlRenderer(void) const
{
    return _renderer;
}

void
Renderer::RenderPresent(bool doRenderClear) const
{
    SDL_RenderPresent(_renderer);
    if (doRenderClear) { RenderClear(); }
}

void
Renderer::SetRenderDrawColor(Color c) const
{
    if (SDL_SetRenderDrawColor(_renderer, c.r, c.g, c.b, c.a) != 0) {
        Logger::Debug("Unable to set render drawcolor: {}", SDL_GetError());
    }
}

void
Renderer::SetLogicalSize(Dimensions2D size) const
{
    if (SDL_RenderSetLogicalSize(_renderer, size.W, size.H) != 0) {
        Logger::Debug("Unable to set renderer logical size to {}x{}: {}", size.W, size.H, SDL_GetError());
    }
}

void
Renderer::RenderClear(void) const
{
    if (SDL_RenderClear(_renderer) != 0) {
        Logger::Critical("Unable to clear the current rendering target: {}", SDL_GetError());
    }
}

void
Renderer::RenderCopy(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect) const
{
    if (SDL_RenderCopy(_renderer, texture, srcrect, dstrect) != 0) {
        Logger::Critical("Unable to copy texture to rendering target: {}", SDL_GetError());
    }
}

// Private methods
Uint32
Renderer::getFlags(void) const
{
    SDL_RendererInfo info;

    if (SDL_GetRendererInfo(_renderer, &info) != 0) {
        Logger::Debug("Unable to fill SDL_Rendererinfo struct: {}", SDL_GetError());
        return 0;
    }

    return info.flags;
}

Uint32
Renderer::combineRendererFlags(bool renderSoftware, bool renderAccelerated,
                               bool presentVsync, bool renderTargetTexture)
{ // Static function
    Uint32 flags = 0;
    if (renderSoftware)      flags |= SDL_RENDERER_SOFTWARE;
    if (renderAccelerated)   flags |= SDL_RENDERER_ACCELERATED;
    if (presentVsync)        flags |= SDL_RENDERER_PRESENTVSYNC;
    if (renderTargetTexture) flags |= SDL_RENDERER_TARGETTEXTURE;
    
    return flags;
}
