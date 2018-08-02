/* ==========================================================================
   >File: ColorUtilities.h
   >Date: 20180713
   >Author: Vik Pandher
   >Details: Here's the color struct I use and some color defines.
   ========================================================================== */

#pragma once
#include "SizedIntegers.h"
#include "MathUtilities.h"



// RGB values are always paired, this struct should make function parameters
// neater.
struct Color
{
    Color(uint32_t v);
    Color() : r(255), g(255), b(255) {}
    Color(uint8_t _r, uint8_t _g, uint8_t _b) : r(_r), g(_g), b(_b) {}
    Color(const Color & other) : r(other.r), g(other.g), b(other.b) {}

    // Converts the float into an 8 bit binary number. If the float is too
    // large it becomes 255 and if it's to small it becomes 0.
    static uint8_t boundFloat(float f);
    
    uint8_t r; // red (0 - 255)
    uint8_t g; // green
    uint8_t b; // blue

    // 0 <= ratio <= 1
    // 1 == normal color
    // 0 == completely black
    Color getFade(const float & ratio) const;

    // These don't blend colors together in any way. They just combine the raw
    // r g b values and bound them from 0 to 255.
    Color operator+(const Color & other) const;
    Color & operator+=(const Color & other);
    Color operator-(const Color & other) const;
    Color & operator-=(const Color & other);

    Color operator*(const unsigned & other) const;
    Color & operator*=(const unsigned & other);
    Color operator/(const unsigned & other) const;
    Color & operator/=(const unsigned & other);
};


// -------------------------------------------------------------------------- //
#define GET_RGB(r,g,b) ((r << 16) | (g << 8) | (b))

// Sample Colors
#define COLOR_BLACK Color(0, 0, 0)
#define COLOR_BLUE Color(0, 0, 255)
#define COLOR_GREEN Color(0, 255, 0)
#define COLOR_MAGENTA Color(255, 0, 255)
#define COLOR_RED Color(255, 0, 0)
#define COLOR_TEAL Color(0, 255, 255) // or is this turquoise?
#define COLOR_WHITE Color(255, 255, 255)
#define COLOR_YELLOW Color(255, 255, 0)

#define COLOR_ORANGE Color(255, 165, 0)
