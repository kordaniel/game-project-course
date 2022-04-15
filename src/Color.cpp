#include "Color.hpp"

Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
    : r(red)
    , g(green)
    , b(blue)
    , a(alpha)
{
    //
}

Color::Color(uint32_t color)
    : Color(
        static_cast<uint8_t>(color >> 24),
        static_cast<uint8_t>(color >> 16),
        static_cast<uint8_t>(color >> 8),
        static_cast<uint8_t>(color)
    )
{
    //
}

uint32_t
Color::ToUint32_t(const Color& c)
{ // Static function
    return static_cast<uint32_t>(c.r) << 24 |
           static_cast<uint32_t>(c.g) << 16 |
           static_cast<uint32_t>(c.b) << 8 |
           static_cast<uint32_t>(c.a);
}
