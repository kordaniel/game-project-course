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
    _texture = SDL_CreateTextureFromSurface(renderer.GetSdlRenderer(), surface);
    if (_texture == nullptr) {
        Logger::Critical("Unable to create texture from surface: {}", SDL_GetError());
    }
}

void
Texture::CreateTexture(const Renderer& renderer, Uint32 format, int access, int w, int h)
{
    // "You can set the texture scaling method by setting SDL_HINT_RENDER_SCALE_QUALITY before creating the texture."
    destroyTexture();
    _texture = SDL_CreateTexture(renderer.GetSdlRenderer(), format, access, w, h);
    if (_texture == nullptr) {
        Logger::Critical("Unable to create texture: {}", SDL_GetError());
    }
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
Texture::SetBlendMode(Texture::BlendMode blendMode) const
{
    if (SDL_SetTextureBlendMode(_texture, static_cast<SDL_BlendMode>(blendMode)) != 0) {
        Logger::Critical("Unable to set blendmode for texture: {}", SDL_GetError());
    }
}

const SDL_Texture*
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
Texture::Render(const Renderer& renderer, bool stretchToRenderingTarget, const SDL_Rect* dstrect) const
{
    assert(_texture != nullptr); // Need to call CreateTexture before rendering

    if (stretchToRenderingTarget) {
        renderer.RenderCopy(_texture, nullptr, nullptr);
    } else {
        renderer.RenderCopy(_texture, nullptr, dstrect);
    }
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
