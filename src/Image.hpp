#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "Renderer.hpp"
#include "Texture.hpp"

#include <SDL.h>
#include <string>


class Image
{
public:
    Image(std::string filepath);
    Image(const Image& other) = delete; // Copy constructor
    Image(Image&& other)      = delete; // Move constructor
    ~Image(void);

    void UpdateTexture(const Renderer& renderer);

    const SDL_Surface* GetSurface(void) const;
    const SDL_Texture* GetTexture(void) const;

    void Render(const Renderer& renderer, bool scaleToDstRect) const;

private:
    const std::string _filepath;
    SDL_Surface*      _imageSurf;
    SDL_Rect          _imageRect;
    Texture           _imageTexture;

};

#endif // IMAGE_HPP
