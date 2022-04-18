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

const SDL_Texture*
Texture::GetTexture(void) const
{
    return _texture;
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
