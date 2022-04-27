#include "Helpers.hpp"

namespace std
{

    // TODO: Move implementation of hash<std::pair<std::string, int>>::operator()(..) const
    // into here from header file and also remove inline keyword from the prototype

} // end namespace std

namespace Helpers
{

    std::string&
    ReplaceAll(std::string& strBuf, std::string_view what, std::string_view with)
    {
        if (what.empty()) {
            return strBuf;
        }

        for (size_t pos = 0;
             strBuf.npos != (pos = strBuf.find(what.data(), pos, what.length()));
             pos += with.length())
        {
            strBuf.replace(pos, what.length(), with.data(), with.length());
        }

        return strBuf;
    }

} // end namespace Helpers
