/* ==========================================================================
   >File: ColorUtilities.h
   >Date: 20180713
   >Author: Vik Pandher
   >Details: Here's the color struct I use and some color defines.
   ========================================================================== */

#include "ColorUtilities.h"



Color::Color(uint32_t v)
{
    b = (v & 0xff);
    g = ((v >> 8) & 0xff);
    r = ((v >> 16) & 0xff);
}


uint8_t Color::boundFloat(float f)
{
    if (f > 255 || equalULP(f, 255))
    {
        return 255;
    }
    else if (f < 0 || equalULP(f, 0))
    {
        return 0;
    }
    else
    {
        return (uint8_t)f;
    }
}


Color Color::getFade(const float & ratio) const
{
    if (equalULP(ratio, 0) || ratio < 0)
    {
        return Color(0, 0, 0);
    }
    else if (ratio > 0 && ratio < 1)
    {
        return Color((uint8_t)(r * ratio), (uint8_t)(g * ratio), (uint8_t)(b * ratio));
    }

    return Color(r, g, b);
}


Color Color::operator+(const Color & other) const
{
    uint8_t red, gre, blu;

    if (this->r + other.r < 256)
        red = this->r + other.r;
    else
        red = 255;

    if (this->g + other.g < 256)
        gre = this->g + other.g;
    else
        gre = 255;

    if (this->b + other.b < 256)
        blu = this->b + other.b;
    else
        blu = 255;

    return Color(red, gre, blu);
}


Color & Color::operator+=(const Color & other)
{
    if (this->r + other.r < 256)
        this->r += other.r;
    else
        this->r = 255;

    if (this->g + other.g < 256)
        this->g += other.g;
    else
        this->g = 255;

    if (this->b + other.b < 256)
        this->b += other.b;
    else
        this->b = 255;

    return *this;
}


Color Color::operator-(const Color & other) const
{
    uint8_t red, gre, blu;

    if (this->r - other.r >= 0)
        red = this->r - other.r;
    else
        red = 0;

    if (this->g - other.g >= 0)
        gre = this->g - other.g;
    else
        gre = 0;

    if (this->b - other.b >= 0)
        blu = this->b - other.b;
    else
        blu = 0;

    return Color(red, gre, blu);
}


Color & Color::operator-=(const Color & other)
{
    if (this->r - other.r >= 0)
        this->r -= other.r;
    else
        this->r = 0;

    if (this->g - other.g >= 0)
        this->g -= other.g;
    else
        this->g = 0;

    if (this->b - other.b >= 0)
        this->b -= other.b;
    else
        this->b = 0;

    return *this;
}


Color Color::operator*(const unsigned & other) const
{
    Color newColor(*this);

    if (this->r * other < 256)
        newColor.r *= other;
    else
        newColor.r = 255;

    if (this->g * other < 256)
        newColor.g *= other;
    else
        newColor.g = 255;

    if (this->b * other < 256)
        newColor.b *= other;
    else
        newColor.b = 255;

    return newColor;
}


Color & Color::operator*=(const unsigned & other)
{
    if (this->r * other < 256)
        this->r *= other;
    else
        this->r = 255;

    if (this->g * other < 256)
        this->g *= other;
    else
        this->g = 255;

    if (this->b * other < 256)
        this->b *= other;
    else
        this->b = 255;

    return *this;
}


Color Color::operator/(const unsigned & other) const
{
    Color newColor(*this);

    newColor.r /= other;
    newColor.g /= other;
    newColor.b /= other;

    return newColor;
}


Color & Color::operator/=(const unsigned & other)
{
    this->r /= other;
    this->g /= other;
    this->b /= other;

    return *this;
}