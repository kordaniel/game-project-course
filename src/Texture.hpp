#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Renderer.hpp"

#include <SDL.h>


class Texture
{
public:
    Texture(void);
    Texture(const Renderer& renderer, SDL_Surface* surface);
    Texture(const Texture& other) = delete; // Copy constructor
    Texture(Texture&& other) noexcept; // Move constructor, destructor is called after move
    ~Texture(void);

    void CreateTexture(const Renderer& renderer, SDL_Surface* surface);
    void CreateTexture(const Renderer& renderer, Uint32 format, int access, int w, int h);

    const SDL_Texture* GetTexture(void) const;

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
