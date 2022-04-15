#include "ResourceManager.hpp"
#include "Logger.hpp"

#include <SDL.h>
#include <SDL_ttf.h>
#include <cassert>


ResourceManager::ResourceManager(void)
    : _fonts()
    , _images()
{
    assert(!s_isInstantiated);
    s_isInstantiated = true;
}

Font&
ResourceManager::GetFont(std::string_view filepath, int size)
{
    const std::pair<std::string, int> key = { std::string(filepath), size };
    auto it = _fonts.find(key);
    if (it != _fonts.end()) {
        return it->second;
    }
    return loadFont(key);
}

Image&
ResourceManager::GetImage(std::string_view filepath)
{
    std::string key(filepath);
    auto it = _images.find(key);
    if (it != _images.end()) {
        return it->second;
    }
    return loadImage(key);
}

Font&
ResourceManager::loadFont(const std::pair<std::string, int>& key)
{
    auto retval = _fonts.try_emplace(key, key.first, key.second);
    assert(retval.second); // A new font was inserted
    Logger::Debug("ResourceManager loaded font {} with size {}", key.first, key.second);
    return retval.first->second;
}

Image&
ResourceManager::loadImage(std::string& filepath)
{
    auto retval = _images.try_emplace(filepath, filepath);
    assert(retval.second);
    Logger::Debug("ResourceManager loaded image {}", filepath);
    return retval.first->second;
}
