/* ==========================================================================
   >File: Point.cpp
   >Date: 20180713
   >Author: Vik Pandher
   >Details: This class represents a point in 3D space.
   ========================================================================== */

#include "Point.h"

Point::Point()
{
    x = 0;
    y = 0;
    z = 0;
    w = 1;
}


Point::Point(const Point & other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
    m_color = other.m_color;
}


Point::Point(float _x, float _y, float _z)
{
    x = _x;
    y = _y;
    z = _z;
    w = 1;
}


Point::Point(float _x, float _y, float _z, Color _color)
{
    x = _x;
    y = _y;
    z = _z;
    w = 1;
    m_color = _color;
}


Point::~Point()
{
    // nothing to do here
}


void Point::moveTowards(const Point & other, float fraction)
{
    x += (other.x - x) * fraction;
    y += (other.y - y) * fraction;
    z += (other.z - z) * fraction;
}


bool Point::operator==(const Point & other) const
{
    for (int i = 0; i < 4; i++)
    {
        if (!equalULP(m_data[i], other.m_data[i])) return false;
    }

    return true;
}


bool Point::operator!=(const Point & other) const
{
    for (int i = 0; i < 4; i++)
    {
        if (!equalULP(m_data[i], other.m_data[i])) return true;
    }

    return false;
}


Vector Point::operator-(const Point & other) const
{
    Vector newVector;
    for (int i = 0; i < 4; i++)
    {
        newVector.m_data[i] = m_data[i] - other.m_data[i];
    }
    return newVector;
}


Point Point::operator+(const Vector & other) const
{
    Point newPoint;
    for (int i = 0; i < 4; i++)
    {
        newPoint.m_data[i] = m_data[i] + other.m_data[i];
    }
    return newPoint;
}


Point & Point::operator+=(const Vector & other)
{
    for (int i = 0; i < 4; i++)
    {
        m_data[i] += other.m_data[i];
    }
    return *this;
}


Point Point::operator-(const Vector & other) const
{
    Point newPoint;
    for (int i = 0; i < 4; i++)
    {
        newPoint.m_data[i] = m_data[i] - other.m_data[i];
    }
    return newPoint;
}


Point & Point::operator-=(const Vector & other)
{
    for (int i = 0; i < 4; i++)
    {
        m_data[i] -= other.m_data[i];
    }
    return *this;
}


Point Point::operator*(const Matrix & other) const
{
    float newData[4] = { 0 };
    for (int col = 0; col < 4; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            newData[col] += this->m_data[row] * other.m_data[row][col];
        }
    }

    Point newPoint;

    for (int i = 0; i < 4; i++)
    {
        newPoint.m_data[i] = newData[i];
    }

    return newPoint;
}


Point & Point::operator*=(const Matrix & other)
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


Point Point::operator*(const float & other) const
{
    Point newPoint;
    for (int i = 0; i < 4; i++)
    {
        newPoint.m_data[i] = m_data[i] * other;
    }
    return newPoint;
}


Point & Point::operator*=(const float & other)
{
    for (int i = 0; i < 4; i++)
    {
        m_data[i] *= other;
    }
    return *this;
}


Point Point::operator/(const float & other) const
{
    Point newPoint;
    for (int i = 0; i < 4; i++)
    {
        newPoint.m_data[i] = m_data[i] / other;
    }
    return newPoint;
}


Point & Point::operator/=(const float & other)
{
    for (int i = 0; i < 4; i++)
    {
        m_data[i] /= other;
    }
    return *this;
}
