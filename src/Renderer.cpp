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
    // Docs: https://wiki.libsdl.org/SDL_RenderSetLogicalSize
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
        size = { -1, -1 };
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
Renderer::RenderClear(void) const
{
    if (SDL_RenderClear(_renderer) != 0) {
        Logger::Critical("Unable to clear the current rendering target: {}", SDL_GetError());
    }
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
Renderer::SetDrawBlendMode(BlendMode blendMode) const
{ // NOTE: This method has not been tested
    if (SDL_SetRenderDrawBlendMode(_renderer, static_cast<SDL_BlendMode>(blendMode)) != 0) {
        Logger::Debug("Unable to set renderer draw blend mode: {}", SDL_GetError());
    }
}

void
Renderer::RenderCopy(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect) const
{
    if (SDL_RenderCopy(_renderer, texture, srcrect, dstrect) != 0) {
        Logger::Critical("Unable to copy texture to rendering target: {}", SDL_GetError());
    }
}

void
Renderer::RenderCopyEx(SDL_Texture* texture, const SDL_Rect* srcrect,
                       const SDL_Rect* dstrect, const double angle,
                       const SDL_Point* center, const SDL_RendererFlip flip) const
{ // NOTE: This method has not been tested
    if (SDL_RenderCopyEx(_renderer, texture, srcrect, dstrect, angle, center, flip) != 0) {
        Logger::Critical("Unable to copy texture to rendering target: {}", SDL_GetError());
    }
}

void
Renderer::DrawPoint(Point2D position) const
{ // NOTE: This method has not been tested
    if (SDL_RenderDrawPoint(_renderer, position.X, position.Y) != 0) {
        Logger::Debug("Renderer was not able to draw a point to the coords {}x{}: {}",
                      position.X, position.Y, SDL_GetError());
    }
}

void
Renderer::DrawLine(Point2D point1, Point2D point2) const
{ // NOTE: This method has not been tested
    if (SDL_RenderDrawLine(_renderer, point1.X, point1.Y, point2.X, point2.Y) != 0) {
        Logger::Debug("Renderer was not able to draw a line: {}", SDL_GetError());
    }
}

void
Renderer::DrawRectangle(Rectangle rectangle) const
{ // NOTE: This method has not been tested
    DrawRectangle(&rectangle);
}

void
Renderer::DrawRectangle(Rectangle* rectangle) const
{ // NOTE: This method has not been tested
  // TODO: Cast rectangle to SDL_Rect
    SDL_Rect sdlRect = { rectangle->X, rectangle->Y, rectangle->W, rectangle->H };
    if (SDL_RenderDrawRect(_renderer, &sdlRect) != 0) {
        Logger::Debug("Renderer was not able to draw a rectangle: {}", SDL_GetError());
    }
}

void
Renderer::FillRectangle(Rectangle rectangle) const
{ // NOTE: This method has not been tested
    FillRectangle(&rectangle);
}

void
Renderer::FillRectangle(Rectangle* rectangle) const
{ // NOTE: This method has not been tested
  // TODO: Cast rectangle to SDL_Rect
    SDL_Rect sdlRect = { rectangle->X, rectangle->Y, rectangle->W, rectangle->H };
    if (SDL_RenderFillRect(_renderer, &sdlRect) != 0) {
        Logger::Debug("Renderer was not able to fill a rectangle: {}", SDL_GetError());
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
