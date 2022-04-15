#include "Constants.hpp"

namespace Constants::Paths
{

const std::string BASEPATH = "res/";
const std::string FONTS    = BASEPATH + "fonts/";
const std::string IMAGES   = BASEPATH + "images/";

} // end namespace Constants::Paths

namespace Constants::Fonts::TTF
{

const std::string INDIEFLOWER     = Constants::Paths::FONTS + "indieflower.ttf";
const std::string PATRICKHAND     = Constants::Paths::FONTS + "patrickhand.ttf";
const std::string PERMANENTMARKER = Constants::Paths::FONTS + "permanentmarker.ttf";
const std::string RUBIKBUBBLES    = Constants::Paths::FONTS + "rubikbubbles.ttf";

} // end namespace Constants::Fonts::TTF

namespace Constants::Images
{

const std::string MICRA = Constants::Paths::IMAGES + "micra.bmp";

} // end namespace Constants::Images

namespace Constants::Colors
{

const uint32_t BLACK    = 0x0000'00FF;
const uint32_t DARKEST  = 0x1E1F'26FF;
const uint32_t DARK     = 0x2836'55FF;
const uint32_t LIGHT    = 0x4D64'8DFF;
const uint32_t LIGHTEST = 0xD0E1'F9FF;
const uint32_t WHITE    = 0xFFFF'FFFF;

} // end namespace Constants::Colors
