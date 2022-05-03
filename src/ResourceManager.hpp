#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include "Helpers.hpp"
#include "Font.hpp"
#include "Image.hpp"
#include "Music.hpp"
#include "Sound.hpp"

#include <string>
#include <string_view>
#include <unordered_map>


/// Class for handling resources. Only one object should ever be created of this class.
/// When requesting a resource for the first time this class will load it from disk
/// and store it for future references.
class ResourceManager
{
public:
    ResourceManager(void);
    ResourceManager(const ResourceManager& other) = delete; // Copy constructor
    ResourceManager(ResourceManager&& other)      = delete; // Move constructor

    Font&  GetFont(std::string_view filepath, int size);
    Image& GetImage(std::string_view filepath);
    Music& GetMusic(std::string_view filepath);
    Sound& GetSound(std::string_view filepath);

private:
    Font&  loadFont(const std::pair<std::string, int>& key);
    Image& loadImage(std::string& filepath);
    Music& loadMusic(std::string& filepath);
    Sound& loadSound(std::string& filepath);

private:
    inline static bool s_isInstantiated = false;

    // Hash function for str&int pair is defined in Helpers.hpp
    std::unordered_map<std::pair<std::string, int>, Font> _fonts;
    std::unordered_map<std::string, Image>                _images;
    std::unordered_map<std::string, Music>                _musics;
    std::unordered_map<std::string, Sound>                _sounds;

};

#endif // RESOURCEMANAGER_HPP
