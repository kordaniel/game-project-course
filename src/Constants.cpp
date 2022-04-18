#include "Constants.hpp"

namespace Constants::Paths
{

const std::string BASEPATH            = "res/";
const std::string FONTS               = BASEPATH + "fonts/";
const std::string IMAGES              = BASEPATH + "images/";

} // end namespace Constants::Paths

namespace Constants::Fonts::TTF
{
using Constants::Paths::FONTS;

const std::string INDIEFLOWER         = FONTS + "indieflower.ttf";
const std::string PATRICKHAND         = FONTS + "patrickhand.ttf";
const std::string PERMANENTMARKER     = FONTS + "permanentmarker.ttf";
const std::string RUBIKBUBBLES        = FONTS + "rubikbubbles.ttf";

} // end namespace Constants::Fonts::TTF

namespace Constants::Images
{
using Constants::Paths::IMAGES;

const std::string PIXNIO_DARK_BLUERED = IMAGES + "pixnio/PIXNIO-1781347-5113x3409.jpeg";
const std::string PIXNIO_BLUE         = IMAGES + "pixnio/PIXNIO-225682-2850x1901.jpeg";
const std::string PIXNIO_RED          = IMAGES + "pixnio/PIXNIO-1730356-5198x3465.jpeg";

} // end namespace Constants::Images

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
