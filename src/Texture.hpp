#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Renderer.hpp"
#include "Geometry.hpp"
#include "Color.hpp"

#include <SDL.h>
#include <type_traits>
#include <cassert>


class Texture
{
public:
    Texture(void);
    Texture(const Renderer& renderer, SDL_Surface* surface);
    Texture(const Texture& other) = delete; // Copy constructor
    Texture(Texture&& other) noexcept; // Move constructor, destructor is called after move
    ~Texture(void);

    /// Creates a texture from surface
    void CreateTexture(const Renderer& renderer, SDL_Surface* surface);

    /// Creates a texture, default parameters allow the texture to be used as a renderer target.
    /// NOTE: "You can set the texture scaling method by setting SDL_HINT_RENDER_SCALE_QUALITY before creating the texture."
    void CreateTexture(const Renderer& renderer, Dimensions2D size,
                       Uint32 format = SDL_PIXELFORMAT_RGBA8888,
                       int access = SDL_TEXTUREACCESS_TARGET);

    /// Creates a horizontally tiled (2 times) texture from the surface.
    void CreaateTextureTiled(const Renderer& renderer, Dimensions2D size, SDL_Surface* surface);

    /// Modulates each of the color values of the texture according to the formula
    /// srcC = srcC * (color / 255) when it is rendered.
    void SetColorModulation(Color color) const;

    /// Modulates the alpha value of the texture according to the formula
    /// srcA = srcA * (alpha / 255) when it is rendered.
    void SetAlphaModulation(uint8_t alpha) const;
    void SetBlendMode(Renderer::BlendMode blendMode) const;

    SDL_Texture*       GetTexture(void) const;
    Dimensions2D       GetSize(void)    const;

    /// Copies the texture to the renderer target.
    /// NOTE: If stretchToRenderingTarget is passed the argument false then you also must
    ///       pass a pointer to dstrect.
    /// @param stretchToRenderingTarget true = stretch the texture to fill the whole rendering target.
    /// @param srcrect The area of the texture to use. nullptr = use the whole texture.
    /// @param dstrect The area of the destination rectangle to copy the texture to. Use default arg dstrect = nullptr to fill the whole target.
    void Render(const Renderer& renderer,
                const SDL_Rect* srcrect = nullptr,
                const SDL_Rect* dstrect = nullptr) const;

private:
    void destroyTexture(void);

private:
    SDL_Texture* _texture;

};

#endif // TEXTURE_HPP
