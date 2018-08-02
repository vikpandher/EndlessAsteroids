/* ==========================================================================
   >File: Matrix.h
   >Date: 20180713
   >Author: Vik Pandher
   >Details: This class represents a matrix in 3D space.
   ========================================================================== */

#pragma once
#include "MathUtilities.h"

#ifndef GRAPHICS_INCLUDES
    #define GRAPHICS_INCLUDES true
    #include "Point.h"
    #include "Vector.h"
#else
    class Point;
    class Vector;
#endif

/* ==========================================================================
    !!! IMPORTANT NOTE ABOUT MATRIX MULTIPLICATION !!!
    
    Matrix multiplication is NOT commutative, order matters.
    
    Example:
    Vector * Matrix0 * Matrix1 != Vector * Matrix1 * Matrix0

    Look at the following code; result0, result1, and result2 are all equal.

    > Vector v0(2, 2, 2);
    > Matrix mX;
    > mX.addRotationX(_PI / 2);
    > Matrix mY;
    > mY.addRotationY(_PI / 2);
    > Matrix mZ;
    > mZ.addRotationZ(_PI / 2);
    > 
    > Vector result0 = v0 * mX * mY * mZ;
    > 
    > Vector result1 = v0 * mX;
    > result1 *= mY;
    > result1 *= mZ;
    > 
    > Matrix mC;
    > mC.addRotationX(_PI / 2);
    > mC.addRotationY(_PI / 2);
    > mC.addRotationZ(_PI / 2);
    > 
    > Vector result2 = v0 * mC;

    ========================================================================== */

class Matrix
{
public:
    Matrix();
    Matrix(const Matrix & other);
    Matrix(float a, float b, float c, float d,
           float e, float f, float g, float h,
           float i, float j, float k, float l,
           float m, float n, float o, float p);
    ~Matrix();

    // setToIdentity
    // ====================================================================== //
    // Sets this matrix to be a identity matrix.
    // 
    // [ 1 0 0 0 ]
    // [ 0 1 0 0 ]
    // [ 0 0 1 0 ]
    // [ 0 0 0 1 ]
    void setToIdentity();

    // setToZero
    // ====================================================================== //
    // Sets all the values in this matrix to zero.
    // 
    // [ 0 0 0 0 ]
    // [ 0 0 0 0 ]
    // [ 0 0 0 0 ]
    // [ 0 0 0 0 ]
    void setToZero();

    // getDeterminant
    // ====================================================================== //
    // Calculate the determinant of this matrix.
    // 
    // @return
    // The determinant of this matrix.
    float getDeterminant() const;

    // invert
    // ====================================================================== //
    // Turn this matrix into its inverse.
    // (matrix) * (inverse matrix) = (identity matrix)
    void invert();

    // transpose
    // ====================================================================== //
    // Transpose this matrix.
    // 
    // [ a b c d ]    [ a e i m ]
    // [ e f g h ] -> [ b f j n ]
    // [ i j k l ] -> [ c g k o ]
    // [ m n o p ]    [ d h l p ]
    void transpose();

    // addTranslation
    // ====================================================================== //
    // Add a translation to this matrix. This is done by multiplying it with
    // a translation matrix. It kind of looks like the following:
    // 
    // [ 1 0 0 0 ]
    // [ 0 1 0 0 ]
    // [ 0 0 1 0 ]
    // [ x y z 1 ]
    // 
    // @params
    // * float x, translation in the x direction
    // * float y, translation in the y direction
    // * float z, translation in the z direction
    void addTranslation(float x, float y, float z);
    void addTranslationX(float x);
    void addTranslationY(float y);
    void addTranslationZ(float z);

    // addRotationV
    // ====================================================================== //
    // Add a rotation to this matrix.
    // 
    // NOTE: Calculate the final r and v values before adding the rotaiton,
    //       rather then adding a buch of rotations at varius vectors, to
    //       avoid gimble lock.
    // 
    // @params
    // * Vector v, the vector being rotated around
    // * float r, rotation angle counter clockwise around v (in radians)
    void addRotationV(Vector v, float r);

    // addRotationX/Y/Z
    // ====================================================================== //
    // Add a rotation to this matrix. This is done by multiplying it with a
    // rotation matrix. It looks like the following:
    // 
    // R_x =
    // [    1       0       0       0   ]
    // [    0    +cos(x) +sin(x)    0   ]
    // [    0    -sin(x) +cos(x)    0   ]
    // [    0       0       0       1   ]
    // 
    // R_y =
    // [ +cos(x)    0    -sin(x)    0   ]
    // [    0       1       0       0   ]
    // [ +sin(x)    0    +cos(x)    0   ]
    // [    0       0       0       1   ]
    //
    // R_z =
    // [ +cos(z) +sin(z)    0       0   ]
    // [ -sin(z) +cos(z)    0       0   ]
    // [    0       0       1       0   ]
    // [    0       0       0       1   ]
    //
    // @params
    // * float x, rotation angle counter clockwise around x (in radians)
    // * float y, rotation angle counter clockwise around y (in radians)
    // * float z, rotation angle counter clockwise around z (in radians)
    void addRotationX(float x);
    void addRotationY(float y);
    void addRotationZ(float z);

    // addScale
    // ====================================================================== //
    // Add a scaling factor to this matrix. This is done by multiplying it
    // with a scaling matrix. It looks like the following:
    //
    // [ x 0 0 0 ]
    // [ 0 y 0 0 ]
    // [ 0 0 z 0 ]
    // [ 0 0 0 1 ]
    // 
    // @params
    // * float x, scaling in the x direction
    // * float y, scaling in the y direction
    // * float z, scaling in the z direction
    void addScale(float x, float y, float z);
    void addScaleX(float x);
    void addScaleY(float y);
    void addScaleZ(float z);

    // addCameraTransform
    // ====================================================================== //
    // Add a camera transform to this matrix. This matrix moves points from
    // world space to be relative to a camaera. The camera is given by an
    // origin point, a direction vector, and an up vector.
    // 
    // @params
    // * const Point & p, origin point of the camera
    // * const Vector & direction, direction the camera is pointing
    // * const Vector & up, up direction used to orient the camera
    void addCameraTransform(const Point & p, const Vector & direction, const Vector & up);

    // addViewingTransform
    // ====================================================================== //
    // Add a viewing transform to this matrix. The viewing transform converts
    // points to image space. This transform's viewing frustum points down
    // the -z axis.
    // 
    // r = angle of the viewing frustum
    // n = location of near plane down z axis, so it's on z = -n
    // f = location of far plane down z axis, so it's on z = -f
    // 
    // [ cot(r/2)     0          0       0 ]
    // [    0      cot(r/2)      0       0 ]
    // [    0         0     (f+n)/(f-n) -1 ]
    // [    0         0     (2nf)/(f-n)  0 ]
    // 
    // @params
    // * float r, the viewing angle of the frustum in radians
    // * float n, how far the near plane is down the z axis
    // * float f, how far the far plane is down the z axis
    void addViewingTransform(float r, float n, float f);

    bool operator==(const Matrix & other) const;
    bool operator!=(const Matrix & other) const;

    // Matrix * Matrix = Matrix
    Matrix operator*(const Matrix & other) const;
    Matrix & operator*=(const Matrix & other);

    // Matrix * Scalar = Matrix
    Matrix operator*(const float & other) const;
    Matrix & operator*=(const float & other);

    // Matrix / Scalar = Matrix
    Matrix operator/(const float & other) const;
    Matrix & operator/=(const float & other);

    // ---------------------------------------------------------------------- //
    // !!!                    Want to see gimble lock?                    !!! //
    // ---------------------------------------------------------------------- //

    /* ----------------------------------------------------------------------
       NOTES:
       The following functions caused gimble lock.
       ---------------------------------------------------------------------- */
    
    // addRotationV2V
    // ====================================================================== //
    // Add a rotation to this matrix. The rotation will be the rotation
    // required to orient one vector in the same direction as another.
    // 
    // Done via quaternion math.
    // 
    // @params
    // * Vector start, the starting vector, the one being rotated
    // * Vector end, the ending vector, the goal of the roation
    void addRotationV2V(Vector start, Vector end);

    // addRotationXYZ && addRotationZYX
    // ====================================================================== //
    // Add a rotation to this matrix. This is done by multiplying it with a
    // rotation matrix. It looks like the following:
    // 
    // R_x * R_y * R_z =
    // [ +CyCz        +CySz        -Sy   0 ]
    // [ +SxSyCz-CxSz +SxSySz+CxCz +SxCy 0 ]
    // [ CxSyCz+SxSz  +CxSySz-SxCz +CxCy 0 ]
    // [ 0            0            0     1 ]
    // 
    // R_z * R_y * R_x =
    // [ +CyCz +SxSyCz+CxSz +SxSz-CxSyCz 0 ]
    // [ -CySz +CxCz-SxSySz +CxSySz+SxCz 0 ]
    // [ +Sy   -SxCy        +CxCy        0 ]
    // [ 0     0            0            1 ]
    //
    // @params
    // * float x, rotation angle clockwise around x (in radians)
    // * float y, rotation angle clockwise around y (in radians)
    // * float z, rotation angle clockwise around z (in radians)
    void addRotationXYZ(float x, float y, float z);
    void addRotationZYX(float z, float y, float x);

    // addRotationGL
    // ====================================================================== //
    // Add a rotation to this matrix around an arbitrary axis.
    // 
    // (more accurate if the vector is a unit vector)
    // 
    // @params
    // * Vector v, the axis of rotation
    // * float r, rotation angle clockwise (in radians)
    void addRotationGL(Vector v, float r);



    // data will be represented in row major form (data[row][col])
    //
    //     col =  0  1  2  3
    // row = 0 [  0  1  2  3 ]
    //       1 [  4  5  6  7 ]
    //       2 [  8  9 10 11 ]
    //       3 [ 12 13 14 15 ]
    float m_data[4][4];
};