#ifndef LABEL_HPP
#define LABEL_HPP

#include "Font.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Color.hpp"
#include "Geometry.hpp"

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <array>


class Label
{ // TODO: Update only the needed textures, 2 labels when switching active label and especially
  //       background only when window size changes, instead of always updating every texture
private:
    enum class Selection : size_t { NOT_SELECTED = 0, SELECTED = 1 };

public:
    Label(const Font& font, std::string text, Color color, int xPos, int yPos, bool isSelected = false);
    Label(const Label& other) = delete;  // Copy constructor
    Label(Label&& other) noexcept; // Move constructor, destructor is called after move
    ~Label(void) noexcept;

    const SDL_Surface* GetSurface(void) const;
    const SDL_Texture* GetTexture(void) const;

    Point2D GetCoords(void)     const;
    int     GetWidth(void)      const;
    int     GetHeight(void)     const;
    bool    GetIsSelected(void) const;

    /// UpdateTexture MUST be called after this method has been called
    void SetIsSelected(bool selected);
    void UpdateTexture(const Renderer& renderer);

    void Render(const Renderer& renderer, bool ScaleToDstRect) const;

private:
    size_t getSurfaceIndex(Label::Selection selection) const;

private:
    Selection    _activeSurface;
    const Font&  _font;
    std::string  _text;
    std::array<SDL_Surface*, 2> _textSurfaces;
    SDL_Rect     _textRect;
    Texture      _textTexture;

};

#endif // LABEL_HPP
