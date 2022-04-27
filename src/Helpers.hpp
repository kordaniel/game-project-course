#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <utility> // std::pair
#include <string>
#include <string_view>


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

namespace Helpers
{
    /// Mutates the original string and returns a reference to the same string object. Replaces all occurrences of
    /// what with the argument with.
    /// @param strBuf The string to mutate.
    /// @param what The substring to be replaced.
    /// @param with The substring to replace with.
    std::string& ReplaceAll(std::string& strBuf, std::string_view what, std::string_view with);

} // end namespace Helpers

#endif // HELPERS_HPP
