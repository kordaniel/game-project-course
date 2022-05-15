#include "Constants.hpp"


const std::string  Constants::SCREEN_TITLE = "Game Project";

// ( TODO: Implement bg scaling) // Render size width must be of the same size as the width of the used background image, no scaling implemented
const Dimensions2D Constants::RENDER_SIZE = { 1000, 750 };

namespace Constants::Paths
{

const std::string BASEPATH            = "res/";
const std::string FONTS               = BASEPATH + "fonts/";
const std::string TILESETS            = BASEPATH + "tilesets/";
const std::string IMAGES              = BASEPATH + "images/";
const std::string MUSICS              = BASEPATH + "musics/";
const std::string SOUNDS              = BASEPATH + "sounds/";

} // end namespace Constants::Paths

namespace Constants::Fonts::TTF
{
using Constants::Paths::FONTS;

const std::string INDIEFLOWER         = FONTS + "indieflower.ttf";
const std::string PATRICKHAND         = FONTS + "patrickhand.ttf";
const std::string PERMANENTMARKER     = FONTS + "permanentmarker.ttf";
const std::string RUBIKBUBBLES        = FONTS + "rubikbubbles.ttf";

} // end namespace Constants::Fonts::TTF

namespace Constants::Tilesets
{
using Constants::Paths::TILESETS;

    namespace FPT
    {

    const std::string FPT_PATH = TILESETS + "freetileset/png/";
    const std::string BG       = FPT_PATH + "BG/BG.png";

    } // end namespace Constants::Tilesets::FPT

} // end namespace Constants::Tilesets

namespace Constants::Images
{
using Constants::Paths::IMAGES;

const std::string PIXNIO_DARK_BLUERED = IMAGES + "pixnio/PIXNIO-1781347-5113x3409.jpeg";
const std::string PIXNIO_BLUE         = IMAGES + "pixnio/PIXNIO-225682-2850x1901.jpeg";
const std::string PIXNIO_RED          = IMAGES + "pixnio/PIXNIO-1730356-5198x3465.jpeg";

} // end namespace Constants::Images

namespace Constants::Musics
{
using Constants::Paths::MUSICS;

const std::string BEATS_A             = MUSICS + "beats_a.ogg";
const std::string BEATS_D             = MUSICS + "beats_d.ogg";
const std::string EFFECTS_B           = MUSICS + "effects_b.ogg";
const std::string GROOVY_BOOTY        = MUSICS + "groovy_booty.ogg";
const std::string LIGHT_MUSIC         = MUSICS + "light_music.ogg";
const std::string SPACESHIP           = MUSICS + "spaceship.ogg";

} // end namespace Constants::Musics

namespace Constants::Sounds
{
using Constants::Paths::SOUNDS;

const std::string JUMP                = SOUNDS + "/8bit/03_Jump_v1.ogg";

} // end namespace Constants::Sounds

namespace Constants::Colors
{

const Color RED      = 0xFF00'00FF;
const Color GREEN    = 0x00FF'00FF;
const Color BLUE     = 0x0000'FFFF;
const Color BLACK    = 0x0000'00FF;
const Color WHITE    = 0xFFFF'FFFF;
const Color DARKEST  = 0x1E1F'26FF;
const Color DARK     = 0x2836'55FF;
const Color LIGHT    = 0x4D64'8DFF;
const Color LIGHTEST = 0xD0E1'F9FF;

} // end namespace Constants::Colors
