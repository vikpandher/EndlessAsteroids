/* ==========================================================================
   >File: Vector.cpp
   >Date: 20180713
   >Author: Vik Pandher
   >Details: This class represents a vector in 3D space.
   ========================================================================== */

#include "Vector.h"

Vector::Vector()
{
    x = 0;
    y = 0;
    z = 0;
    m_data[3] = 0;
}


Vector::Vector(const Vector & other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    m_data[3] = other.m_data[3];
}


Vector::Vector(float _x, float _y, float _z)
{
    x = _x;
    y = _y;
    z = _z;
    m_data[3] = 0;
}


Vector::~Vector()
{
    // nothing to do here
}


float Vector::magnitude() const
{
    return sqrt(x * x + y * y + z * z);
}


void Vector::normalize()
{
    *this /= magnitude();
}


float Vector::dotProduct(const Vector & other) const
{
    return x * other.x + y * other.y + z * other.z;
}


Vector Vector::crossProduct(const Vector & other) const
{
    float newX = this->y * other.z - this->z * other.y;
    float newY = this->z * other.x - this->x * other.z;
    float newZ = this->x * other.y - this->y * other.x;
    return Vector(newX, newY, newZ);
}


float Vector::angleBetween(const Vector & other) const
{
    float dot = dotProduct(other);
    float mag0 = magnitude();
    float mag1 = other.magnitude();
    float denominator = mag0 * mag1;
    if (denominator == 0)
        return arcCos(SIGN(dot));
    else
        return arcCos(dot / (mag0 * mag1));
}


void Vector::rotateTowards(const Vector & other, float fraction)
{
    float rotationAngle = angleBetween(other);
    Vector crossVector;
    if (equalULP(rotationAngle, 0)) return;
    if (equalULP(rotationAngle, _PI)) crossVector = Vector(0, 0, 1);
    else crossVector = crossProduct(other);
    if (fraction > 1) fraction = 1;
    if (fraction < 0) fraction = 0;
    Matrix rotation; rotation.addRotationV(crossVector, -1 * ABS(rotationAngle) * fraction);
    *this *= rotation;
}


bool Vector::isParallel(const Vector & other) const
{
    Vector temp = crossProduct(other);
    return equalULP(temp.x, 0) && equalULP(temp.y, 0) && equalULP(temp.z, 0);
}


bool Vector::isOrthogonal(const Vector & other) const
{
    return equalULP(dotProduct(other), 0);
}


bool Vector::isUnitVector() const
{
    return equalULP(magnitude(), 1);
}


Vector Vector::getProjection(const Vector & other) const
{
    return other * (other.dotProduct(*this) / other.dotProduct(other));
}


bool Vector::operator==(const Vector & other) const
{
    for (int i = 0; i < 4; i++)
    {
        if (!equalULP(m_data[i], other.m_data[i])) return false;
    }

    return true;
}


bool Vector::operator!=(const Vector & other) const
{
    for (int i = 0; i < 4; i++)
    {
        if (!equalULP(m_data[i], other.m_data[i])) return true;
    }

    return false;
}


Point Vector::operator+(const Point & other) const
{
    Point newPoint;
    for (int i = 0; i < 4; i++)
    {
        newPoint.m_data[i] = m_data[i] + other.m_data[i];
    }
    return newPoint;
}


Vector Vector::operator+(const Vector & other) const
{
    Vector newVector;
    for (int i = 0; i < 4; i++)
    {
        newVector.m_data[i] = m_data[i] + other.m_data[i];
    }
    return newVector;
}


Vector & Vector::operator+=(const Vector & other)
{
    for (int i = 0; i < 4; i++)
    {
        m_data[i] += other.m_data[i];
    }
    return *this;
}


Vector Vector::operator-(const Vector & other) const
{
    Vector newVector;
    for (int i = 0; i < 4; i++)
    {
        newVector.m_data[i] = m_data[i] - other.m_data[i];
    }
    return newVector;
}


Vector & Vector::operator-=(const Vector & other)
{
    for (int i = 0; i < 4; i++)
    {
        m_data[i] -= other.m_data[i];
    }
    return *this;
}


Vector Vector::operator*(const Matrix & other) const
{
    float newData[4] = { 0 };
    for (int col = 0; col < 4; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            newData[col] += this->m_data[row] * other.m_data[row][col];
        }
    }

    Vector newVector;

    for (int i = 0; i < 4; i++)
    {
        newVector.m_data[i] = newData[i];
    }

    return newVector;
}


Vector & Vector::operator*=(const Matrix & other)
{
    float newData[4] = { 0 };
    for (int col = 0; col < 4; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            newData[col] += this->m_data[row] * other.m_data[row][col];
        }
    }

    for (int i = 0; i < 4; i++)
    {
        this->m_data[i] = newData[i];
    }

    return *this;
}


Vector Vector::operator*(const float & other) const
{
    Vector newVector;
    for (int i = 0; i < 4; i++)
    {
        newVector.m_data[i] = m_data[i] * other;
    }
    return newVector;
}


Vector & Vector::operator*=(const float & other)
{
    for (int i = 0; i < 4; i++)
    {
        m_data[i] *= other;
    }
    return *this;
}


Vector Vector::operator/(const float & other) const
{
    Vector newVector;
    for (int i = 0; i < 4; i++)
    {
        newVector.m_data[i] = m_data[i] / other;
    }
    return newVector;
}


Vector & Vector::operator/=(const float & other)
{
    for (int i = 0; i < 4; i++)
    {
        m_data[i] /= other;
    }
    return *this;
}