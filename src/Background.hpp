#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "Camera.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Timetools.hpp"

#include <glm/vec4.hpp>
#include <SDL.h>
#include <string>


class Background
{
public:
    Background(std::string filepath);
    Background(const Background& other) = delete; // Copy constructor
    Background(Background&& other)      = delete; // Move constructor
    ~Background(void);

    void UpdateTexture(const Renderer& renderer);

    const SDL_Surface* GetSurface(void) const;
    const SDL_Texture* GetTexture(void) const;

    void Draw(const Renderer& renderer, const Camera& camera, Timestep it) const;

private:
    const std::string _filepath;
    SDL_Surface*      _imageSurf;
    SDL_Rect          _imageRect;
    Texture           _imageTexture;

};

#endif // BACKGROUND_HPP
