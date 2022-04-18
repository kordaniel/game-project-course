#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdint>


struct Color
{
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
    Color(uint32_t color);
    uint8_t r, g, b, a;

    /// Returns a new color based on arguments passed.
    /// @param c The desired color.
    /// @param alpha The alpha value for the new color.
    /// @return A new color with the same RGB values as the color passed as argument and with alpha set to argument alpha.
    static Color WithAlpha(Color c, uint8_t alpha);

    /// Returns a new color tinted by factor (rgb * factor), alpha is not affected.
    /// @param c The Color to tint.
    /// @param factor The value to multiplicate the color values with.
    /// @return A new color computed from the arguments passed.
    static Color Tinted(Color c, double factor);

    /// Returns a 32bit unsigned integer bitfield representing the color.
    /// @param c The color value.
    /// @return Bitfield representing the color.
    static uint32_t ToUint32_t(Color c);
};

#endif // COLOR_HPP
