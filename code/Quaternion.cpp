/* ==========================================================================
   >File: Quaternion.cpp
   >Date: 20180713
   >Author: Vik Pandher
   >Details: This class represents a quaternion. It's usefull for doing 3D
             rotations.
   ========================================================================== */

#include "Quaternion.h"



Quaternion::Quaternion()
{
    setToIdentity();
}


Quaternion::Quaternion(const Quaternion & other)
{
    a = other.a;
    b = other.b;
    c = other.c;
    d = other.d;
}


Quaternion::~Quaternion()
{

}


void Quaternion::setToIdentity()
{
    a = 1;
    b = 0;
    c = 0;
    d = 0;
}


void Quaternion::setToZero()
{
    a = 0;
    b = 0;
    c = 0;
    d = 0;
}


float Quaternion::length() const
{
    return sqrt(a * a + b * b + c * c + d * d);
}


void Quaternion::normalize()
{
    float length = this->length();
    a /= length;
    b /= length;
    c /= length;
    d /= length;
}


bool Quaternion::isUnitQuaternion() const
{
    return equalULP(length(), 1);
}


void Quaternion::invert()
{
    float denominator = a * a + b * b + c * c + d * d;
    a /= denominator;
    b /= (-1) * denominator;
    c /= (-1) * denominator;
    d /= (-1) * denominator;
}

Quaternion Quaternion::getInverse() const
{
    float denominator = a * a + b * b + c * c + d * d;
    return Quaternion(a / denominator, -b / denominator, -c / denominator, -d / denominator);
}

void Quaternion::addRotation(const Vector & vector, float theta)
{
    float newA = cos(theta / 2);
    float sinTheta = sin(theta / 2);
    float newB = sinTheta * vector.x;
    float newC = sinTheta * vector.y;
    float newD = sinTheta * vector.z;
    // Note the order
    *this = Quaternion(newA, newB, newC, newD) * *this;
}


Matrix Quaternion::getMatrix() const
{
    return Matrix((1 - 2 * c * c - 2 * d * d), (2 * b * c - 2 * d * a), (2 * b * d + 2 * c * a), 0,
                  (2 * b * c + 2 * d * a), (1 - 2 * b * b - 2 * d * d), (2 * c * d - 2 * b * a), 0,
                  (2 * b * d - 2 * c * a), (2 * c * d + 2 * b * a), (1 - 2 * b * b - 2 * c * c), 0,
                  0, 0, 0, 1);
}


Vector Quaternion::getForwardVector() const
{
    return Vector((1 - 2 * c * c - 2 * d * d), (2 * b * c - 2 * d * a), (2 * b * d + 2 * c * a));
}
Vector Quaternion::getUpVector() const
{
    return Vector((2 * b * c + 2 * d * a), (1 - 2 * b * b - 2 * d * d), (2 * c * d - 2 * b * a));
}
Vector Quaternion::getRightVector() const
{
    return Vector((2 * b * d - 2 * c * a), (2 * c * d + 2 * b * a), (1 - 2 * b * b - 2 * c * c));
}


bool Quaternion::operator==(const Quaternion & other) const
{
    return equalULP(a, other.a) && equalULP(b, other.b) && equalULP(c, other.c) && equalULP(d, other.d);
}


bool Quaternion::operator!=(const Quaternion & other) const
{
    return !equalULP(a, other.a) || !equalULP(b, other.b) || !equalULP(c, other.c) || !equalULP(d, other.d);
}


Quaternion Quaternion::operator*(const Quaternion & other) const
{
    // q = (a, v) = a + b * i + c * j + d * k
    // q0 * q1 = (a0 * a2 - (v0 dot v1), a0 * v1 + a1 * v0 + (v0 cross v1))
    // (v0 dot v1) = b0 * b1 + c0 * c1 + d0 * d1
    //                 [c0 * d1 - d0 * c1]
    // (v0 cross v1) = [d0 * b1 - b0 * d1]
    //                 [b0 * c1 - c0 * b1]
    float newA = a * other.a - b * other.b - c * other.c - d * other.d;
    float newB = a * other.b + b * other.a + c * other.d - d * other.c;
    float newC = a * other.c + c * other.a + d * other.b - b * other.d;
    float newD = a * other.d + d * other.a + b * other.c - c * other.b;
    return Quaternion(newA, newB, newC, newD);
}


Quaternion & Quaternion::operator*=(const Quaternion & other)
{
    float newA = a * other.a - b * other.b - c * other.c - d * other.d;
    float newB = a * other.b + b * other.a + c * other.d - d * other.c;
    float newC = a * other.c + c * other.a + d * other.b - b * other.d;
    float newD = a * other.d + d * other.a + b * other.c - c * other.b;
    a = newA;
    b = newB;
    c = newC;
    d = newD;
    return *this;
}


Quaternion Quaternion::operator/(const Quaternion & other) const
{
    return *this * other.getInverse();
}


Quaternion & Quaternion::operator/=(const Quaternion & other)
{
    *this *= other.getInverse();
    return *this;
}


Quaternion Quaternion::operator*(const float & other) const
{
    return Quaternion(a * other, b * other, c * other, d * other);
}


Quaternion & Quaternion::operator*=(const float & other)
{
    a *= other; b *= other; c *= other; d *= other;
    return *this;
}


Quaternion Quaternion::operator/(const float & other) const
{
    return Quaternion(a / other, b / other, c / other, d / other);
}


Quaternion & Quaternion::operator/=(const float & other)
{
    a /= other; b /= other; c /= other; d /= other;
    return *this;
}
