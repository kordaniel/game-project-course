#include "Label.hpp"
#include "Logger.hpp"

#include <cassert>


// TODO: Create system to distinguish colors between selected and not selected!
Label::Label(const Font& font, std::string text, Color color, Point2D position, bool isSelected)
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
        TTF_RenderText_Solid(_font.GetFont(), _text.c_str(), { color.r/2, color.g/2, color.b/2, color.a } ) // Selected
    })
    , _textRectangles{ {
        { position.X, position.Y, _textSurfaces[0]->w, _textSurfaces[0]->h }, // Not selected
        { position.X, position.Y, _textSurfaces[1]->w, _textSurfaces[1]->h }  // Selected
    } }
    , _textTexture()
{
    for (const auto& surface : _textSurfaces) {
        if (surface == nullptr) {
            Logger::Critical("Error loading texture surface {}", TTF_GetError());
            assert(false);
        }
    }
}

Label::Label(Label&& other) noexcept
    : _activeSurface(other._activeSurface)
    , _font(other._font)
    , _text(std::move(other._text))
    , _textSurfaces(std::move(other._textSurfaces))
    , _textRectangles(std::move(other._textRectangles))
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

    // Logger::Debug("Destructed label: {}", _text);
    // If this logs an label without any str => The destructed object is the source obj when move ctor was called
}

const SDL_Surface*
Label::GetSurface(void) const
{
    return _textSurfaces.at(getSurfaceIndex(_activeSurface));
}

const SDL_Texture*
Label::GetTexture(void) const { return _textTexture.GetTexture(); }

Point2D
Label::GetCoords(void) const {
    const SDL_Rect& activeRect = _textRectangles.at(getSurfaceIndex(_activeSurface));
    return { activeRect.x, activeRect.y };
}

Dimensions2D
Label::GetDimensions(void) const
{
    return {
        _textRectangles.at(getSurfaceIndex(_activeSurface)).w,
        _textRectangles.at(getSurfaceIndex(_activeSurface)).h
    };
}

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
Label::UpdateTexture(const Renderer& renderer, bool setCentered)
{
    SDL_Surface* activeSurface = _textSurfaces.at(getSurfaceIndex(_activeSurface));
    SDL_Rect& activeRectangle = _textRectangles.at(getSurfaceIndex(_activeSurface));

    _textTexture.CreateTexture(renderer, activeSurface);

    if (setCentered) {
        activeRectangle.x = renderer.GetOutputSize().W / 2 - activeRectangle.w / 2;
    } else {
        activeRectangle.x = 0;
    }

    // TODO: Define a global that sets/unsets the blending mode when NDEBUG is not set
    //_textTexture.SetBlendMode(Texture::BlendMode::BLEND);
}

void
Label::Render(const Renderer& renderer, bool ScaleToDstRect) const
{
    const SDL_Rect& activeRectangle = _textRectangles.at(getSurfaceIndex(_activeSurface));
    if (ScaleToDstRect) {
        _textTexture.Render(renderer, true, nullptr);
    } else {
        _textTexture.Render(renderer, false, &activeRectangle);
    }
}

// Private methods
size_t
Label::getSurfaceIndex(Label::Selection selection) const
{
    return static_cast<size_t>(selection);
}
