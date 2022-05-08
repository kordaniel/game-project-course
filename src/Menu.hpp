#ifndef MENU_HPP
#define MENU_HPP

#include "Font.hpp"
#include "Image.hpp"
#include "Label.hpp"
#include "Renderer.hpp"
#include "Input.hpp"

#include <SDL.h>

#include <vector>
#include <utility>
#include <functional>
#include <string>
#include <memory>


class Menu
{ // TODO: Move menu labels into the centre of the X-axis
public:
    using SelectionCallback = std::function<void(void)>;

    enum class SelectionDirectory { DOWN, UP };

    Menu(const Font& fontTitle, Color colorTitle, const Font& fontLabels,
         Color colorLabel, Image& background, const std::string& titleText);
    Menu(const Menu& other) = delete; // Copy constructor
    Menu(Menu&& other)      = delete; // Move constructor
    ~Menu(void) = default;

    void AddLabel(const std::string& labelText, const SelectionCallback callback = nullptr);
    void UpdateTextures(const Renderer& renderer);
    void MoveSelection(Menu::SelectionDirectory dir);
    void ActivateSelection(void);

    void Render(const Renderer& renderer) const;

    void ActivateCallbacks(Input& input);

private:
    const Font&                                      _fontTitle;
    const Font&                                      _fontLabels;
    Color                                            _colorTitle;
    Color                                            _colorLabels;
    Image&                                           _background;
    Label                                            _title;
    std::vector<std::pair<Label, SelectionCallback>> _labels;
    size_t                                           _labelSelected;
    std::shared_ptr<ObjectMappedInputCallbacks>      _callbacks;

};

#endif // MENU_HPP
