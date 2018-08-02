/* ==========================================================================
   >File: GameUtilities.cpp
   >Date: 20180713
   >Author: Vik Pandher
   >Details: Functions use in the game, mostly revolving around Entities.
   ========================================================================== */

#include "GameUtilities.h"



void Entity::update()
{
    locationPoint += velocity;
    orientation *= angularVelocity;
    updateWorldSpaceShape();
}


Shape Entity::getShapeInWorldSpace() const
{
    Shape shape = frame.getShape(drawProperties & DRAW_POINTS, drawProperties & DRAW_LINES, drawProperties & DRAW_TRIANGLES);

    if (drawProperties & DRAW_NORMALS)
    {
        const Array<FrameTriangle> & t = frame.triangles;
        for (int i = 0; i < t.size(); i++)
        {
            float x = (t[i].p0->x + t[i].p1->x + t[i].p2->x) / 3;
            float y = (t[i].p0->y + t[i].p1->y + t[i].p2->y) / 3;
            float z = (t[i].p0->z + t[i].p1->z + t[i].p2->z) / 3;
            Point c = Point(x, y, z);
            Vector n = t[i].getNormal();
            shape.lines += Line(c, c + n);
        }
    }

    Matrix rotationMatrix = orientation.getMatrix();
    shape *= rotationMatrix;

    Matrix locationMatrix;
    locationMatrix.addTranslation(locationPoint.x, locationPoint.y, locationPoint.z);
    shape *= locationMatrix;

    return shape;
}


void Entity::updateWorldSpaceShape()
{
    worldSpaceShape = frame.getShape(false, true, true);

    Matrix rotationMatrix = orientation.getMatrix();
    worldSpaceShape *= rotationMatrix;

    Matrix locationMatrix;
    locationMatrix.addTranslation(locationPoint.x, locationPoint.y, locationPoint.z);
    worldSpaceShape *= locationMatrix;
}


void alignPlayCamera(const Entity & entity, Camera & camera, float d, float f)
{
    Vector goalVector = entity.orientation.getForwardVector();
    Quaternion rotation; rotation.addRotation(entity.orientation.getRightVector(), _PI / 8); // angle camera slightly above
    goalVector *= rotation.getMatrix();

    Vector upVector = entity.orientation.getUpVector();

    Vector newForwardVector = camera.centerOfAttention - camera.cameraLocation;
    newForwardVector.normalize();
    newForwardVector.rotateTowards(goalVector, f);
    Vector newUpVector = camera.upDirection;
    newUpVector.rotateTowards(upVector, f);

    camera.centerOfAttention = entity.locationPoint;
    camera.cameraLocation = entity.locationPoint - newForwardVector * d;
    camera.upDirection = newUpVector;

    camera.centerOfAttention += camera.upDirection * d / 8; // translate angle camera slightly above
    camera.cameraLocation += camera.upDirection * d / 8;
}


void alignZoomCamera(const Entity & entity, Camera & camera, float d, float f)
{
    camera.centerOfAttention.moveTowards(entity.locationPoint + entity.orientation.getForwardVector(), f);
    camera.cameraLocation.moveTowards(entity.locationPoint, f);
    camera.upDirection.rotateTowards(entity.orientation.getUpVector(), f);
}


void initializeShipFrame(Frame & frame, Color color0, Color color1, Color color2)
{
    // the pointers will get lost on array resizing
    frame.points.setCapacity(6);
    ///frame.points.setCapacity(12);

    frame.points += Point(1.5, 0, 0, color0);
    Point * front = frame.points.getPointerTo(0);

    frame.points += Point(-0.5, 0, 0, color0);
    Point * back = frame.points.getPointerTo(1);

    frame.points += Point(-0.5, 1, 0, color1);
    Point * top = frame.points.getPointerTo(2);

    frame.points += Point(-0.5, -1, 0, color1);
    Point * bottom = frame.points.getPointerTo(3);

    frame.points += Point(-1.75, 0, 1, color2);
    Point * right = frame.points.getPointerTo(4);

    frame.points += Point(-1.75, 0, -1, color2);
    Point * left = frame.points.getPointerTo(5);

    FrameLine line0(front, top);
    frame.lines += line0;
    FrameLine line1(front, right);
    frame.lines += line1;
    FrameLine line2(front, bottom);
    frame.lines += line2;
    FrameLine line3(front, left);
    frame.lines += line3;
    FrameLine line4(back, top);
    frame.lines += line4;
    FrameLine line5(back, right);
    frame.lines += line5;
    FrameLine line6(back, bottom);
    frame.lines += line6;
    FrameLine line7(back, left);
    frame.lines += line7;
    FrameLine line8(top, right);
    frame.lines += line8;
    FrameLine line9(right, bottom);
    frame.lines += line9;
    FrameLine line10(bottom, left);
    frame.lines += line10;
    FrameLine line11(left, top);
    frame.lines += line11;

    FrameTriangle face0(front, top, right);
    frame.triangles += face0;
    FrameTriangle face1(front, left, top);
    frame.triangles += face1;
    FrameTriangle face2(front, right, bottom);
    frame.triangles += face2;
    FrameTriangle face3(front, bottom, left);
    frame.triangles += face3;
    FrameTriangle face4(back, right, top);
    frame.triangles += face4;
    FrameTriangle face5(back, top, left);
    frame.triangles += face5;
    FrameTriangle face6(back, bottom, right);
    frame.triangles += face6;
    FrameTriangle face7(back, left, bottom);
    frame.triangles += face7;

    // add axes as well for debugging
    ///drawProperties |= DRAW_LINES;
    ///float r = 3;
    ///frame.points += Point(r, 0, 0, COLOR_RED);
    ///Point * xPos = frame.points.getPointerTo(6);
    ///frame.points += Point(-r, 0, 0, COLOR_RED / 4);
    ///Point * xNeg = frame.points.getPointerTo(7);
    ///frame.points += Point(0, r, 0, COLOR_GREEN);
    ///Point * yPos = frame.points.getPointerTo(8);
    ///frame.points += Point(0, -r, 0, COLOR_GREEN / 4);
    ///Point * yNeg = frame.points.getPointerTo(9);
    ///frame.points += Point(0, 0, r, COLOR_BLUE);
    ///Point * zPos = frame.points.getPointerTo(10);
    ///frame.points += Point(0, 0, -r, COLOR_BLUE / 4);
    ///Point * zNeg = frame.points.getPointerTo(11);
    ///FrameLine line12(xPos, xNeg);
    ///frame.lines += line12;
    ///FrameLine line13(yPos, yNeg);
    ///frame.lines += line13;
    ///FrameLine line14(zPos, zNeg);
    ///frame.lines += line14;
}


void initializeSaucerFrame(Frame & frame, float r, Color color0, Color color1)
{
    // the pointers will get lost on array resizing
    frame.points.setCapacity(14);

    // hypotenuse
    float h = sqrt(0.5 * r * r);

    frame.points += Point(r, 0, 0, color1);
    Point * front = frame.points.getPointerTo(0);

    frame.points += Point(h, 0, h, color1);
    Point * frontRight = frame.points.getPointerTo(1);

    frame.points += Point(0, 0, r, color1);
    Point * right = frame.points.getPointerTo(2);

    frame.points += Point(-h, 0, h, color1);
    Point * backRight = frame.points.getPointerTo(3);

    frame.points += Point(-r, 0, 0, color1);
    Point * back = frame.points.getPointerTo(4);

    frame.points += Point(-h, 0, -h, color1);
    Point * backLeft = frame.points.getPointerTo(5);

    frame.points += Point(0, 0, -r, color1);
    Point * left = frame.points.getPointerTo(6);

    frame.points += Point(h, 0, -h, color1);
    Point * frontLeft = frame.points.getPointerTo(7);

    frame.points += Point(0, -r / 4, 0, color1);
    Point * bottom = frame.points.getPointerTo(8);

    frame.points += Point(r / 4, r / 4, 0, color0);
    Point * topFront = frame.points.getPointerTo(9);

    frame.points += Point(0, r / 4, r / 4, color0);
    Point * topRight = frame.points.getPointerTo(10);

    frame.points += Point(-r / 4, r / 4, 0, color0);
    Point * topBack = frame.points.getPointerTo(11);

    frame.points += Point(0, r / 4, -r / 4, color0);
    Point * topLeft = frame.points.getPointerTo(12);

    frame.points += Point(0, r, 0, color0);
    Point * tippyTop = frame.points.getPointerTo(13);

    FrameLine line00(bottom, front);
    frame.lines += line00;
    FrameLine line01(bottom, frontRight);
    frame.lines += line01;
    FrameLine line02(bottom, right);
    frame.lines += line02;
    FrameLine line03(bottom, backRight);
    frame.lines += line03;
    FrameLine line04(bottom, back);
    frame.lines += line04;
    FrameLine line05(bottom, backLeft);
    frame.lines += line05;
    FrameLine line06(bottom, left);
    frame.lines += line06;
    FrameLine line07(bottom, frontLeft);
    frame.lines += line07;
    FrameLine line08(front, frontRight);
    frame.lines += line08;
    FrameLine line09(frontRight, right);
    frame.lines += line09;
    FrameLine line10(right, backRight);
    frame.lines += line10;
    FrameLine line11(backRight, back);
    frame.lines += line11;
    FrameLine line12(back, backLeft);
    frame.lines += line12;
    FrameLine line13(backLeft, left);
    frame.lines += line13;
    FrameLine line14(left, frontLeft);
    frame.lines += line14;
    FrameLine line15(frontLeft, front);
    frame.lines += line15;
    FrameLine line16(topFront, frontLeft);
    frame.lines += line16;
    FrameLine line17(topFront, front);
    frame.lines += line17;
    FrameLine line18(topFront, frontRight);
    frame.lines += line18;
    FrameLine line19(topRight, frontRight);
    frame.lines += line19;
    FrameLine line20(topRight, right);
    frame.lines += line20;
    FrameLine line21(topRight, backRight);
    frame.lines += line21;
    FrameLine line22(topBack, backRight);
    frame.lines += line22;
    FrameLine line23(topBack, back);
    frame.lines += line23;
    FrameLine line24(topBack, backLeft);
    frame.lines += line24;
    FrameLine line25(topLeft, backLeft);
    frame.lines += line25;
    FrameLine line26(topLeft, left);
    frame.lines += line26;
    FrameLine line27(topLeft, frontLeft);
    frame.lines += line27;
    FrameLine line28(topFront, topRight);
    frame.lines += line28;
    FrameLine line29(topRight, topBack);
    frame.lines += line29;
    FrameLine line30(topBack, topLeft);
    frame.lines += line30;
    FrameLine line31(topLeft, topFront);
    frame.lines += line31;
    FrameLine line32(topFront, tippyTop);
    frame.lines += line32;
    FrameLine line33(topRight, tippyTop);
    frame.lines += line33;
    FrameLine line34(topBack, tippyTop);
    frame.lines += line34;
    FrameLine line35(topLeft, tippyTop);
    frame.lines += line35;

    FrameTriangle face00(bottom, front, frontRight);
    frame.triangles += face00;
    FrameTriangle face01(bottom, frontRight, right);
    frame.triangles += face01;
    FrameTriangle face02(bottom, right, backRight);
    frame.triangles += face02;
    FrameTriangle face03(bottom, backRight, back);
    frame.triangles += face03;
    FrameTriangle face04(bottom, back, backLeft);
    frame.triangles += face04;
    FrameTriangle face05(bottom, backLeft, left);
    frame.triangles += face05;
    FrameTriangle face06(bottom, left, frontLeft);
    frame.triangles += face06;
    FrameTriangle face07(bottom, frontLeft, front);
    frame.triangles += face07;
    FrameTriangle face08(topFront, frontRight, front);
    frame.triangles += face08;
    FrameTriangle face09(topFront, topRight, frontRight);
    frame.triangles += face09;
    FrameTriangle face10(topRight, right, frontRight);
    frame.triangles += face10;
    FrameTriangle face11(topRight, backRight, right);
    frame.triangles += face11;
    FrameTriangle face12(topRight, topBack, backRight);
    frame.triangles += face12;
    FrameTriangle face13(topBack, back, backRight);
    frame.triangles += face13;
    FrameTriangle face14(topBack, backLeft, back);
    frame.triangles += face14;
    FrameTriangle face15(topBack, topLeft, backLeft);
    frame.triangles += face15;
    FrameTriangle face16(topLeft, left, backLeft);
    frame.triangles += face16;
    FrameTriangle face17(topLeft, frontLeft, left);
    frame.triangles += face17;
    FrameTriangle face18(topLeft, topFront, frontLeft);
    frame.triangles += face18;
    FrameTriangle face19(topFront, front, frontLeft);
    frame.triangles += face19;
    FrameTriangle face20(tippyTop, topRight, topFront);
    frame.triangles += face20;
    FrameTriangle face21(tippyTop, topBack, topRight);
    frame.triangles += face21;
    FrameTriangle face22(tippyTop, topLeft, topBack);
    frame.triangles += face22;
    FrameTriangle face23(tippyTop, topFront, topLeft);
    frame.triangles += face23;
}


void initializeBulletFrame(Frame & frame, Color color)
{
    // the pointers will get lost on array resizing
    frame.points.setCapacity(6);

    frame.points += Point(0.5, 0, 0, color);
    Point * front = frame.points.getPointerTo(0);
    frame.points += Point(-0.5, 0, 0, color);
    Point * back = frame.points.getPointerTo(1);
    frame.points += Point(0, 0.5, 0, color);
    Point * top = frame.points.getPointerTo(2);
    frame.points += Point(0, -0.5, 0, color);
    Point * bottom = frame.points.getPointerTo(3);
    frame.points += Point(0, 0, 0.5, color);
    Point * right = frame.points.getPointerTo(4);
    frame.points += Point(0, 0, -0.5, color);
    Point * left = frame.points.getPointerTo(5);

    FrameLine line0(top, front);
    frame.lines += line0;
    FrameLine line1(top, right);
    frame.lines += line1;
    FrameLine line2(top, back);
    frame.lines += line2;
    FrameLine line3(top, left);
    frame.lines += line3;
    FrameLine line4(bottom, front);
    frame.lines += line4;
    FrameLine line5(bottom, right);
    frame.lines += line5;
    FrameLine line6(bottom, back);
    frame.lines += line6;
    FrameLine line7(bottom, left);
    frame.lines += line7;

    FrameTriangle face0(front, top, right);
    frame.triangles += face0;
    FrameTriangle face1(right, top, back);
    frame.triangles += face1;
    FrameTriangle face2(back, top, left);
    frame.triangles += face2;
    FrameTriangle face3(left, top, front);
    frame.triangles += face3;
    FrameTriangle face4(bottom, front, right);
    frame.triangles += face4;
    FrameTriangle face5(bottom, right, back);
    frame.triangles += face5;
    FrameTriangle face6(bottom, back, left);
    frame.triangles += face6;
    FrameTriangle face7(bottom, left, front);
    frame.triangles += face7;
}


void initializeLongBulletFrame(Frame & frame, Color color)
{
    // the pointers will get lost on array resizing
    frame.points.setCapacity(6);

    frame.points += Point(1.55, 0, 0, color);
    Point * front = frame.points.getPointerTo(0);
    frame.points += Point(-1.55, 0, 0, color);
    Point * back = frame.points.getPointerTo(1);
    frame.points += Point(0, 0.5, 0, color);
    Point * top = frame.points.getPointerTo(2);
    frame.points += Point(0, -0.5, 0, color);
    Point * bottom = frame.points.getPointerTo(3);
    frame.points += Point(0, 0, 0.5, color);
    Point * right = frame.points.getPointerTo(4);
    frame.points += Point(0, 0, -0.5, color);
    Point * left = frame.points.getPointerTo(5);

    FrameLine line0(top, front);
    frame.lines += line0;
    FrameLine line1(top, right);
    frame.lines += line1;
    FrameLine line2(top, back);
    frame.lines += line2;
    FrameLine line3(top, left);
    frame.lines += line3;
    FrameLine line4(bottom, front);
    frame.lines += line4;
    FrameLine line5(bottom, right);
    frame.lines += line5;
    FrameLine line6(bottom, back);
    frame.lines += line6;
    FrameLine line7(bottom, left);
    frame.lines += line7;

    FrameTriangle face0(front, top, right);
    frame.triangles += face0;
    FrameTriangle face1(right, top, back);
    frame.triangles += face1;
    FrameTriangle face2(back, top, left);
    frame.triangles += face2;
    FrameTriangle face3(left, top, front);
    frame.triangles += face3;
    FrameTriangle face4(bottom, front, right);
    frame.triangles += face4;
    FrameTriangle face5(bottom, right, back);
    frame.triangles += face5;
    FrameTriangle face6(bottom, back, left);
    frame.triangles += face6;
    FrameTriangle face7(bottom, left, front);
    frame.triangles += face7;
}


void initializeAsteroidFrame(Frame & frame, float r, float n, float d, Color color)
{
    // These coordinates are in the Spherical Coordinate System
    // 
    // So the x,y,z for points will be interpreted a bit differently:
    // x = radial distance of a point from the origin
    //     range = [0, infinity)
    // y = polar angle from the +z axis towards the -z axis
    //     range = [0, _PI]
    // z = azimouth angle of projection in the xy plane, starting from the
    //     +x axis and going around counter clockwise
    //     range = [0, 2 * _PI]
    //
    // NOTE: These is a special case when c, the azimouth angle, doesn't matter
    //       This occurs when the point is on the z-axis. In these cases i'll
    //       set the aximouth angle equal to -1. This will make it easier to
    //       find a point on a line between the special case point and another
    //       arbitrary point. The new point's aximouth angle will simply be that
    //       of the non special case. If both points are on the z-axis, the
    //       azimouth angle will remain -1.
    //
    // ALSO: To make the calculations easier, the points with the azimouth
    //       angle of 2 * PI are included. These are in the same spot as points
    //       with an azimouth angle of 0. This overlap will be fixed
    //       after all the triangles have been calculated, but before the
    //       conversion to cartesian coordinates by redirecting the Point
    //       pointers in the triangles. Pointers to points with an azimouth
    //       angle of 2 * PI will point to points in the same spot with an
    //       azimouth angle of 0. To make this faster, pointers to points with
    //       an azimouth angle of zero will also be stored in an array.

    Array<Point> & points = frame.points;
    Array<Point*> zeros;
    Array<FrameTriangle> & triangles = frame.triangles;
    Array<FrameLine> & lines = frame.lines;

    // First things first, since the array of triangles contains pointers to
    // points, we can't be having those points move around in memory. I'll have
    // to make sure the array of points won't need to resize itself, so I'll
    // calculate how many points I'll end up with, with the given parameter n,
    // and set the array of points capacity to that.
    int pointCapacity = 0;
    int j = 1;
    for (int i = 0; i < n; i++)
    {
        j *= 2;
    }
    int i = 0;
    for (; i < j; i++)
    {
        pointCapacity += (1 + 4 * i) * 2;
    }
    pointCapacity += 1 + 4 * i;
    points.setCapacity(pointCapacity);

    // Adding initial points and triangles
    points += Point(r, 0, -1, color); // [0] top point
    points += Point(r, _PI / 2, 0, color); // [1] azimouth angle 0 point
    points += Point(r, _PI / 2, _PI / 2, color);
    points += Point(r, _PI / 2, _PI, color);
    points += Point(r, _PI / 2, 3 * _PI / 2, color);
    points += Point(r, _PI / 2, 2 * _PI, color); // [5] wrap around point
    points += Point(r, _PI, -1, color); // [6] bottom point

    // Add the first azimouth angle 0 point
    zeros += points.getPointerTo(1);

    // Starting triangles
    triangles += FrameTriangle(&(points[0]), &(points[1]), &(points[2]));
    triangles += FrameTriangle(&(points[6]), &(points[2]), &(points[1]));
    triangles += FrameTriangle(&(points[0]), &(points[2]), &(points[3]));
    triangles += FrameTriangle(&(points[6]), &(points[3]), &(points[2]));
    triangles += FrameTriangle(&(points[0]), &(points[3]), &(points[4]));
    triangles += FrameTriangle(&(points[6]), &(points[4]), &(points[3]));
    triangles += FrameTriangle(&(points[0]), &(points[4]), &(points[5]));
    triangles += FrameTriangle(&(points[6]), &(points[5]), &(points[4]));

    // Split up the triangles into smaller triangles to it more spherical
    for (int i = 0; i < n; i++)
    {
        int numberOfOldPoints = points.size();
        Array<FrameTriangle> newTriangles;

        for (int j = 0; j < triangles.size(); j++)
        {
            /*const*/ Point* pointA = triangles[j].p0;
            /*const*/ Point* pointB = triangles[j].p1;
            /*const*/ Point* pointC = triangles[j].p2;

            float aby = (pointA->y + pointB->y) / 2;
            float abz;
            if (pointA->z == -1 && pointB->z == -1) abz = -1;
            else if (pointA->z == -1) abz = pointB->z;
            else if (pointB->z == -1) abz = pointA->z;
            else abz = (pointA->z + pointB->z) / 2;
            Point maybePointAB(r, aby, abz, color);
            int indexAB = -1;
            for (int k = numberOfOldPoints; k < points.size(); k++)
            {
                if (maybePointAB == points[k])
                {
                    indexAB = k;
                    break;
                }
            }
            if (indexAB == -1)
            {
                indexAB = points.size();
                points += maybePointAB;
                if (maybePointAB.z == 0) zeros += points.getPointerTo(indexAB);
            }
            Point* pointAB = points.getPointerTo(indexAB);

            float bcy = (pointB->y + pointC->y) / 2;
            float bcz;
            if (pointB->z == -1 && pointC->z == -1) bcz = -1;
            else if (pointB->z == -1) bcz = pointC->z;
            else if (pointC->z == -1) bcz = pointB->z;
            else bcz = (pointB->z + pointC->z) / 2;
            Point maybePointBC(r, bcy, bcz, color);
            int indexBC = -1;
            for (int k = numberOfOldPoints; k < points.size(); k++)
            {
                if (maybePointBC == points[k])
                {
                    indexBC = k;
                    break;
                }
            }
            if (indexBC == -1)
            {
                indexBC = points.size();
                points += maybePointBC;
                if (maybePointBC.z == 0) zeros += points.getPointerTo(indexBC);
            }
            Point* pointBC = points.getPointerTo(indexBC);

            float acy = (pointA->y + pointC->y) / 2;
            float acz;
            if (pointA->z == -1 && pointC->z == -1) acz = -1;
            else if (pointA->z == -1) acz = pointC->z;
            else if (pointC->z == -1) acz = pointA->z;
            else acz = (pointA->z + pointC->z) / 2;
            Point maybePointAC(r, acy, acz, color);
            int indexAC = -1;
            for (int k = numberOfOldPoints; k < points.size(); k++)
            {
                if (maybePointAC == points[k])
                {
                    indexAC = k;
                    break;
                }
            }
            if (indexAC == -1)
            {
                indexAC = points.size();
                points += maybePointAC;
                if (maybePointAC.z == 0) zeros += points.getPointerTo(indexAC);
            }
            Point* pointAC = points.getPointerTo(indexAC);

            newTriangles += FrameTriangle(pointA, pointAB, pointAC);
            newTriangles += FrameTriangle(pointB, pointBC, pointAB);
            newTriangles += FrameTriangle(pointC, pointAC, pointBC);
            newTriangles += FrameTriangle(pointAB, pointBC, pointAC);
        }

        triangles = newTriangles;
    }

    // Fix the wrap around points in triangles.
    for (int i = 0; i < triangles.size(); i++)
    {
        const Point* pointA = triangles[i].p0;
        const Point* pointB = triangles[i].p1;
        const Point* pointC = triangles[i].p2;

        if (equalULP(pointA->z, 2 * _PI))
        {
            Point replacementPointA(pointA->x, pointA->y, 0);
            for (int k = 0; k < zeros.size(); k++)
            {
                if (replacementPointA == *(zeros[k]))
                {
                    triangles[i].p0 = zeros[k];
                    break;
                }
            }
        }

        if (equalULP(pointB->z, 2 * _PI))
        {
            Point replacementPointB(pointB->x, pointB->y, 0);
            for (int k = 0; k < zeros.size(); k++)
            {
                if (replacementPointB == *(zeros[k]))
                {
                    triangles[i].p1 = zeros[k];
                    break;
                }
            }
        }

        if (equalULP(pointC->z, 2 * _PI))
        {
            Point replacementPointC(pointC->x, pointC->y, 0);
            for (int k = 0; k < zeros.size(); k++)
            {
                if (replacementPointC == *(zeros[k]))
                {
                    triangles[i].p2 = zeros[k];
                    break;
                }
            }
        }
    }

    // Generate line segments for the frame
    for (int i = 0; i < triangles.size(); i++)
    {
        FrameLine lineA(triangles[i].p0, triangles[i].p1);
        FrameLine lineB(triangles[i].p1, triangles[i].p2);
        FrameLine lineC(triangles[i].p2, triangles[i].p0);

        bool hasLineA = false;
        for (int j = 0; j < lines.size(); j++)
        {
            if (lineA == lines[j])
            {
                hasLineA = true;
                break;
            }
        }
        if (!hasLineA) lines += lineA;

        bool hasLineB = false;
        for (int j = 0; j < lines.size(); j++)
        {
            if (lineB == lines[j])
            {
                hasLineB = true;
                break;
            }
        }
        if (!hasLineB) lines += lineB;

        bool hasLineC = false;
        for (int j = 0; j < lines.size(); j++)
        {
            if (lineC == lines[j])
            {
                hasLineC = true;
                break;
            }
        }
        if (!hasLineC) lines += lineC;
    }

    // Add some dents to make it more assteroid-ish
    float dent = r / (d * (n + 1));
    for (int i = 0; i < points.size(); i++)
    {
        if (rand() % 3 == 0)
        {
            points[i].x -= dent;
        }
        else if (rand() % 3 == 1)
        {
            points[i].x += dent;
        }
    }

    // Turn the points, into points in the Cartesian Coordinate System
    for (int i = 0; i < points.size(); i++)
    {
        float x = points[i].x;
        float y = points[i].y;
        float z = points[i].z;
        points[i].x = x * sin(y) * cos(z);
        points[i].y = x * sin(y) * sin(z);
        points[i].z = x * cos(y);
    }
}


void initializeBorderFrame(Frame & frame, float r, float n, Color color)

{
    // Similar to initializeAsteroidFrame, but no dents and the triangle
    // normals point toward the center.

    Array<Point> & points = frame.points;
    Array<Point*> zeros;
    Array<FrameTriangle> & triangles = frame.triangles;
    Array<FrameLine> & lines = frame.lines;

    int pointCapacity = 0;
    int j = 1;
    for (int i = 0; i < n; i++)
    {
        j *= 2;
    }
    int i = 0;
    for (; i < j; i++)
    {
        pointCapacity += (1 + 4 * i) * 2;
    }
    pointCapacity += 1 + 4 * i;
    points.setCapacity(pointCapacity);

    // Adding initial points and triangles
    points += Point(r, 0, -1, color); // [0] top point
    points += Point(r, _PI / 2, 0, color); // [1] azimouth angle 0 point
    points += Point(r, _PI / 2, _PI / 2, color);
    points += Point(r, _PI / 2, _PI, color);
    points += Point(r, _PI / 2, 3 * _PI / 2, color);
    points += Point(r, _PI / 2, 2 * _PI, color); // [5] wrap around point
    points += Point(r, _PI, -1, color); // [6] bottom point

    // Add the first azimouth angle 0 point
    zeros += points.getPointerTo(1);

    // Starting triangles
    triangles += FrameTriangle(&(points[0]), &(points[2]), &(points[1]));
    triangles += FrameTriangle(&(points[6]), &(points[1]), &(points[2]));
    triangles += FrameTriangle(&(points[0]), &(points[3]), &(points[2]));
    triangles += FrameTriangle(&(points[6]), &(points[2]), &(points[3]));
    triangles += FrameTriangle(&(points[0]), &(points[4]), &(points[3]));
    triangles += FrameTriangle(&(points[6]), &(points[3]), &(points[4]));
    triangles += FrameTriangle(&(points[0]), &(points[5]), &(points[4]));
    triangles += FrameTriangle(&(points[6]), &(points[4]), &(points[5]));

    // Split up the triangles into smaller triangles to it more spherical
    for (int i = 0; i < n; i++)
    {
        int numberOfOldPoints = points.size();
        Array<FrameTriangle> newTriangles;

        for (int j = 0; j < triangles.size(); j++)
        {
            /*const*/ Point* pointA = triangles[j].p0;
            /*const*/ Point* pointB = triangles[j].p1;
            /*const*/ Point* pointC = triangles[j].p2;

            float aby = (pointA->y + pointB->y) / 2;
            float abz;
            if (pointA->z == -1 && pointB->z == -1) abz = -1;
            else if (pointA->z == -1) abz = pointB->z;
            else if (pointB->z == -1) abz = pointA->z;
            else abz = (pointA->z + pointB->z) / 2;
            Point maybePointAB(r, aby, abz, color);
            int indexAB = -1;
            for (int k = numberOfOldPoints; k < points.size(); k++)
            {
                if (maybePointAB == points[k])
                {
                    indexAB = k;
                    break;
                }
            }
            if (indexAB == -1)
            {
                indexAB = points.size();
                points += maybePointAB;
                if (maybePointAB.z == 0) zeros += points.getPointerTo(indexAB);
            }
            Point* pointAB = points.getPointerTo(indexAB);

            float bcy = (pointB->y + pointC->y) / 2;
            float bcz;
            if (pointB->z == -1 && pointC->z == -1) bcz = -1;
            else if (pointB->z == -1) bcz = pointC->z;
            else if (pointC->z == -1) bcz = pointB->z;
            else bcz = (pointB->z + pointC->z) / 2;
            Point maybePointBC(r, bcy, bcz, color);
            int indexBC = -1;
            for (int k = numberOfOldPoints; k < points.size(); k++)
            {
                if (maybePointBC == points[k])
                {
                    indexBC = k;
                    break;
                }
            }
            if (indexBC == -1)
            {
                indexBC = points.size();
                points += maybePointBC;
                if (maybePointBC.z == 0) zeros += points.getPointerTo(indexBC);
            }
            Point* pointBC = points.getPointerTo(indexBC);

            float acy = (pointA->y + pointC->y) / 2;
            float acz;
            if (pointA->z == -1 && pointC->z == -1) acz = -1;
            else if (pointA->z == -1) acz = pointC->z;
            else if (pointC->z == -1) acz = pointA->z;
            else acz = (pointA->z + pointC->z) / 2;
            Point maybePointAC(r, acy, acz, color);
            int indexAC = -1;
            for (int k = numberOfOldPoints; k < points.size(); k++)
            {
                if (maybePointAC == points[k])
                {
                    indexAC = k;
                    break;
                }
            }
            if (indexAC == -1)
            {
                indexAC = points.size();
                points += maybePointAC;
                if (maybePointAC.z == 0) zeros += points.getPointerTo(indexAC);
            }
            Point* pointAC = points.getPointerTo(indexAC);

            newTriangles += FrameTriangle(pointA, pointAB, pointAC);
            newTriangles += FrameTriangle(pointB, pointBC, pointAB);
            newTriangles += FrameTriangle(pointC, pointAC, pointBC);
            newTriangles += FrameTriangle(pointAB, pointBC, pointAC);
        }

        triangles = newTriangles;
    }

    // Fix the wrap around points in triangles.
    for (int i = 0; i < triangles.size(); i++)
    {
        const Point* pointA = triangles[i].p0;
        const Point* pointB = triangles[i].p1;
        const Point* pointC = triangles[i].p2;

        if (equalULP(pointA->z, 2 * _PI))
        {
            Point replacementPointA(pointA->x, pointA->y, 0);
            for (int k = 0; k < zeros.size(); k++)
            {
                if (replacementPointA == *(zeros[k]))
                {
                    triangles[i].p0 = zeros[k];
                    break;
                }
            }
        }

        if (equalULP(pointB->z, 2 * _PI))
        {
            Point replacementPointB(pointB->x, pointB->y, 0);
            for (int k = 0; k < zeros.size(); k++)
            {
                if (replacementPointB == *(zeros[k]))
                {
                    triangles[i].p1 = zeros[k];
                    break;
                }
            }
        }

        if (equalULP(pointC->z, 2 * _PI))
        {
            Point replacementPointC(pointC->x, pointC->y, 0);
            for (int k = 0; k < zeros.size(); k++)
            {
                if (replacementPointC == *(zeros[k]))
                {
                    triangles[i].p2 = zeros[k];
                    break;
                }
            }
        }
    }

    // Generate line segments for the frame
    for (int i = 0; i < triangles.size(); i++)
    {
        FrameLine lineA(triangles[i].p0, triangles[i].p1);
        FrameLine lineB(triangles[i].p1, triangles[i].p2);
        FrameLine lineC(triangles[i].p2, triangles[i].p0);

        bool hasLineA = false;
        for (int j = 0; j < lines.size(); j++)
        {
            if (lineA == lines[j])
            {
                hasLineA = true;
                break;
            }
        }
        if (!hasLineA) lines += lineA;

        bool hasLineB = false;
        for (int j = 0; j < lines.size(); j++)
        {
            if (lineB == lines[j])
            {
                hasLineB = true;
                break;
            }
        }
        if (!hasLineB) lines += lineB;

        bool hasLineC = false;
        for (int j = 0; j < lines.size(); j++)
        {
            if (lineC == lines[j])
            {
                hasLineC = true;
                break;
            }
        }
        if (!hasLineC) lines += lineC;
    }

    // Turn the points, into points in the Cartesian Coordinate System
    for (int i = 0; i < points.size(); i++)
    {
        float x = points[i].x;
        float y = points[i].y;
        float z = points[i].z;
        points[i].x = x * sin(y) * cos(z);
        points[i].y = x * sin(y) * sin(z);
        points[i].z = x * cos(y);
    }
}


Entity* createShip(Color color0, Color color1, Color color2)
{
    Entity * ship = new Entity();
    ship->typeID = ENTITY_ID_SHIP;
    ship->boundingRadius = 2.05;
    initializeShipFrame(ship->frame, color0, color1, color2);
    ship->mass = 1;
    ship->drawProperties |= DRAW_DISTANCE_SHADING_OFF;
    return ship;
}


Entity * createSaucer(Color color0, Color color1)
{
    Entity * saucer = new Entity();
    saucer->typeID = ENTITY_ID_SAUCER;
    saucer->boundingRadius = 5;
    initializeSaucerFrame(saucer->frame, 5, color0, color1);
    saucer->mass = 2;
    ///saucer->drawProperties |= DRAW_DISTANCE_SHADING_OFF;
    return saucer;
}


Entity* createBullet(Color color)
{
    Entity * bullet = new Entity();
    bullet->boundingRadius = 0.5;
    bullet->drawProperties |= DRAW_DISTANCE_SHADING_OFF;
    initializeBulletFrame(bullet->frame, color);
    bullet->mass = 0.05;
    return bullet;
}


Entity* createLongBullet(Color color)
{
    Entity * bullet = new Entity();
    bullet->boundingRadius = 1.55;
    bullet->drawProperties |= DRAW_DISTANCE_SHADING_OFF;
    initializeLongBulletFrame(bullet->frame, color);
    bullet->mass = 0.25;
    return bullet;
}


Entity* createAsteroid(unsigned size, Color color)
{
    Entity * asteroid = new Entity();
    float r, n, d;
    switch (size)
    {
    case 0:
    {
        r = 3; n = 1; d = 1.5;
        asteroid->mass = 1;
        asteroid->health = 0;
        break;
    }
    case 1:
    {
        r = 5; n = 1; d = 2.5;
        asteroid->mass = 2;
        asteroid->health = 1;
        break;
    }
    case 2:
    {
        r = 9; n = 2; d = 3;
        asteroid->mass = 4;
        asteroid->health = 2;
        break;
    }
    default:
    {
        r = 17; n = 3; d = 4.5;
        asteroid->mass = 8;
        asteroid->health = 3;
        break;
    }
    }

    asteroid->typeID = ENTITY_ID_ASTEROID;
    asteroid->boundingRadius = r + r / (2 * (d + 1));
    initializeAsteroidFrame(asteroid->frame, r, n, d, color);

    return asteroid;
}


Entity* createBorder(float r, float n, Color color)
{
    Entity * border = new Entity();
    border->typeID = ENTITY_ID_BORDER;
    border->boundingRadius = r;

    // Unlike other entities, mass isn't used for border collisions, so...
    // in this case mass is the inner bounding radius. It represents biggest
    // sphere that could fit in this border withought crossing outside.
    border->mass = cos(_PI / (4 * powerOf(2, n))) * r;
    initializeBorderFrame(border->frame, r, n, color);

    border->drawProperties = DRAW_LINES;

    return border;
}


Entity* createTrail(unsigned segments, Color first, Color last)
{
    Entity * trail = new Entity();
    trail->typeID = ENTITY_ID_NONE;
    trail->collidable = false;
    trail->drawProperties = DRAW_POINTS;// DRAW_LINES;
    Frame & frame = trail->frame;
    frame.points.setCapacity(segments + 1);
    for (int i = 0; i <= segments; i++)
    {
        Color c = first.getFade(1.0 * (segments - i) / segments) + last.getFade(1.0 * i / segments);
        frame.points += Point(0, 0, 0, c);
    }/*
    for (int i = 0; i < segments; i++)
    {
        Point * one = frame.points.getPointerTo(i);
        Point * two = frame.points.getPointerTo(i + 1);
        frame.lines += FrameLine(one, two);
    }*/
    return trail;
}


Entity* createLaser(unsigned nPoints, float nGap, Color first, Color last)
{
    Entity * laser = new Entity();
    laser->typeID = ENTITY_ID_NONE;
    laser->collidable = false;
    laser->drawProperties = DRAW_POINTS;
    Frame & frame = laser->frame;
    frame.points.setCapacity(nPoints);
    for (int i = 0; i < nPoints; i++)
    {
        Color c = first.getFade(1.0 * (nPoints - i) / nPoints) + last.getFade(1.0 * i / nPoints);
        frame.points += Point(0, 0, 0, c);
    }
    return laser;
}


Color randomColorFrom7()
{

    switch (rand() % 7)
    {
    case 0:
    {
        return COLOR_WHITE;
        break;
    }
    case 1:
    {
        return COLOR_RED;
        break;
    }
    case 2:
    {
        return COLOR_GREEN;
        break;
    }
    case 3:
    {
        return COLOR_BLUE;
        break;
    }
    case 4:
    {
        return COLOR_TEAL;
        break;
    }
    case 5:
    {
        return COLOR_MAGENTA;
        break;
    }
    default:
    {
        return COLOR_YELLOW;
        break;
    }
    }
}


Vector randomOrthogonalVector(const Vector & v)
{
    Vector offV(rand(), rand(), rand());
    while (offV.isParallel(v))
    {
        offV.x = rand() - (RAND_MAX / 2);
        offV.y = rand() - (RAND_MAX / 2);
        offV.z = rand() - (RAND_MAX / 2);
    }
    Vector orthogonal = v.crossProduct(offV);
    orthogonal.normalize();
    return orthogonal;
}


Entity* createFlower(const Vector & v, unsigned numberOfLines)
{
    Entity * flower = new Entity();
    flower->typeID = ENTITY_ID_FLOWER;
    flower->collidable = false;
    flower->drawProperties = DRAW_LINES;

    // So the plan is to have a bunch of points, half at the origin, and half
    // going out radialy perpendicular to the given vector v.
    Frame & frame = flower->frame;
    unsigned numberOfPoints = numberOfLines * 2;
    frame.points.setCapacity(numberOfPoints);
    for (int i = 0; i < numberOfPoints; i++)
    {
        frame.points += Point(0, 0, 0, randomColorFrom7());
    }

    // Great, now I need a vector perpendicular to the given vector v. This
    // will be used to make the lines.
    Vector orthogonal = randomOrthogonalVector(v);
    orthogonal *= 0.1;

    // Ok, all the points are created and colored. Now to pick the angle their
    // lines will be seperated by.
    float rotationAngle = 2 * _PI / numberOfLines;
    Quaternion rotation;
    rotation.addRotation(v, rotationAngle);

    // Spreading the points and making the lines.
    for (int i = 1; i < numberOfPoints; i += 2)
    {
        frame.points[i] += orthogonal;
        orthogonal *= rotation.getMatrix();
    }
    for (int i = 0; i < numberOfPoints; i += 2)
    {
        frame.lines += FrameLine(frame.points.getPointerTo(i), frame.points.getPointerTo(i + 1));
    }
    return flower;
}


Entity* spawnShipBullet(const Entity * entity, Color color, float speed)
{
    Entity * bullet = createLongBullet(color);
    Vector v(entity->boundingRadius + bullet->boundingRadius + 0.1, 0, 0);
    v *= entity->orientation.getMatrix();
    bullet->locationPoint = entity->locationPoint + v;
    bullet->orientation = entity->orientation;
    v.normalize();
    bullet->velocity = v * speed; // + entity->velocity; Adding entity velocity makes it too hard to aim
    bullet->typeID = ENTITY_ID_SHIP_BULLET;
    return bullet;
}


Entity* spawnSaucerBullet(const Entity * source, const Point & target, Color color, float speed)
{
    Entity * bullet = createBullet(color);

    // Set bullet's start point
    Vector v(0, source->boundingRadius + bullet->boundingRadius + 0.1, 0);
    v *= source->orientation.getMatrix();
    bullet->locationPoint = source->locationPoint + v;

    // Set bulletr's velocity
    bullet->velocity = target - bullet->locationPoint;
    bullet->velocity.normalize();
    bullet->velocity *= speed;

    // Set bullet's orientation, meh... no need
    ///Vector currentlyFacingDirection = bullet->orientation.getForwardVector();
    ///float angleToGoalFacingDirection = currentlyFacingDirection.angleBetween(bullet->velocity);
    ///Vector orthogonal = currentlyFacingDirection.crossProduct(bullet->velocity);
    ///bullet->orientation.addRotation(orthogonal, angleToGoalFacingDirection);
    
    bullet->typeID = ENTITY_ID_SAUCER_BULLET;
    return bullet;
}


void updateTrailFrame(Frame & frame, const Point & location)
{
    for (int i = frame.points.size() - 1; i > 0; i--)
    {
        frame.points[i].x = frame.points[i - 1].x;
        frame.points[i].y = frame.points[i - 1].y;
        frame.points[i].z = frame.points[i - 1].z;
    }
    frame.points[0].x = location.x;
    frame.points[0].y = location.y;
    frame.points[0].z = location.z;
}


void updateRTrailFrame(Frame & frame, const Entity * entity)
{
    for (int i = frame.points.size() - 1; i > 0; i--)
    {
        frame.points[i].x = frame.points[i - 1].x;
        frame.points[i].y = frame.points[i - 1].y;
        frame.points[i].z = frame.points[i - 1].z;
    }

    Point p(-1.5, 0, 1);

    Matrix rotationMatrix = entity->orientation.getMatrix();
    p *= rotationMatrix;

    Matrix locationMatrix;
    locationMatrix.addTranslation(entity->locationPoint.x, entity->locationPoint.y, entity->locationPoint.z);
    p *= locationMatrix;

    frame.points[0].x = p.x;
    frame.points[0].y = p.y;
    frame.points[0].z = p.z;
}


void updateLTrailFrame(Frame & frame, const Entity * entity)
{
    for (int i = frame.points.size() - 1; i > 0; i--)
    {
        frame.points[i].x = frame.points[i - 1].x;
        frame.points[i].y = frame.points[i - 1].y;
        frame.points[i].z = frame.points[i - 1].z;
    }

    Point p(-1.5, 0, -1);

    Matrix rotationMatrix = entity->orientation.getMatrix();
    p *= rotationMatrix;

    Matrix locationMatrix;
    locationMatrix.addTranslation(entity->locationPoint.x, entity->locationPoint.y, entity->locationPoint.z);
    p *= locationMatrix;

    frame.points[0].x = p.x;
    frame.points[0].y = p.y;
    frame.points[0].z = p.z;
}


void updateLaserFrame(Frame & frame, float nGap, const Entity * entity)
{
    Vector direction(entity->boundingRadius, 0, 0);
    direction *= entity->orientation.getMatrix();

    float currentX = entity->locationPoint.x + direction.x;
    float currentY = entity->locationPoint.y + direction.y;
    float currentZ = entity->locationPoint.z + direction.z;

    direction.normalize();
    direction *= nGap;
    for (int i = 0; i < frame.points.size(); i++)
    {
        currentX += direction.x;
        currentY += direction.y;
        currentZ += direction.z;
        frame.points[i].x = currentX;
        frame.points[i].y = currentY;
        frame.points[i].z = currentZ;
    }
}


void updateFlowerFrame(Frame & frame)
{
    Array<Point> & points = frame.points;
    for (int i = 0; i < points.size(); i += 2)
    {
        Vector v = points[i + 1] - points[i];
        v.normalize();
        points[i] += v * 0.25;
        points[i + 1] += v * 0.5; // HARD_SPEED_LIMIT from Game.h
    }
}


// collision


bool fasterLineTriangleIntersect(const Line & line, const Triangle & triangle, Point & location)
{
    // Method:
    // First check if the line intersects the plane the triangle is on.
    // Then, check if the line intersects the triangle, not just the plane.
    // Use the...
    // Moller-Trumbore algorithm
    // -------------------------------------------------------------------------
    // NOTE: '.' will represent the dot product when in an equation.
    // 
    // P = w * A + u * B + v * C; w + u + v = 1    <-- barycentric coordinates
    // 
    // P = point on plane
    // w, u, v = scalars that all add up to 1
    // A, B, C = verticies of the triangle (p0, p1, p2)
    // 
    // P = O + t * D    <-- line equation
    // 
    // P = point on plane
    // O = starting point of line (p0)
    // t = scalar
    // D = direction vector of the line (line.p1 - line.p0)
    // 
    // 
    // First use the scalar relation to rewrite the barycentric triangle
    // coordinates equation in terms of two scalars
    // 
    // P = (1 - u - v) * A + u * B + v * C
    //   = A + u * (B - A) + v * (C - A)
    // 
    // Combile this equation with the line equation
    // 
    // O + t * D  = A + u * (B - A) + v * (C - A)
    // O - A  = t * (-D) + u * (B - A) + v * (C - A)
    // 
    // Write it in matrix form
    // 
    //                      [ t ]
    // [ -D, B - A, C - A ] [ u ] = O - A
    //                      [ v ]
    // 
    // Apply Cramer's Rule (det stands for determinant)
    // 
    // [ t ]                         [ det( T, E, F) ]
    // [ u ] = (1 / det(-D, E, F)) * [ det(-D, T, F) ]
    // [ v ]                         [ det(-D, E, T) ]
    // 
    // E = B - A
    // F = C - A
    // 
    // [ t ]                 [ Q . F ]
    // [ u ] = 1 / (P . E) * [ P . T ]
    // [ v ]                 [ Q . D ]
    // 
    // P = D cross F
    // Q = T cross E
    // 
    // t gives input into "P = O + t * D" that gives P, the intersect point
    // u & v are used to check if the point lies in the triangle
    // (u >= 0 && v >= 0 && u + v <= 1)

    // Point O
    const float & O_x = line.p0.x;
    const float & O_y = line.p0.y;
    const float & O_z = line.p0.z;

    // Point Z
    const float & Z_x = line.p1.x;
    const float & Z_y = line.p1.y;
    const float & Z_z = line.p1.z;

    // Point A
    const float & A_x = triangle.p0.x;
    const float & A_y = triangle.p0.y;
    const float & A_z = triangle.p0.z;

    // Point B
    const float & B_x = triangle.p1.x;
    const float & B_y = triangle.p1.y;
    const float & B_z = triangle.p1.z;

    // Point C
    const float & C_x = triangle.p2.x;
    const float & C_y = triangle.p2.y;
    const float & C_z = triangle.p2.z;

    // Vector T
    float T_x = O_x - A_x;
    float T_y = O_y - A_y;
    float T_z = O_y - A_y;

    // Vector W
    float W_x = Z_x - A_x;
    float W_y = Z_y - A_y;
    float W_z = Z_y - A_y;

    float temp0_x = B_x - A_x;
    float temp0_y = B_y - A_y;
    float temp0_z = B_z - A_z;
    float temp1_x = C_x - B_x;
    float temp1_y = C_y - B_y;
    float temp1_z = C_z - B_z;
    float temp2_x = temp0_y * temp1_z - temp0_z - temp1_y;
    float temp2_y = temp0_z * temp1_x - temp0_x - temp1_z;
    float temp2_z = temp0_x * temp1_y - temp0_y - temp1_x;
    float mag = sqrt(temp2_x * temp2_x + temp2_y * temp2_y + temp2_z * temp2_z);
    // Vector N
    float N_x = temp2_x / mag;
    float N_y = temp2_y / mag;
    float N_z = temp2_z / mag;

    // dot0 = N dot T
    float dot0 = N_x * T_x + N_y * T_y + N_z * T_z;

    // dot1 = N dot W 
    float dot1 = N_x * W_x + N_y * W_y + N_z * W_z;

    // First check if the line could intersect the plane the triangle is on.
    if (!equalULP(dot0, 0) || !equalULP(dot1, 0))
    {
        // If either dot product is equal to zero, one end of the line is on the
        // plane, so it's intersecting.
        if (SIGN(dot0) == SIGN(dot1))
        {
            // If the dot products have the same sign, then both points are on
            // one side of the plane and the line doesn't intersect the plane
            // which the triangle is on.
            return false;
        }
    }

    // Vector D
    float D_x = O_x - Z_x;
    float D_y = O_y - Z_y;
    float D_z = O_y - Z_y;

    // Vector E
    float E_x = B_x - A_x;
    float E_y = B_y - A_y;
    float E_z = B_y - A_y;

    // Vector F
    float F_x = C_x - A_x;
    float F_y = C_y - A_y;
    float F_z = C_y - A_y;

    // Vector P = D cross F
    float P_x = D_y * F_z - D_z - F_y;
    float P_y = D_z * F_x - D_x - F_z;
    float P_z = D_x * F_y - D_y - F_x;

    // Vector Q = T cross F
    float Q_x = T_y * F_z - T_z - F_y;
    float Q_y = T_z * F_x - T_x - F_z;
    float Q_z = T_x * F_y - T_y - F_x;

    // pretty sure I don't need an "== 0" check
    float PdotE = P_x * E_x + P_y * E_y + P_z * E_z;

    float QdotF = Q_x * F_x + Q_y * F_y + Q_z * F_z;
    float PdotT = P_x * T_x + P_y * T_y + P_z * T_z;
    float QdotD = Q_x * D_x + Q_y * D_y + Q_z * D_z;

    float t = QdotF / PdotE;
    float u = PdotT / PdotE;
    float v = QdotD / PdotE;

    if (u >= 0 && v >= 0 && u + v <= 1)
    {
        // P = O + t * D
        location.x = O_x + (D_x * T_x);
        location.y = O_y + (D_y * T_y);
        location.z = O_z + (D_z * T_z);
        return true;
    }
    else
    {
        return false; // Line doesn't intersect the triangle
    }
}


bool pointInsideShapeCheck(const Point & point, const Shape & shape, float length)
{
    int crossCount = 0;
    for (int i = 0; i < shape.triangles.size(); i++)
    {
        Line temp = Line(point, Point(point.x + length, point.y, point.z));
        if (fasterLineTriangleIntersect(temp, shape.triangles[i], Point()))
        {
            crossCount++;
        }
    }
    if (crossCount % 2 == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}


void hardCapSpeed(Entity * entity, float limit)
{
    Vector & velocity = entity->velocity;
    if (velocity.magnitude() > limit)
    {
        velocity.normalize();
        velocity *= limit;
    }
}


void softCapSpeed(Entity * entity, float limit, float deceleration)
{
    Vector & velocity = entity->velocity;
    if (velocity.magnitude() > limit)
    {
        Vector decelerationV = velocity;
        decelerationV.normalize();
        decelerationV *= deceleration;
        velocity -= decelerationV;
    }
}


// unused

void createBorderAlt(Array<Entity*> & entities, float r, float n, float m, Color color)
{
    // Starting frame
    // This will be used to generate a single large sphere frame
    // -------------------------------------------------------------------------
    Frame frame;
    {
        Array<Point> & points = frame.points;
        Array<FrameTriangle> & triangles = frame.triangles;
        Array<FrameLine> & lines = frame.lines;

        // First things first...
        int pointCapacity = 0;
        int j = 1;
        for (int i = 0; i < n; i++)
        {
            j *= 2;
        }
        int i = 0;
        for (; i < j; i++)
        {
            pointCapacity += (1 + 4 * i) * 2;
        }
        pointCapacity += 1 + 4 * i;
        points.setCapacity(pointCapacity);

        // Adding initial points and triangles
        points += Point(r, 0, -1, color); // [0] top point
        points += Point(r, _PI / 2, 0, color); // [1] azimouth angle 0 point
        points += Point(r, _PI / 2, _PI / 2, color);
        points += Point(r, _PI / 2, _PI, color);
        points += Point(r, _PI / 2, 3 * _PI / 2, color);
        points += Point(r, _PI / 2, 2 * _PI, color); // [5] wrap around point
        points += Point(r, _PI, -1, color); // [6] bottom point

        // Starting triangles, (facing the oposite dirction as in initializeAsteroidFrame
        // so that all normals face towards the center)
        triangles += FrameTriangle(&(points[0]), &(points[2]), &(points[1]));
        triangles += FrameTriangle(&(points[6]), &(points[1]), &(points[2]));
        triangles += FrameTriangle(&(points[0]), &(points[3]), &(points[2]));
        triangles += FrameTriangle(&(points[6]), &(points[2]), &(points[3]));
        triangles += FrameTriangle(&(points[0]), &(points[4]), &(points[3]));
        triangles += FrameTriangle(&(points[6]), &(points[3]), &(points[4]));
        triangles += FrameTriangle(&(points[0]), &(points[5]), &(points[4]));
        triangles += FrameTriangle(&(points[6]), &(points[4]), &(points[5]));

        // Split up the triangles into smaller triangles to it more spherical
        for (int i = 0; i < n; i++)
        {
            int numberOfOldPoints = points.size();
            Array<FrameTriangle> newTriangles;

            for (int j = 0; j < triangles.size(); j++)
            {
                Point* pointA = triangles[j].p0;
                Point* pointB = triangles[j].p1;
                Point* pointC = triangles[j].p2;

                float aby = (pointA->y + pointB->y) / 2;
                float abz;
                if (pointA->z == -1 && pointB->z == -1) abz = -1;
                else if (pointA->z == -1) abz = pointB->z;
                else if (pointB->z == -1) abz = pointA->z;
                else abz = (pointA->z + pointB->z) / 2;
                Point maybePointAB(r, aby, abz, color);
                int indexAB = -1;
                for (int k = numberOfOldPoints; k < points.size(); k++)
                {
                    if (maybePointAB == points[k])
                    {
                        indexAB = k;
                        break;
                    }
                }
                if (indexAB == -1)
                {
                    indexAB = points.size();
                    points += maybePointAB;
                }
                Point* pointAB = points.getPointerTo(indexAB);

                float bcy = (pointB->y + pointC->y) / 2;
                float bcz;
                if (pointB->z == -1 && pointC->z == -1) bcz = -1;
                else if (pointB->z == -1) bcz = pointC->z;
                else if (pointC->z == -1) bcz = pointB->z;
                else bcz = (pointB->z + pointC->z) / 2;
                Point maybePointBC(r, bcy, bcz, color);
                int indexBC = -1;
                for (int k = numberOfOldPoints; k < points.size(); k++)
                {
                    if (maybePointBC == points[k])
                    {
                        indexBC = k;
                        break;
                    }
                }
                if (indexBC == -1)
                {
                    indexBC = points.size();
                    points += maybePointBC;
                }
                Point* pointBC = points.getPointerTo(indexBC);

                float acy = (pointA->y + pointC->y) / 2;
                float acz;
                if (pointA->z == -1 && pointC->z == -1) acz = -1;
                else if (pointA->z == -1) acz = pointC->z;
                else if (pointC->z == -1) acz = pointA->z;
                else acz = (pointA->z + pointC->z) / 2;
                Point maybePointAC(r, acy, acz, color);
                int indexAC = -1;
                for (int k = numberOfOldPoints; k < points.size(); k++)
                {
                    if (maybePointAC == points[k])
                    {
                        indexAC = k;
                        break;
                    }
                }
                if (indexAC == -1)
                {
                    indexAC = points.size();
                    points += maybePointAC;
                }
                Point* pointAC = points.getPointerTo(indexAC);

                newTriangles += FrameTriangle(pointA, pointAB, pointAC);
                newTriangles += FrameTriangle(pointB, pointBC, pointAB);
                newTriangles += FrameTriangle(pointC, pointAC, pointBC);
                newTriangles += FrameTriangle(pointAB, pointBC, pointAC);
            }

            triangles = newTriangles;
        }
    }

    // Ok, so now triangles holds a bunch of triangles that make up the border.
    // Each one will be further subdivided and turned into an Entity.

    for (int i = 0; i < frame.triangles.size(); i++)
    {
        Entity * borderPiece = new Entity();

        borderPiece->typeID = ENTITY_ID_BORDER;
        borderPiece->drawProperties = DRAW_LINES;

        Array<Point> & points = borderPiece->frame.points;
        Array<FrameTriangle> & triangles = borderPiece->frame.triangles;
        Array<FrameLine> & lines = borderPiece->frame.lines;

        // Figure ould how many pointers are goning to be in all these figures
        int pointCapacity = 0;
        int sumCap = 1 + powerOf(2, m);
        for (int j = 1; j <= sumCap; j++) pointCapacity += j;
        points.setCapacity(pointCapacity);

        points += *(frame.triangles[i].p0);
        points += *(frame.triangles[i].p1);
        points += *(frame.triangles[i].p2);

        triangles += FrameTriangle(points.getPointerTo(0), points.getPointerTo(1), points.getPointerTo(2));

        // Once again Split up the triangles into smaller triangles to it more spherical
        for (int j = 0; j < m; j++)
        {
            int numberOfOldPoints = points.size();
            Array<FrameTriangle> newTriangles;

            for (int k = 0; k < triangles.size(); k++)
            {
                Point* pointA = triangles[k].p0;
                Point* pointB = triangles[k].p1;
                Point* pointC = triangles[k].p2;

                float aby = (pointA->y + pointB->y) / 2;
                float abz;
                if (pointA->z == -1 && pointB->z == -1) abz = -1;
                else if (pointA->z == -1) abz = pointB->z;
                else if (pointB->z == -1) abz = pointA->z;
                else abz = (pointA->z + pointB->z) / 2;
                Point maybePointAB(r, aby, abz, color);
                int indexAB = -1;
                for (int k = numberOfOldPoints; k < points.size(); k++)
                {
                    if (maybePointAB == points[k])
                    {
                        indexAB = k;
                        break;
                    }
                }
                if (indexAB == -1)
                {
                    indexAB = points.size();
                    points += maybePointAB;
                }
                Point* pointAB = points.getPointerTo(indexAB);

                float bcy = (pointB->y + pointC->y) / 2;
                float bcz;
                if (pointB->z == -1 && pointC->z == -1) bcz = -1;
                else if (pointB->z == -1) bcz = pointC->z;
                else if (pointC->z == -1) bcz = pointB->z;
                else bcz = (pointB->z + pointC->z) / 2;
                Point maybePointBC(r, bcy, bcz, color);
                int indexBC = -1;
                for (int k = numberOfOldPoints; k < points.size(); k++)
                {
                    if (maybePointBC == points[k])
                    {
                        indexBC = k;
                        break;
                    }
                }
                if (indexBC == -1)
                {
                    indexBC = points.size();
                    points += maybePointBC;
                }
                Point* pointBC = points.getPointerTo(indexBC);

                float acy = (pointA->y + pointC->y) / 2;
                float acz;
                if (pointA->z == -1 && pointC->z == -1) acz = -1;
                else if (pointA->z == -1) acz = pointC->z;
                else if (pointC->z == -1) acz = pointA->z;
                else acz = (pointA->z + pointC->z) / 2;
                Point maybePointAC(r, acy, acz, color);
                int indexAC = -1;
                for (int k = numberOfOldPoints; k < points.size(); k++)
                {
                    if (maybePointAC == points[k])
                    {
                        indexAC = k;
                        break;
                    }
                }
                if (indexAC == -1)
                {
                    indexAC = points.size();
                    points += maybePointAC;
                }
                Point* pointAC = points.getPointerTo(indexAC);

                newTriangles += FrameTriangle(pointA, pointAB, pointAC);
                newTriangles += FrameTriangle(pointB, pointBC, pointAB);
                newTriangles += FrameTriangle(pointC, pointAC, pointBC);
                newTriangles += FrameTriangle(pointAB, pointBC, pointAC);
            }

            triangles = newTriangles;
        }

        // Generate line segments for the frame
        for (int j = 0; j < triangles.size(); j++)
        {
            FrameLine lineA(triangles[j].p0, triangles[j].p1);
            FrameLine lineB(triangles[j].p1, triangles[j].p2);
            FrameLine lineC(triangles[j].p2, triangles[j].p0);

            bool hasLineA = false;
            for (int k = 0; k < lines.size(); k++)
            {
                if (lineA == lines[k])
                {
                    hasLineA = true;
                    break;
                }
            }
            if (!hasLineA) lines += lineA;

            bool hasLineB = false;
            for (int k = 0; k < lines.size(); k++)
            {
                if (lineB == lines[k])
                {
                    hasLineB = true;
                    break;
                }
            }
            if (!hasLineB) lines += lineB;

            bool hasLineC = false;
            for (int k = 0; k < lines.size(); k++)
            {
                if (lineC == lines[k])
                {
                    hasLineC = true;
                    break;
                }
            }
            if (!hasLineC) lines += lineC;
        }

        // Turn the points, into points in the Cartesian Coordinate System
        for (int j = 0; j < points.size(); j++)
        {
            float x = points[j].x;
            float y = points[j].y;
            float z = points[j].z;
            points[j].x = x * sin(y) * cos(z);
            points[j].y = x * sin(y) * sin(z);
            points[j].z = x * cos(y);
        }

        // Shift points to the origin and add the location point to the entity
        // (average ofthe main 3 of the triangle before it was devided up)
        float averageX = (points[0].x + points[1].x + points[2].x) / 3;
        float averageY = (points[0].y + points[1].y + points[2].y) / 3;
        float averageZ = (points[0].z + points[1].z + points[2].z) / 3;

        borderPiece->locationPoint.x = averageX;
        borderPiece->locationPoint.y = averageY;
        borderPiece->locationPoint.z = averageZ;

        for (int j = 0; j < points.size(); j++)
        {
            points[j].x -= averageX;
            points[j].y -= averageY;
            points[j].z -= averageZ;
        }

        float mag0 = sqrt(points[0].x * points[0].x + points[0].y * points[0].y + points[0].z * points[0].z);
        float mag1 = sqrt(points[1].x * points[1].x + points[1].y * points[1].y + points[1].z * points[1].z);
        float mag2 = sqrt(points[2].x * points[2].x + points[2].y * points[2].y + points[2].z * points[2].z);

        float bounding;
        if (mag0 > mag1) bounding = mag0;
        else bounding = mag1;
        if (bounding < mag2) bounding = mag2;

        borderPiece->boundingRadius = bounding;

        entities += borderPiece;
    }
}


bool lineTriangleIntersect(const Line & line, const Triangle & triangle, Point & location)
{
    // Method:
    // First check if the line intersects the plane the triangle is on.
    // Then, check if the line intersects the triangle, not just the plane.
    // Use the...
    // Moller-Trumbore algorithm
    // -------------------------------------------------------------------------
    // NOTE: '.' will represent the dot product when in an equation.
    // 
    // P = w * A + u * B + v * C; w + u + v = 1    <-- barycentric coordinates
    // 
    // P = point on plane
    // w, u, v = scalars that all add up to 1
    // A, B, C = verticies of the triangle (p0, p1, p2)
    // 
    // P = O + t * D    <-- line equation
    // 
    // P = point on plane
    // O = starting point of line (p0)
    // t = scalar
    // D = direction vector of the line (line.p1 - line.p0)
    // 
    // 
    // First use the scalar relation to rewrite the barycentric triangle
    // coordinates equation in terms of two scalars
    // 
    // P = (1 - u - v) * A + u * B + v * C
    //   = A + u * (B - A) + v * (C - A)
    // 
    // Combile this equation with the line equation
    // 
    // O + t * D  = A + u * (B - A) + v * (C - A)
    // O - A  = t * (-D) + u * (B - A) + v * (C - A)
    // 
    // Write it in matrix form
    // 
    //                      [ t ]
    // [ -D, B - A, C - A ] [ u ] = O - A
    //                      [ v ]
    // 
    // Apply Cramer's Rule (det stands for determinant)
    // 
    // [ t ]                         [ det( T, E, F) ]
    // [ u ] = (1 / det(-D, E, F)) * [ det(-D, T, F) ]
    // [ v ]                         [ det(-D, E, T) ]
    // 
    // E = B - A
    // F = C - A
    // 
    // [ t ]                 [ Q . F ]
    // [ u ] = 1 / (P . E) * [ P . T ]
    // [ v ]                 [ Q . D ]
    // 
    // P = D cross F
    // Q = T cross E
    // 
    // t gives input into "P = O + t * D" that gives P, the intersect point
    // u & v are used to check if the point lies in the triangle
    // (u >= 0 && v >= 0 && u + v <= 1)

    const Point & O = line.p0;
    const Point & Z = line.p1;
    const Point & A = triangle.p0;
    const Point & B = triangle.p1;
    const Point & C = triangle.p2;
    const Vector T = O - A;
    const Vector W = Z - A;

    const Vector N = triangle.getNormal();
    float dot0 = N.dotProduct(T);
    float dot1 = N.dotProduct(W);

    // First check if the line could intersect the plane the triangle is on.
    if (!equalULP(dot0, 0) || !equalULP(dot1, 0))
    {
        // If either dot product is equal to zero, one end of the line is on the
        // plane, so it's intersecting.
        if (SIGN(dot0) == SIGN(dot1))
        {
            // If the dot products have the same sign, then both points are on
            // one side of the plane and the line doesn't intersect the plane
            // which the triangle is on.
            return false;
        }
    }

    const Vector D = O - Z;
    const Vector E = B - A;
    const Vector F = C - A;
    const Vector P = D.crossProduct(F);
    const Vector Q = T.crossProduct(E);

    float PdotE = P.dotProduct(E); // pretty sure I don't need an "== 0" check
    float t = Q.dotProduct(F) / PdotE;
    float u = P.dotProduct(T) / PdotE;
    float v = Q.dotProduct(D) / PdotE;

    if (u >= 0 && v >= 0 && u + v <= 1)
    {
        location = O + (D * t); // P = O + t * D
        return true;
    }
    else
    {
        return false; // Line doesn't intersect the triangle
    }
}


bool shapeShapeIntersect(const Shape & shapeA, const Shape & shapeB, Point & location)
{
    const Array<Line> & lines = shapeA.lines;
    const Array<Triangle> & triangles = shapeB.triangles;
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = 0; j < triangles.size(); j++)
        {
            if (lineTriangleIntersect(lines[i], triangles[j], location)) return true;
        }
    }
    return false;
}


void calculateEntityCollisionsSimple(Array<Entity*> entities)
{
    struct Collision
    {
        Entity * entityA;
        Entity * entityB;
        Point collisionLocation;
    };

    // Holds all the found collisions
    Array<Collision> collisions;

    for (int i = 0; i < entities.size(); i++)
    {
        for (int j = i + 1; j < entities.size(); j++)
        {
            if (i == j) continue;


            // Check if the entities' bounding radii overlap, if they don't collision isn't possilbe.
            float xDifference = entities[i]->locationPoint.x - entities[j]->locationPoint.x;
            float yDifference = entities[i]->locationPoint.y - entities[j]->locationPoint.y;
            float zDifference = entities[i]->locationPoint.z - entities[j]->locationPoint.z;
            float locationDifference = sqrt(xDifference * xDifference + yDifference * yDifference + zDifference * zDifference);
            float sumBoundingRadii = entities[i]->boundingRadius + entities[j]->boundingRadius;

            // There won't be any collisionpoints unless these requirements are met
            if (locationDifference < sumBoundingRadii && entities[i]->collidable && entities[j]->collidable)
            {
                // All the entity intersects between these two
                Array<Point> collisionPoints;

                Shape & shapeA = entities[i]->worldSpaceShape;
                Shape & shapeB = entities[j]->worldSpaceShape;

                // For the sake of speed, we'll only be comparing the smaller
                // entity's lines to the larger entity's faces.
                if (entities[i]->boundingRadius < entities[j]->boundingRadius)
                {
                    for (int k = 0; k < shapeA.lines.size(); k++)
                    {
                        for (int l = 0; l < shapeB.triangles.size(); l++)
                        {
                            Point intersectLocation;
                            if (fasterLineTriangleIntersect(shapeA.lines[k], shapeB.triangles[l], intersectLocation))
                            {
                                collisionPoints += intersectLocation;
                            }
                        }
                    }
                }
                else
                {
                    for (int k = 0; k < shapeB.lines.size(); k++)
                    {
                        for (int l = 0; l < shapeA.triangles.size(); l++)
                        {
                            Point intersectLocation;
                            if (fasterLineTriangleIntersect(shapeB.lines[k], shapeA.triangles[l], intersectLocation))
                            {
                                collisionPoints += intersectLocation;
                            }
                        }
                    }
                }

                // A collision has been found
                if (collisionPoints.size() > 0)
                {
                    // Get an average point from the collected collision poiints
                    float xAverage = 0;
                    float yAverage = 0;
                    float zAverage = 0;
                    for (int k = 0; k < collisionPoints.size(); k++)
                    {
                        xAverage += collisionPoints[k].x;
                        yAverage += collisionPoints[k].y;
                        zAverage += collisionPoints[k].z;
                    }
                    xAverage /= collisionPoints.size();
                    yAverage /= collisionPoints.size();
                    zAverage /= collisionPoints.size();

                    Collision newCollision;
                    newCollision.entityA = *(entities.getPointerTo(i));
                    newCollision.entityB = *(entities.getPointerTo(j));
                    newCollision.collisionLocation = Point(xAverage, yAverage, zAverage);

                    collisions += newCollision;
                }
            }
        }
    }

    // Resolve all the collisions
    for (int i = 0; i < collisions.size(); i++)
    {

        Point & centerOfMassA = collisions[i].entityA->locationPoint;
        Point & centerOfMassB = collisions[i].entityB->locationPoint;

        Vector & velocityA = collisions[i].entityA->velocity;
        Vector & velocityB = collisions[i].entityB->velocity;

        float & massA = collisions[i].entityA->mass;
        float & massB = collisions[i].entityB->mass;

        Point & collisionLocation = collisions[i].collisionLocation;

        Vector normalA = centerOfMassB - centerOfMassA;
        if (equalULP(normalA.magnitude(), 0))
        {
            // Objects are colliding, but their center of mass is in the same
            // location, can be caused by a faulty spawn, to fix, just have
            // them move away in random directions
            normalA.x = rand() - (RAND_MAX / 2);
            normalA.y = rand() - (RAND_MAX / 2);
            normalA.z = rand() - (RAND_MAX / 2);
            normalA.normalize();

            // Oh no, both objects aren't moveing or are moving, but in the same
            // direction!
            if (velocityA == velocityB)
            {
                velocityA += normalA * 0.01;
                velocityB -= normalA * 0.01;
            }
        }
        else
        {
            normalA.normalize();
        }
        Vector normalB = normalA * -1;

        Vector projectionA = velocityA.getProjection(normalA);
        float dotTestA = projectionA.dotProduct(normalA);
        if (dotTestA < 0 || equalULP(dotTestA, 0))
        {
            projectionA.x = 0;
            projectionA.y = 0;
            projectionA.z = 0;
        }
        Vector leftoverA = velocityA - projectionA;

        Vector projectionB = velocityB.getProjection(normalB);
        float dotTestB = projectionB.dotProduct(normalB);
        if (dotTestB < 0 || equalULP(dotTestB, 0))
        {
            projectionB.x = 0;
            projectionB.y = 0;
            projectionB.z = 0;
        }
        Vector leftoverB = velocityB - projectionB;

        Vector momentumA = projectionA * massA;
        Vector velocityReceivedB = momentumA / massB;

        Vector momentumB = projectionB * massB;
        Vector velocityReceivedA = momentumB / massA;

        velocityA = velocityReceivedA + leftoverA;
        velocityB = velocityReceivedB + leftoverB;
    }
}


void calculateBorderCollisionsSimple(Array<Entity*> entities, Entity * border)
{
    for (int i = 0; i < entities.size(); i++)
    {
        if (entities[i]->collidable)
        {
            // Check if the entity's bounding radii overlap, if they don't
            // collision isn't possilbe.
            float xDiff = border->locationPoint.x - entities[i]->locationPoint.x;
            float yDiff = border->locationPoint.y - entities[i]->locationPoint.y;
            float zDiff = border->locationPoint.z - entities[i]->locationPoint.z;
            float locationDifference = sqrt(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);
            // In the special case of a border entity, itsmass represents its inner
            // bounding radius and the its boudingRadius represents its outter 
            // bounding radius
            float averageBoundingRadius = (border->mass + border->boundingRadius) / 2;
            float minCollidingRadius = averageBoundingRadius - entities[i]->boundingRadius;
            if (locationDifference > minCollidingRadius)
            {
                // Ok, looks like collision could be possibe, but before checking
                // all the points, make sure the entity is moving toward the border.
                // If it's not, it might have already collided.
                const float & V_x = entities[i]->velocity.x;
                const float & V_y = entities[i]->velocity.y;
                const float & V_z = entities[i]->velocity.z;
                float nMag = sqrt(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);
                // Normal Vector from entity location to the center of the border
                // shape
                float N_x = xDiff / nMag;
                float N_y = yDiff / nMag;
                float N_z = zDiff / nMag;
                float NdotV = N_x * V_x + N_y * V_y + N_z * V_z;
                if (NdotV < 0 || equalULP(NdotV, 0))
                {
                    // Look's like the entity is moving into the border.
                    // If any of the entity's points are beyond the collision radius
                    // it's colliding!                
                    const Array<Point> & points = entities[i]->frame.points;
                    for (int j = 0; j < points.size(); j++)
                    {
                        float pXDiff = xDiff - points[j].x;
                        float pYDiff = yDiff - points[j].y;
                        float pZDiff = zDiff - points[j].z;
                        float pointLocationDifference = sqrt(pXDiff * pXDiff + pYDiff * pYDiff + pZDiff * pZDiff);
                        if (locationDifference > minCollidingRadius)
                        {
                            // Point has passed the collidingradius, apply the
                            // collision as if the center of the entity was the
                            // collision point for simplicity and call it a day.
                            entities[i]->velocity.x = V_x - N_x * 2 * NdotV;
                            entities[i]->velocity.y = V_y - N_y * 2 * NdotV;
                            entities[i]->velocity.z = V_z - N_z * 2 * NdotV;
                            break;
                        }
                    }
                }
            }
        }
    }
}


void calculateBorderCollisionsOld(Array<Entity*> entities, Entity * border)
{
    struct Collision
    {
        Entity * entity;
        Point collisionLocation;
    };

    // Holds all the found collisions
    Array<Collision> collisions;

    for (int i = 0; i < entities.size(); i++)
    {
        // Check if the entities' bounding radii overlap, if they don't collision isn't possilbe.
        float xDifference = entities[i]->locationPoint.x - border->locationPoint.x;
        float yDifference = entities[i]->locationPoint.y - border->locationPoint.y;
        float zDifference = entities[i]->locationPoint.z - border->locationPoint.z;
        float locationDifference = sqrt(xDifference * xDifference + yDifference * yDifference + zDifference * zDifference);
        // In the special case of a border entity, mass represents its inner bounding radius
        float minBoundingRadius = border->mass - entities[i]->boundingRadius;

        // If the entity could have points outside the border's bounding
        // radius, they could be intersecting.
        if (locationDifference > minBoundingRadius)
        {
            // All the entity intersects between these two
            Array<Point> collisionPoints;

            Shape & shapeA = entities[i]->worldSpaceShape;
            Shape & shapeB = border->worldSpaceShape;

            for (int k = 0; k < shapeA.lines.size(); k++)
            {
                for (int l = 0; l < shapeB.triangles.size(); l++)
                {
                    Point intersectLocation;
                    if (fasterLineTriangleIntersect(shapeA.lines[k], shapeB.triangles[l], intersectLocation))
                    {
                        collisionPoints += intersectLocation;
                    }
                }
            }

            // A collision has been found
            if (collisionPoints.size() > 0)
            {
                // Get an average point from the collected collision poiints
                float xAverage = 0;
                float yAverage = 0;
                float zAverage = 0;
                for (int k = 0; k < collisionPoints.size(); k++)
                {
                    xAverage += collisionPoints[k].x;
                    yAverage += collisionPoints[k].y;
                    zAverage += collisionPoints[k].z;
                }
                xAverage /= collisionPoints.size();
                yAverage /= collisionPoints.size();
                zAverage /= collisionPoints.size();

                Collision newCollision;
                newCollision.entity = *(entities.getPointerTo(i));
                newCollision.collisionLocation = Point(xAverage, yAverage, zAverage);

                collisions += newCollision;
            }
            else if (locationDifference > border->boundingRadius)
            {
                // Check if the entity somehow passed the border without 
                // colliding. This can happen when the velocities are
                // higher then the width of the entity.
                Collision newCollision;
                newCollision.entity = *(entities.getPointerTo(i));
                newCollision.collisionLocation = entities[i]->locationPoint;

                collisions += newCollision;
            }
        }
    }

    // Resolve all the collisions
    for (int i = 0; i < collisions.size(); i++)
    {
        Vector & oldVelocity = collisions[i].entity->velocity;
        Vector & normal = border->locationPoint - collisions[i].collisionLocation;
        normal.normalize();
        Vector newVelocity = oldVelocity - normal * 2 * normal.dotProduct(oldVelocity);

        float dotTest = normal.dotProduct(newVelocity);
        if (dotTest > 0 || (!equalULP(dotTest, 0) && dotTest > 0))
        {
            collisions[i].entity->velocity = newVelocity;
        }
    }
}
