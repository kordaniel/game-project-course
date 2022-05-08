#include "Menu.hpp"
#include "Geometry.hpp"
#include "Constants.hpp"
#include "Logger.hpp"

#include <cassert>


Menu::Menu(const Font& fontTitle, Color colorTitle, const Font& fontLabels,
           Color colorLabels, Image& background, const std::string& titleText)
    : _fontTitle(fontTitle)
    , _fontLabels(fontLabels)
    , _colorTitle(colorTitle)
    , _colorLabels(colorLabels)
    , _background(background)
    , _title(_fontTitle, titleText, _colorTitle, { 0, 0 }, false)
    , _labels()
    , _labelSelected(0)
    , _callbacks(std::make_shared<ObjectMappedInputCallbacks>())
{
    _callbacks->AddKeyCallback(Input::KeyCode::UP,     std::bind(&Menu::MoveSelection, this, SelectionDirectory::UP));
    _callbacks->AddKeyCallback(Input::KeyCode::DOWN,   std::bind(&Menu::MoveSelection, this, SelectionDirectory::DOWN));
    _callbacks->AddKeyCallback(Input::KeyCode::RETURN, std::bind(&Menu::ActivateSelection, this));
}

void
Menu::AddLabel(const std::string& labelText, const SelectionCallback callback)
{
    Point2D position;
    bool selected;

    if (_labels.empty()) {
        _labelSelected = 0;
        position = { 0, _title.GetDimensions().H };
        selected = true;
    } else {
        const Label& lastLabel = _labels.back().first;
        position = { 0, lastLabel.GetCoords().Y + lastLabel.GetDimensions().H };
        selected = false;
    }

    _labels.emplace_back(
        std::piecewise_construct,
        std::forward_as_tuple(_fontLabels, labelText, _colorLabels, position, selected),
        std::forward_as_tuple(callback)
    );
}

void
Menu::UpdateTextures(const Renderer& renderer)
{
    bool setCentered = true;

    _background.UpdateTexture(renderer);
    _title.UpdateTexture(renderer, setCentered);
    for (auto& label : _labels) {
        label.first.UpdateTexture(renderer, setCentered);
    }
}

void
Menu::MoveSelection(Menu::SelectionDirectory dir)
{
    _labels.at(_labelSelected).first.SetIsSelected(false);

    switch (dir)
    {
        case SelectionDirectory::DOWN:
            _labelSelected = (_labelSelected + 1) % _labels.size();
            break;
        case SelectionDirectory::UP:
            _labelSelected = _labelSelected == 0
                           ? _labels.size() - 1
                           : _labelSelected - 1;
            break;
    }
    _labels.at(_labelSelected).first.SetIsSelected(true);
}

void
Menu::ActivateSelection(void)
{
    const SelectionCallback& callback = _labels.at(_labelSelected).second;
    if (callback) {
        callback();
    }
}

void
Menu::Render(const Renderer& renderer) const
{
    _background.Render(renderer, true);
    _title.Render(renderer, false);

    for (const auto& label : _labels) {
        label.first.Render(renderer, false);
    }
}

void
Menu::ActivateCallbacks(Input& input)
{
    input.UseObjectCallbacks(_callbacks);
}
