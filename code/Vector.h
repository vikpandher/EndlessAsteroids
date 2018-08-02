/* ==========================================================================
   >File: Vector.h
   >Date: 20180713
   >Author: Vik Pandher
   >Details: This class represents a vector in 3D space.
   ========================================================================== */

#pragma once
#include "MathUtilities.h"

#ifndef GRAPHICS_INCLUDES
    #define GRAPHICS_INCLUDES true
    #include "Point.h"
    #include "Matrix.h"
#else
    class Point;
    class Matrix;
#endif



class Vector
{
public:
    Vector();
    Vector(const Vector & other);
    Vector(float _x, float _y, float _z);
    ~Vector();

    // magnitude
    // ====================================================================== //
    // Calculate the magnitude of this vector.
    // 
    // @return
    // The magnitude of this vector
    float magnitude() const;

    // normalize
    // ====================================================================== //
    // Normalize this vector. Make it so it still points in the same
    // direction, but now has a magnitude of 1 a. k. a. a unit vector.
    // 
    // NOTE: Doesn't work if the magnitude of the vector is 0.
    void normalize();

    // dotProduct
    // ====================================================================== //
    // Calculate the dotProduct of this vector with another.
    // 
    // @param
    // * const Vector & other, reference to the other vector
    // 
    // @return
    // The dot product of this vector with another vector
    float dotProduct(const Vector & other) const;

    // crossProduct
    // ====================================================================== //
    // Calculate the crossProduct of this vector with another
    // 
    // @param
    // * const Vector & other, reference to the other vector
    // 
    // @return
    // The cross product of this vector with another vector
    Vector crossProduct(const Vector & other) const;

    // angleBetween
    // ====================================================================== //
    // Calculate the angle between this vector and another.
    // 
    // @param
    // * const Vector & other, reference to the other vector
    // 
    // @return
    // The angle, in radians.
    float angleBetween(const Vector & other) const;

    // rotateTowards
    // ====================================================================== //
    // Move this vectors towards another by some given fraction. For example,
    // the fraction 0.25 would move this vector 0.25 the way to the other.
    // 
    // @params
    // * const Vector & other, reference to the other vector
    // * float fraction, fractional distance towards other vector
    void rotateTowards(const Vector & other, float fraction);

    // isParallel
    // ====================================================================== //
    // Check if this vector is parallel to another vector.
    // 
    // NOTE: Vector(0, 0, 0) will be parallel to any other vector.
    // 
    // @param
    // * const Vector & other, reference to the other vector
    // 
    // @return
    // Are the two vectors parallel? (true == yes)
    bool isParallel(const Vector & other) const;

    // isOrthogonal
    // ====================================================================== //
    // Check if this vector is orthogonal (perpendicular) to another vector.
    // 
    // @param
    // * const Vector & other, reference to the other vector
    // 
    // @return
    // Are the two vectors orthogonal? (true == yes)
    bool isOrthogonal(const Vector & other) const;

    // isUnitVector
    // ====================================================================== //
    // Check if this vector is a unit vector (has a length of 1).
    //
    // @return
    // True if this is a unit vector.
    bool isUnitVector() const;

    // getProjection
    // ====================================================================== //
    // Calculate the projection of this vector onto another.
    // 
    // @param
    // * const Vector & other, reference to the other vector
    // 
    // @return
    // A Vector equalling the projection of this vector onto another.
    Vector getProjection(const Vector & other) const;

    bool operator==(const Vector & other) const;
    bool operator!=(const Vector & other) const;

    // Vector + Point = Point
    Point operator+(const Point & other) const;

    // Vector + Vector = Vector
    Vector operator+(const Vector & other) const;
    Vector & operator+=(const Vector & other);
    
    // Vector - Vector = Vector
    Vector operator-(const Vector & other) const;
    Vector & operator-=(const Vector & other);

    // Vector * Matrix = Vector
    // Vectors act like a 4x1 matrix
    Vector operator*(const Matrix & other) const;
    Vector & operator*=(const Matrix & other);

    // Vector * Scalar = Vector
    Vector operator*(const float & other) const;
    Vector & operator*=(const float & other);

    // Vector / Scalar = Vector
    Vector operator/(const float & other) const;
    Vector & operator/=(const float & other);

    union
    {
        float m_data[4]; // last value is a 0
        struct
        {
            float x, y, z;
        };
    };
};