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
Background::Draw(const Renderer& renderer, const Camera& camera, [[maybe_unused]] Timestep it) const
{
    // TODO: Interpolate bg position, is it needed?
    assert(_imageRect.w == camera.GetDimensions().W); // background image must be of same width as the camera viewport width

    SDL_Rect transformed = {
        ((camera.GetX() % _imageRect.w) + _imageRect.w) % _imageRect.w,
        _imageRect.y,
        _imageRect.w,
        _imageRect.h
    };

    _imageTexture.Render(renderer, &transformed, nullptr);
}
