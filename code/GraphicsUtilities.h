/* ==========================================================================
   >File: GraphicsUtilities.h
   >Date: 20180713
   >Author: Vik Pandher
   >Details: Here are some structs I use for graphics stuff.
   ========================================================================== */

#pragma once
#include "String.h"
// ^ includes Array.h
#include "Point.h"
#include "Vector.h"
#include "Quaternion.h"



// Holds all the things needed for a camera
struct Camera
{
    Camera() {}
    Camera(Point cOA, Point cL, Vector uD, float vA, float nP, float fP) :
        centerOfAttention(cOA),
        cameraLocation(cL),
        upDirection(uD),
        viewingAngle(vA),
        nearPlane(nP),
        farPlane(fP) {}
    
    // Variables for the CameraTransform
    // Where the camera is looking
    Point centerOfAttention;

    // Point where the camera is
    Point cameraLocation;

    // Vector used to orient the camera
    Vector upDirection;

    // viewingVector = centerOfAttention - cameraLocation

    // Variables for the ViewingTransform
    float viewingAngle;
    float nearPlane;
    float farPlane;
};


// Represents a line in 3D space. Holds two points.
struct Line
{
    Line() {}
    Line(const Point & p0, const Point & p1) : p0(p0), p1(p1) {}

    inline void to3D() { p0.to3D(); p1.to3D(); }

    // multiply all the points by this matrix.
    Line operator*(const Matrix & other) const;
    Line & operator*=(const Matrix & other);

    Point p0;
    Point p1;
};


// Represents a triangle in 3D space. Holds three points.
struct Triangle
{
    Triangle() {}
    Triangle(const Point & p0, const Point & p1, const Point & p2) : p0(p0), p1(p1), p2(p2) {}

    inline void to3D() { p0.to3D(); p1.to3D(); p2.to3D(); }

    // Returns the normal calculated by crossing the vector from p0 to p1 with
    // the vector from p1 to p2.
    Vector getNormal() const;

    // multiply all the points by this matrix.
    Triangle operator*(const Matrix & other) const;
    Triangle & operator*=(const Matrix & other);

    Point p0;
    Point p1;
    Point p2;
};


// Represents a shape in 3D space. Holds an array of points, lines, and
// triangles. Points are expected to be repeated: lines and triangles
// will have some identical points if they are touching.
struct Shape
{
    Shape() {}

    // Durring the rasterization step, after applying the viewing transform,
    // the 4th dimension (w) in the points will nolonger be 1. This function
    // turns it back to being 1 by dividing all the values in the points by w.
    void to3D();

    // multiply all the points by a matrix.
    Shape operator*(const Matrix & other) const;
    Shape & operator*=(const Matrix & other);

    bool outlineOnly;
    Array<Point> points;
    Array<Line> lines;
    Array<Triangle> triangles;
};


struct FrameLine
{
    FrameLine() {}
    FrameLine(Point * p0, Point * p1) : p0(p0), p1(p1) {}

    Line getLine() const;

    bool operator==(const FrameLine & other) const;

    Point * p0;
    Point * p1;
};


struct FrameTriangle
{
    FrameTriangle() {}
    FrameTriangle(Point * p0, Point * p1, Point * p2) : p0(p0), p1(p1), p2(p2) {}

    // Returns the normal calculated by crossing the vector from p0 to p1 with
    // the vector from p1 to p2.
    Vector getNormal() const;

    Triangle getTriangle() const;

    Point * p0;
    Point * p1;
    Point * p2;
};

// It's IMPORTANT that a frame be VALID, or things may crash. For a frame to be
// VALID it must have points if it has frame lines. The points must include, but
// are not limited to, the points that make up the frame lines. For a frame to
// be VALID it must have frame lines if it has frame triangles. The frame lines
// must include, but are not limited to, the frame lines that make up the frame
// triangles!!!
struct Frame
{
    Frame() {}

    // Generate a shape object with this frame
    Shape getShape(bool includePoints = false, bool includeLines = false, bool includeTriangles = true) const;

    // This operator NEEDS to be overloaded because FrameLines and FrameTriangles hold pointers to Points.
    // When the arrays get copied over the frame will get assigned pointers to the old points. To avoid this
    // I'll need to figure out how the addresses for the new points relate to the old pointers.
    Frame & operator=(const Frame& other);

    // multiply all the points by a matrix.
    Frame operator*(const Matrix & other) const;
    Frame & operator*=(const Matrix & other);

    Array<Point> points;
    Array<FrameLine> lines;
    Array<FrameTriangle> triangles;
};
