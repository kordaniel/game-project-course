#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "Color.hpp"
#include "Geometry.hpp"

#include <cstdint>
#include <string>


namespace Constants
{
    extern const std::string  SCREEN_TITLE;
    extern const Dimensions2D RENDER_SIZE;

    namespace Paths
    {
        extern const std::string BASEPATH;
        extern const std::string FONTS;
        extern const std::string TILESETS;
        extern const std::string IMAGES;
        extern const std::string MUSICS;
        extern const std::string SOUNDS;
    } // end namespace Constants::Paths

    namespace Fonts::TTF
    {
        // The source and license for every font is placed in the resources directory.
        // The actual path is defined in src/CMakeLists.txt under add_custom_command(<paths>) call
        extern const std::string INDIEFLOWER;
        extern const std::string PATRICKHAND;
        extern const std::string PERMANENTMARKER;
        extern const std::string RUBIKBUBBLES;
    } // end namespace Constants::Fonts::TTF

    namespace Tilesets
    {
        namespace FPT // Free platformer Game Tileset
        {
            // Source: https://www.gameart2d.com/free-platformer-game-tileset.html
            // License: https://creativecommons.org/publicdomain/zero/1.0/ (Public domain)
            extern const std::string FPT_PATH;
            extern const std::string BG;
        } // end namespace Constants::Tilesets::FPT

    } // end namespace Constants::Tilesets

    namespace Images
    {
        // Source:  https://pixnio.com/photos/space
        // License: Universe free photos. Free for personal and commercial use.
        // You can freely use any picture from this gallery. You are free to use the
        // photos for personal or commercial use.
        extern const std::string PIXNIO_DARK_BLUERED;
        extern const std::string PIXNIO_BLUE;
        extern const std::string PIXNIO_RED;
    } // end namespace Constants::Images

    namespace Musics
    {
        extern const std::string BEATS_A;
        extern const std::string BEATS_D;
        extern const std::string EFFECTS_B;
        extern const std::string GROOVY_BOOTY;
        extern const std::string LIGHT_MUSIC;
        extern const std::string SPACESHIP;
    } // end namespace Constants::Musics

    namespace Sounds
    {
        extern const std::string JUMP;
    } // end namespace Constants::Sounds

    namespace Colors
    {
        extern const Color RED;
        extern const Color GREEN;
        extern const Color BLUE;
        extern const Color BLACK;
        extern const Color WHITE;

        // https://www.canva.com/learn/100-color-combinations/ => 30. Berry Blues colors:
        extern const Color DARKEST;
        extern const Color DARK;
        extern const Color LIGHT;
        extern const Color LIGHTEST;
    } // end namespace Constants::Colors    

} // end namespace Constants

#endif // CONSTANTS_HPP
