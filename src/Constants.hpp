#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "Color.hpp"

#include <cstdint>
#include <string>

namespace Constants
{
    namespace Paths
    {
        extern const std::string BASEPATH;
        extern const std::string FONTS;
        extern const std::string IMAGES;
    } // end namespace Constants::Paths

    namespace Fonts::TTF
    {
        extern const std::string INDIEFLOWER;
        extern const std::string PATRICKHAND;
        extern const std::string PERMANENTMARKER;
        extern const std::string RUBIKBUBBLES;
    } // end namespace Constants::Fonts::TTF

    namespace Images
    {
        extern const std::string MICRA;
    } // end namespace Constants::Images

    namespace Colors
    { // https://www.canva.com/learn/100-color-combinations/ => 30. Berry Blues
        extern const uint32_t BLACK;
        extern const uint32_t DARKEST;
        extern const uint32_t DARK;
        extern const uint32_t LIGHT;
        extern const uint32_t LIGHTEST;
        extern const uint32_t WHITE;
    } // end namespace Constants::Colors    

} // end namespace Constants

#endif // CONSTANTS_HPP
