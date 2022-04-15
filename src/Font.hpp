#ifndef FONT_HPP
#define FONT_HPP

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class Font
{
public:
    Font(std::string filepath, int size);
    ~Font(void);

    int       GetSize(void) const;
    TTF_Font* GetFont(void) const;

private:
    const std::string _filepath;
    const int         _size;
    TTF_Font*         _font;
};

#endif // FONT_HPP
