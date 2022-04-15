#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdint>


struct Color
{
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
    Color(uint32_t color);
    uint8_t r, g, b, a;
    static uint32_t ToUint32_t(const Color& c);
};

#endif // COLOR_HPP
