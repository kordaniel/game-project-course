#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include "Helpers.hpp"
#include "Font.hpp"
#include "Image.hpp"

#include <string>
#include <string_view>
#include <unordered_map>


class ResourceManager
{
public:
    ResourceManager(void);
    Font&  GetFont(std::string_view filepath, int size);
    Image& GetImage(std::string_view filepath);

private:
    Font&  loadFont(const std::pair<std::string, int>& key);
    Image& loadImage(std::string& filepath);

private:
    inline static bool s_isInstantiated = false;

    // Hash function defined in Helpers.hpp
    std::unordered_map<std::pair<std::string, int>, Font> _fonts;
    std::unordered_map<std::string, Image>                _images;

};

#endif // RESOURCEMANAGER_HPP
