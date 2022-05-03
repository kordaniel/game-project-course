#include "ResourceManager.hpp"
#include "Logger.hpp"

#include <cassert>


ResourceManager::ResourceManager(void)
    : _fonts()
    , _images()
    , _musics()
{
    if (s_isInstantiated) {
        // Assertions are not executed in release builds - sanity check
        Logger::Critical("[ERROR]: Resource manager was re-initialized");
    }
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

Music&
ResourceManager::GetMusic(std::string_view filepath)
{
    std::string key(filepath);
    auto it = _musics.find(key);
    if (it != _musics.end()) {
        return it->second;
    }
    return loadMusic(key);
}

Sound&
ResourceManager::GetSound(std::string_view filepath)
{
    std::string key(filepath);
    auto it = _sounds.find(key);
    if (it != _sounds.end()) {
        return it->second;
    }
    return loadSound(key);
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
    assert(retval.second); // A new image was inserted
    Logger::Debug("ResourceManager loaded image {}", filepath);
    return retval.first->second;
}

Music&
ResourceManager::loadMusic(std::string& filepath)
{
    auto retval = _musics.try_emplace(filepath, filepath);
    assert(retval.second); // A new music was inserted
    Logger::Debug("ResourceManager loaded music {}", filepath);
    return retval.first->second;
}

Sound&
ResourceManager::loadSound(std::string& filepath)
{
    auto retval = _sounds.try_emplace(filepath, filepath);
    assert(retval.second); // A new sound was inserted
    Logger::Debug("ResourceManager loaded sound {}", filepath);
    return retval.first->second;
}
