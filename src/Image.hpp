#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <string>


class Image
{
public:
    Image(std::string filepath);
    ~Image(void);

    SDL_Surface* GetSurface(void) const;

private:
    const std::string _filepath;
    SDL_Surface*      _imageSurf;

};

#endif // IMAGE_HPP
