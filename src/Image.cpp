#include "Image.hpp"
#include "Logger.hpp"


Image::Image(std::string filepath)
    : _filepath(filepath)
    , _imageSurf(IMG_Load(_filepath.c_str()))
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
    Logger::Debug("Image \"{}\" unloaded.", _filepath);
}

SDL_Surface*
Image::GetSurface(void) const { return _imageSurf; }
