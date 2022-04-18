#include "Image.hpp"
#include "Logger.hpp"

#include <SDL_image.h>


Image::Image(std::string filepath)
    : _filepath(filepath)
    , _imageSurf(IMG_Load(_filepath.c_str()))
    , _imageRect({ 0, 0, _imageSurf-> w, _imageSurf->h })
    , _imageTexture()
{
    if (_imageSurf == nullptr) {
        Logger::Critical("Unable to load image \"{}\": {}", _filepath, IMG_GetError());
    } else {
        Logger::Debug("Image \"{}\" loaded.", _filepath);
    }
}

Image::~Image(void)
{
    SDL_FreeSurface(_imageSurf);
    _imageSurf = nullptr;
    Logger::Debug("Image \"{}\" unloaded.", _filepath);
}

void
Image::UpdateTexture(const Renderer& renderer)
{
    _imageTexture.CreateTexture(renderer, _imageSurf);
}

const SDL_Surface*
Image::GetSurface(void) const { return _imageSurf; }

const SDL_Texture*
Image::GetTexture(void) const { return _imageTexture.GetTexture(); }

void
Image::Render(const Renderer& renderer, bool scaleToDstRect) const
{
    if (scaleToDstRect) {
        _imageTexture.Render(renderer, true, nullptr);
    } else {
        _imageTexture.Render(renderer, false, &_imageRect);
    }
}
