#include "Texture.hpp"
#include "Logger.hpp"

#include <cassert>


Texture::Texture(void)
    : _texture(nullptr)
{
    //
}

Texture::Texture(const Renderer& renderer, SDL_Surface* surface)
    : _texture(nullptr)
{
    CreateTexture(renderer, surface);
}

Texture::Texture(Texture&& other) noexcept
    : _texture(other._texture)
{
    other._texture = nullptr;
}

Texture::~Texture(void)
{
    destroyTexture();
}

void
Texture::CreateTexture(const Renderer& renderer, SDL_Surface* surface)
{
    destroyTexture();
    //assert(surface->w <= renderer.GetMaxTextureSize().W);
    //assert(surface->h <= renderer.GetMaxTextureSize().H);
    _texture = SDL_CreateTextureFromSurface(renderer.GetSdlRenderer(), surface);
    if (_texture == nullptr) {
        Logger::Critical("Unable to create texture from surface: {}", SDL_GetError());
    }
}

void
Texture::CreateTexture(const Renderer& renderer, Dimensions2D size, Uint32 format, int access)
{
    destroyTexture();
    _texture = SDL_CreateTexture(renderer.GetSdlRenderer(), format, access, size.W, size.H);
    if (_texture == nullptr) {
        Logger::Critical("Unable to create texture: {}", SDL_GetError());
    }
}

void
Texture::CreaateTextureTiled(const Renderer& renderer, Dimensions2D size, SDL_Surface* surface)
{
    // TODO: Refactor api to take args for repeat count in both x/y directions
    //       Implement this, use surface size instead of argument size.
    //       return size of created texture(?)
    CreateTexture(renderer, size);
    renderer.SetRenderTarget(GetTexture());
    SDL_Texture* surfTex = SDL_CreateTextureFromSurface(renderer.GetSdlRenderer(), surface);
    if (surfTex == nullptr) {
        Logger::Critical("Unable to create tiled texture{}: ", SDL_GetError());
        destroyTexture();
        return;
    }
    SDL_Rect surfRect = { 0, 0, surface->w, surface->h };
    renderer.RenderCopy(surfTex, nullptr, &surfRect);
    surfRect.x = surfRect.w;
    renderer.RenderCopy(surfTex, nullptr, &surfRect);
    renderer.RenderPresent(false);
    renderer.SetRenderTarget(nullptr);
    SDL_DestroyTexture(surfTex);
    surfTex = nullptr;
}

void
Texture::SetColorModulation(Color color) const
{
    if (SDL_SetTextureColorMod(_texture, color.r, color.g, color.b) != 0) {
        Logger::Critical("Unable to set color modulation for texture: {}", SDL_GetError());
    }
}

void
Texture::SetAlphaModulation(uint8_t alpha) const
{
    if (SDL_SetTextureAlphaMod(_texture, alpha) != 0) {
        Logger::Critical("Unable to set alpha modulation for texture: {}:", SDL_GetError());
    }
}

void
Texture::SetBlendMode(Renderer::BlendMode blendMode) const
{
    if (SDL_SetTextureBlendMode(_texture, static_cast<SDL_BlendMode>(blendMode)) != 0) {
        Logger::Critical("Unable to set blendmode for texture: {}", SDL_GetError());
    }
}

SDL_Texture*
Texture::GetTexture(void) const
{
    return _texture;
}

Dimensions2D
Texture::GetSize(void) const
{
    Dimensions2D size;
    if (SDL_QueryTexture(_texture, nullptr, nullptr, &size.W, &size.H) != 0) {
        Logger::Critical("Unable to query the the size for texture: {}", SDL_GetError());
        size = { -1, -1 };
    }
    return size;
}

void
Texture::Render(const Renderer& renderer, const SDL_Rect* srcrect, const SDL_Rect* dstrect) const
{
    assert(_texture != nullptr); // Need to call CreateTexture before rendering
    renderer.RenderCopy(_texture, srcrect, dstrect);
}

// Private functions
void
Texture::destroyTexture(void)
{
    if (_texture == nullptr) {
        return;
    }
    SDL_DestroyTexture(_texture);
    _texture = nullptr;
}
