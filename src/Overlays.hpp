#ifndef OVERLAYS_HPP
#define OVERLAYS_HPP

#include "Color.hpp"
#include "Font.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"

#include <SDL_ttf.h>
#include <array>
#include <string>


class GameHUD
{
public:
    enum class TEXTS : size_t { LEVEL = 0, SCORE, SCORE_VAL, TIME, TIME_VAL }; // NOTE: If changing this enum, also change std::array _static_texts template size arg !!!

public:
    GameHUD(const Font& font, const Renderer& renderer, int levelNumber, int minTimeValBaseColor);
    GameHUD(const GameHUD& other) = delete;
    GameHUD(GameHUD&& other)      = delete;
    ~GameHUD(void);

    void Update(int time, int score);
    void Draw(const Renderer& renderer) const;

private:
    void renderText(TEXTS textEnum, const Renderer& renderer, const std::string& text) const;

private:
    const Font&  _FONT;

    int          _score, _time;
    double       _timeValMinBaseColor; // The time value where we start shifting the color towards red

    std::array<Color, 3> _colors;      // Text color, base color of time val, computed color of time val to render

    mutable bool _redrawScoreVal, _redrawTimeVal;
    mutable std::array<Texture*, static_cast<size_t>(TEXTS::TIME_VAL) + 1> _textTextures;
};

#endif // OVERLAYS_HPP
