#include "Background.hpp"
#include "Logger.hpp"

#include <SDL_image.h>


Background::Background(std::string filepath)
    : _filepath(filepath)
    , _imageSurf(IMG_Load(_filepath.c_str()))
    , _imageRect({ 0, 0, _imageSurf-> w, _imageSurf->h })
    , _imageTexture()
{
    if (_imageSurf == nullptr) {
        Logger::Critical("Unable to load image \"{}\": {}", _filepath, IMG_GetError());
    } else {
        Logger::Debug("Background image \"{}\" loaded.", _filepath);
    }
}

Background::~Background(void)
{
    SDL_FreeSurface(_imageSurf);
    _imageSurf = nullptr;
    Logger::Debug("Background image \"{}\" unloaded.", _filepath);
}

void
Background::UpdateTexture(const Renderer& renderer)
{
    _imageTexture.CreaateTextureTiled(renderer, { 2*_imageRect.w, _imageRect.h }, _imageSurf);
}

const SDL_Surface*
Background::GetSurface(void) const { return _imageSurf; }

const SDL_Texture*
Background::GetTexture(void) const { return _imageTexture.GetTexture(); }

void
Background::Draw(const Renderer& renderer, bool scaleToDstRect, Point2D center) const
{
    int x = center.X;
    x %= _imageRect.w;

    SDL_Rect srcrect = { x, 0, _imageRect.w, _imageRect.h };

    if (scaleToDstRect) {
        _imageTexture.Render(renderer, &srcrect, nullptr);
    } else {
        _imageTexture.Render(renderer, &srcrect, &_imageRect);
    }
}
