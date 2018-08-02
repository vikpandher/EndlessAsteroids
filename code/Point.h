/* ==========================================================================
   >File: Point.h
   >Date: 20180713
   >Author: Vik Pandher
   >Details: This class represents a point in 3D space.
   ========================================================================== */

#pragma once
#include "MathUtilities.h"
#include "ColorUtilities.h"

// Mexican Standoff
#ifndef GRAPHICS_INCLUDES
    #define GRAPHICS_INCLUDES true
    #include "Vector.h"
    #include "Matrix.h"
#else
    class Vector;
    class Matrix;
#endif



class Point
{
public:
    Point();
    Point(const Point & other);
    Point(float _x, float _y, float _z);
    Point(float _x, float _y, float _z, Color _color);
    ~Point();

    // After preforming the viewingTransform, the point's w value is nolonger 1.
    // By dividing all the point values by w, the point is brought back into the
    // 3D "plane" from a 4D space.
    inline void to3D() { *this /= w; }

    // moveTowards
    // ====================================================================== //
    // Move this point towards another by some given fraction. For example,
    // the fraction 0.25 would move this vector 0.25 the way to the other.
    // 
    // @params
    // * const Point & other, reference to the other vector
    // * float fraction, fractional distance towards other point
    void moveTowards(const Point & other, float fraction);

    // Doesn't compare point colors
    bool operator==(const Point & other) const;
    bool operator!=(const Point & other) const;

    // Point - Point = Vector
    Vector operator-(const Point & other) const;

    // Point + Vector = Point
    Point operator+(const Vector & other) const;
    Point & operator+=(const Vector & other);

    // Point - Vector = Point
    Point operator-(const Vector & other) const;
    Point & operator-=(const Vector & other);

    // Point * Matrix = Point
    // Points act like a 4x1 matrix
    Point operator*(const Matrix & other) const;
    Point & operator*=(const Matrix & other);

    // Point * Scalar = Point
    Point operator*(const float & other) const;
    Point & operator*=(const float & other);

    // Point / Scalar = Point
    Point operator/(const float & other) const;
    Point & operator/=(const float & other);

    union
    {
        float m_data[4]; // last value is a 1
        struct
        {
            float x, y, z, w;
        };
    };

    // NOTE:
    // -,+,*,/ operaters result in the creation of a new point which reverts to
    // the deafault color, white.
    // -=,*=,*=,/= operators modify the current point so they maintain the
    // current color.
    union
    {
        Color m_color;
        struct
        {
            uint8_t r, g, b;
        };
    };
    
};