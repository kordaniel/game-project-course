#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <utility> // std::pair
#include <string>

namespace std
{

    // TODO: Remove inline and refactor implementation into cpp file from header
    template<>
    struct hash<std::pair<std::string, int>>
    {
        inline size_t operator()(const std::pair<std::string, int>& pair) const
        {
            size_t strhash = std::hash<std::string>{}(pair.first);

            if (strhash == static_cast<size_t>(pair.second)) {
                return strhash;
            }

            return strhash ^ static_cast<size_t>(pair.second);
        }
    };

} // end namespace std

#endif // HELPERS_HPP
