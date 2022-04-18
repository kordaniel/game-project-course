#include "Label.hpp"
#include "Logger.hpp"

#include <cassert>


// TODO: Use Position class for x/y
Label::Label(const Font& font, std::string text, Color color, int xPos, int yPos, bool isSelected)
    : _activeSurface(isSelected ? Selection::SELECTED : Selection::NOT_SELECTED)
    , _font(font)
    , _text(text)
    , _textSurfaces({
        TTF_RenderText_Solid(
            _font.GetFont(),
            _text.c_str(),
            {
                static_cast<Uint8>(2*color.r),
                static_cast<Uint8>(2*color.g),
                static_cast<Uint8>(2*color.b),
                color.a
            }
        ), // Not selected
        TTF_RenderText_Solid(_font.GetFont(), _text.c_str(), { color.r, color.g, color.b, color.a } ) // Selected
    })
    , _textRect({ xPos, yPos, _textSurfaces[0]->w, _textSurfaces[0]->h }) // TODO: Use the correct surface (also update rectangle after switching surface?)
    , _textTexture()
{
    for (const auto& surface : _textSurfaces) {
        if (surface == nullptr) {
            Logger::Critical("Error loading texture surface {}", TTF_GetError());
            assert(false);
        }
    }
    Logger::Debug("Initialized label: {}, pos: {},{}", _text, _textRect.x, _textRect.y);
}

Label::Label(Label&& other) noexcept
    : _activeSurface(other._activeSurface)
    , _font(other._font)
    , _text(std::move(other._text))
    , _textSurfaces(std::move(other._textSurfaces))
    , _textRect(other._textRect)
    , _textTexture(std::move(other._textTexture))
{
    for (auto& othersurf : other._textSurfaces) {
        othersurf = nullptr;
    }
}

Label::~Label(void) noexcept
{
    for (auto& surface : _textSurfaces) {
        if (surface == nullptr) {
            continue;
        }
        SDL_FreeSurface(surface);
        surface = nullptr;
    }

#ifndef NDEBUG
    for (SDL_Surface* surface : _textSurfaces) {
        assert(surface == nullptr);
    }
#endif

    Logger::Debug("Destructing label: {}, pos: {},{}", _text, _textRect.x, _textRect.y);
}

const SDL_Surface*
Label::GetSurface(void) const
{
    return _textSurfaces.at(getSurfaceIndex(_activeSurface));
}

const SDL_Texture*
Label::GetTexture(void) const { return _textTexture.GetTexture(); }

Point2D
Label::GetCoords(void) const { return { _textRect.x, _textRect.y }; }

int
Label::GetWidth(void) const { return _textRect.w; }

int
Label::GetHeight(void) const { return _textRect.h; }

bool
Label::GetIsSelected(void) const { return _activeSurface == Selection::SELECTED; }

void
Label::SetIsSelected(bool selected)
{ 
    if (selected) {
        _activeSurface = Selection::SELECTED;
    } else {
        _activeSurface = Selection::NOT_SELECTED;
    }
}

void
Label::UpdateTexture(const Renderer& renderer)
{
    SDL_Surface* activeSurface = _textSurfaces.at(getSurfaceIndex(_activeSurface));
    _textTexture.CreateTexture(renderer, activeSurface);
}

void
Label::Render(const Renderer& renderer, bool ScaleToDstRect) const
{
    if (ScaleToDstRect) {
        _textTexture.Render(renderer, true, nullptr);
    } else {
        _textTexture.Render(renderer, false, &_textRect);
    }
}

// Private methods
size_t
Label::getSurfaceIndex(Label::Selection selection) const
{
    return static_cast<size_t>(selection);
}
