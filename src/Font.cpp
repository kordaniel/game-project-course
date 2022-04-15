#include "Font.hpp"
#include "Logger.hpp"


Font::Font(std::string filepath, int size)
    : _filepath(filepath)
    , _size(size)
    , _font(TTF_OpenFont(_filepath.c_str(), _size))
{
    if (_font == nullptr) {
        Logger::Critical("Unable to load font \"{}\": {}", _filepath, TTF_GetError());
    } else {
        Logger::Debug("Font \"{}\" with size {} loaded.", _filepath, _size);
    }
}

Font::~Font(void)
{
    TTF_CloseFont(_font);
    _font = nullptr;
    Logger::Debug("Font \"{}\" unloaded.", _filepath);
}

int
Font::GetSize(void) const { return _size; }

TTF_Font*
Font::GetFont(void) const { return _font; }
