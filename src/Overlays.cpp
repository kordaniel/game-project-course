#include "Overlays.hpp"

#include "Constants.hpp"
#include "Geometry.hpp"
#include "Logger.hpp"

#include <algorithm>


GameHUD::GameHUD(const Font& font, const Renderer& renderer, int levelNumber, int minTimeValBaseColor)
    : _FONT(font)
    , _score(-1)
    , _time(-1)
    , _timeValMinBaseColor(static_cast<double>(minTimeValBaseColor))
    , _colors{
        Constants::Colors::DARK,
        Color::Tinted(Constants::Colors::GREEN, 0.75),
        Constants::Colors::DARK
    }
    , _redrawScoreVal(true)
    , _redrawTimeVal(true)
    , _textTextures{}
{
    renderText(TEXTS::LEVEL,     renderer, "LEVEL " + std::to_string(levelNumber));
    renderText(TEXTS::SCORE,     renderer, "SCORE ");
    //renderText(TEXTS::SCORE_VAL, renderer, std::to_string(_score));
    renderText(TEXTS::TIME,      renderer, "TIME  ");
    //renderText(TEXTS::TIME_VAL,  renderer, std::to_string(_time));
}

GameHUD::~GameHUD(void)
{
    for (Texture* t : _textTextures) {
        if (t == nullptr) {
            continue;
        }
        delete t;
    }
}

void
GameHUD::Update(int time, int score)
{
    if (time != _time)
    {
        _time = time;
        _redrawTimeVal = true;

        if (_time >= 0)
        { // Convert the color of remaining time in a linear fashion from the base color towards red in the range [_timeValMinBaseColor, 0]
            double factor = std::min(1.0, static_cast<double>(_time) / _timeValMinBaseColor);
            _colors[2].r = static_cast<uint8_t>((1.0 - factor) * (Constants::Colors::RED.r - _colors[1].r)) + _colors[1].r;
            _colors[2].g = static_cast<uint8_t>(factor * _colors[1].g);
            _colors[2].b = static_cast<uint8_t>(factor * _colors[1].b);
        }
    }

    if (score != _score)
    {
        _score = score;
        _redrawScoreVal = true;
    }
}

void
GameHUD::Draw(const Renderer& renderer) const
{
    SDL_Rect rect = { 0, 0, 0, 0 };

    auto drawTexture = [&renderer, &rect](Texture* t, Point2D pos)
    {
        if (t == nullptr) {
            return;
        }
        rect.x = pos.X;
        rect.y = pos.Y;
        rect.w = t->GetSize().W;
        rect.h = t->GetSize().H;

        t->Render(renderer, nullptr, &rect);
    };

    if (_redrawScoreVal)
    {
        renderText(TEXTS::SCORE_VAL, renderer, std::to_string(_score));
        _redrawScoreVal = false;
    }
    if (_redrawTimeVal) {
        renderText(TEXTS::TIME_VAL, renderer, std::to_string(_time));
        _redrawTimeVal = false;
    }

    drawTexture(_textTextures[static_cast<size_t>(TEXTS::LEVEL)], { 0, 0 });

    drawTexture(_textTextures[static_cast<size_t>(TEXTS::SCORE)], { 0, rect.h });
    drawTexture(_textTextures[static_cast<size_t>(TEXTS::SCORE_VAL)], { rect.w, rect.y });

    drawTexture(_textTextures[static_cast<size_t>(TEXTS::TIME)], { 0, rect.y + rect.h });
    drawTexture(_textTextures[static_cast<size_t>(TEXTS::TIME_VAL)], { rect.w, rect.y });
}

void
GameHUD::renderText(TEXTS textEnum, const Renderer& renderer, const std::string& text) const
{
    SDL_Surface* surface;
    SDL_Color    color;

    switch (textEnum)
    {
        case TEXTS::TIME_VAL:
            color.r = _colors[2].r;
            color.g = _colors[2].g;
            color.b = _colors[2].b;
            color.a = _colors[2].a;
            break;

        default:
            color.r = _colors[0].r;
            color.g = _colors[0].g;
            color.b = _colors[0].b;
            color.a = _colors[0].a;
            break;
    }

    if ( (surface = TTF_RenderText_Solid(_FONT.GetFont(), text.c_str(), color)) == nullptr) {
        Logger::Debug("TTF_RenderText failed to render HUD text: {}", TTF_GetError());
    }

    if (_textTextures[static_cast<size_t>(textEnum)] == nullptr) {
        _textTextures[static_cast<size_t>(textEnum)] = new Texture(renderer, surface);
    } else {
        _textTextures[static_cast<size_t>(textEnum)]->CreateTexture(renderer, surface);
    }

    SDL_FreeSurface(surface);
}
