/* ==========================================================================
   >File: GraphicsUtilities.cpp
   >Date: 20180713
   >Author: Vik Pandher
   >Details: Here are some structs I use for graphics stuff.
   ========================================================================== */

#include "GraphicsUtilities.h"



Line Line::operator*(const Matrix & other) const
{
    return Line(p0 * other, p1 * other);
}


Line & Line::operator*=(const Matrix & other)
{
    p0 *= other;
    p1 *= other;
    return *this;
}


Vector Triangle::getNormal() const
{
    Vector v0 = p1 - p0;
    Vector v1 = p2 - p1;
    Vector cross = v0.crossProduct(v1);
    cross.normalize();
    return cross;
}


Triangle Triangle::operator*(const Matrix & other) const
{
    return Triangle(p0 * other, p1 * other, p2 * other);
}


Triangle & Triangle::operator*=(const Matrix & other)
{
    p0 *= other;
    p1 *= other;
    p2 *= other;
    return *this;
}


void Shape::to3D()
{
    for (int i = 0; i < points.size(); i++)
    {
        points[i].to3D();
    }

    for (int i = 0; i < lines.size(); i++)
    {
        lines[i].to3D();
    }

    for (int i = 0; i < triangles.size(); i++)
    {
        triangles[i].to3D();
    }
}


Shape Shape::operator*(const Matrix & other) const
{
    Shape newShape;

    for (int i = 0; i < points.size(); i++)
    {
        newShape.points += points[i] * other;
    }

    for (int i = 0; i < lines.size(); i++)
    {
        newShape.lines += lines[i] * other;
    }

    for (int i = 0; i < triangles.size(); i++)
    {
        newShape.triangles += triangles[i] * other;
    }

    return newShape;
}


Shape & Shape::operator*=(const Matrix & other)
{
    for (int i = 0; i < points.size(); i++)
    {
        points[i] *= other;
    }

    for (int i = 0; i < lines.size(); i++)
    {
        lines[i] *= other;
    }

    for (int i = 0; i < triangles.size(); i++)
    {
        triangles[i] *= other;
    }

    return *this;
}


Line FrameLine::getLine() const
{
    return Line(*p0, *p1);
}


bool FrameLine::operator==(const FrameLine & other) const
{
    return *p0 == *(other.p0) && *p1 == *(other.p1) || *p0 == *(other.p1) && *p1 == *(other.p0);
}


Vector FrameTriangle::getNormal() const
{
    Vector v0 = *p1 - *p0;
    Vector v1 = *p2 - *p1;
    Vector cross = v0.crossProduct(v1);
    cross.normalize();
    return cross;
}


Triangle FrameTriangle::getTriangle() const
{
    return Triangle(*p0, *p1, *p2);
}


Shape Frame::getShape(bool includePoints /*= false*/, bool includeLines /*= false*/, bool includeTriangles /*= true*/) const
{
    Shape shape;

    if (includePoints)
    {
        for (int i = 0; i < points.size(); i++)
        {
            shape.points += points[i];
        }
    }

    if (includeLines)
    {
        for (int i = 0; i < lines.size(); i++)
        {
            shape.lines += Line(*(lines[i].p0), *(lines[i].p1));
        }
    }

    if (includeTriangles)
    {
        for (int i = 0; i < triangles.size(); i++)
        {
            shape.triangles += Triangle(*(triangles[i].p0), *(triangles[i].p1), *(triangles[i].p2));
        }
    }

    return shape;
}

Frame & Frame::operator=(const Frame& other)
{
    points = other.points;
    if (points.size() > 0)
    {
        // calculate the difference to correct the new pointers
        int oldFirstAddress = (int)&other.points[0];
        int newFirstAddress = (int)&points[0];
        int difference = oldFirstAddress - newFirstAddress;

        lines = other.lines;
        for (int i = 0; i < lines.size(); i++)
        {
            lines[i].p0 -= difference;
            lines[i].p1 -= difference;
        }

        triangles = other.triangles;
        for (int i = 0; i < triangles.size(); i++)
        {
            triangles[i].p0 -= difference;
            triangles[i].p1 -= difference;
            triangles[i].p2 -= difference;
        }
    }
    return *this;
}


Frame Frame::operator*(const Matrix & other) const
{
    Frame newFrame;

    for (int i = 0; i < points.size(); i++)
    {
        newFrame.points += points[i] * other;
    }

    for (int i = 0; i < lines.size(); i++)
    {
        newFrame.lines += lines[i];
    }

    for (int i = 0; i < triangles.size(); i++)
    {
        newFrame.triangles += triangles[i];
    }

    return newFrame;
}


Frame & Frame::operator*=(const Matrix & other)
{
    for (int i = 0; i < points.size(); i++)
    {
        points[i] *= other;
    }

    return *this;
}