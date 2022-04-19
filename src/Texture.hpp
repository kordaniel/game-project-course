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
    enum class BlendMode {
        NONE  = SDL_BLENDMODE_NONE,  // dstRGBA = srcRGBA
        BLEND = SDL_BLENDMODE_BLEND, // "Alpha blending" (default)
        ADD   = SDL_BLENDMODE_ADD,   // "Additive blending"
        MOD   = SDL_BLENDMODE_MOD    // "Color modulate"
    };

    Texture(void);
    Texture(const Renderer& renderer, SDL_Surface* surface);
    Texture(const Texture& other) = delete; // Copy constructor
    Texture(Texture&& other) noexcept; // Move constructor, destructor is called after move
    ~Texture(void);

    void CreateTexture(const Renderer& renderer, SDL_Surface* surface);
    // TODO: Use Point2D for position
    void CreateTexture(const Renderer& renderer, Uint32 format, int access, int w, int h);

    /// Modulates each of the color values of the texture according to the formula
    /// srcC = srcC * (color / 255) when it is rendered.
    void SetColorModulation(Color color) const;

    /// Modulates the alpha value of the texture according to the formula
    /// srcA = srcA * (alpha / 255) when it is rendered.
    void SetAlphaModulation(uint8_t alpha) const;
    void SetBlendMode(Texture::BlendMode blendMode) const;

    const SDL_Texture* GetTexture(void) const;
    Dimensions2D       GetSize(void)    const;

    /// Copies the texture to the renderer target.
    /// NOTE: If stretchToRenderingTarget is passed the argument false then you also must
    ///       pass a pointer to dstrect.
    /// @param stretchToRenderingTarget true = stretch the texture to fill the whole rendering target.
    /// @param dstrect The coords of the destination rectangle to copy the texture to.
    void Render(const Renderer& renderer, bool stretchToRenderingTarget = false,
                const SDL_Rect* dstrect = nullptr) const;

private:
    void destroyTexture(void);

private:
    SDL_Texture* _texture;

};

#endif // TEXTURE_HPP
