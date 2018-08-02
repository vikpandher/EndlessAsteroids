/* ==========================================================================
   >File: Quaternion.h
   >Date: 20180713
   >Author: Vik Pandher
   >Details: This class represents a quaternion. It's usefull for doing 3D
             rotations.
   ========================================================================== */

#pragma once
#include "MathUtilities.h"
#include "Vector.h"
#include "Matrix.h"


class Quaternion
{
public:
    Quaternion();
    Quaternion(float a, float b, float c, float d) : a(a), b(b), c(c), d(d) {}
    Quaternion(const Quaternion & other);
    ~Quaternion();

    // setToIdentity
    // ====================================================================== //
    // Set this quaternion to be the identity quaternion.
    // 
    // q = (1) + (0)i + (0)j + (0)k
    // 
    // q * q^-1 = q^-1 * q
    void setToIdentity();

    // setToZero
    // ====================================================================== //
    // Set this quaternion to zero.
    // 
    // q = (0) + (0)i + (0)j + (0)k
    void setToZero();

    // length
    // ====================================================================== //
    // Get the length of this quaternion.
    // 
    // |q| = sqrt(a^2 + |v|^2); |v|^2 = v dot v
    // 
    // @return
    // The length of this quaternion
    float length() const;

    // normalize
    // ====================================================================== //
    // Make this quaternion into a unit quaternion (length == 1).
    void normalize();

    // isUnitQuaternion
    // ====================================================================== //
    // Check if this quaternion is a unit quaternion.
    //
    // Note: A unit quaternion multiplied a unit quaternion results in a unit
    //       quaternion.
    // 
    // @return
    // True if it is a unit quaternion
    bool isUnitQuaternion() const;

    // invert
    // ====================================================================== //
    // Set this quaternion to its multiplicative inverse.
    // 
    // q^-1 = (a, -v) / (a^2 + |v|^2)
    void invert();

    // getInverse
    // ====================================================================== //
    // Get this quaternion's multiplicative inverse.
    // 
    // @return
    // This quaternion's multiplicative inverse.
    Quaternion getInverse() const;

    // addRotation
    // ====================================================================== //
    // Add a rotation around a vector.
    // 
    // (cos(theta/2), sin(theta/2) * vector) where |vector| = 1
    // 
    // @params
    // * const Vector & vector, the vector being rotated around
    //                          (this must be a unit vector)
    // * float theta, the angle of rotation in radians (counter clockwise)
    void addRotation(const Vector & vector, float theta);
    
    // getMatrix
    // ====================================================================== //
    // Get the matrix equavalent of this quaternion.
    // 
    // q = a + bi + cj + dk
    //     [(1 - 2*c^2 - 2*d^2)   (2*b*c - 2*d*a)     (2*b*d + 2*c*a)  ]
    // Q = [  (2*b*c + 2*d*a)   (1 - 2*b^2 - 2*d^2)   (2*c*d - 2*b*a)  ]
    //     [  (2*b*d - 2*c*a)     (2*c*d + 2*b*a)   (1 - 2*b^2 - 2*c^2)]
    //
    // @return
    // 4 by 4 matrix quavalent of this quaternion
    Matrix getMatrix() const;
    
    // getSomeVector
    // ====================================================================== //
    // Get a direction vector from this quaternion.
    // 
    // For an identity quaternion:
    //                 (x, y, z)
    // forwardVector = (1, 0, 0)
    //      upVector = (0, 1, 0)
    //   rightVector = (0, 0, 1)
    // 
    // Note: They are the columns of the quaternion matrix.
    // 
    // @ return
    // A direction vector;
    Vector getForwardVector() const;
    Vector getUpVector() const;
    Vector getRightVector() const;

    bool operator==(const Quaternion & other) const;
    bool operator!=(const Quaternion & other) const;

    Quaternion operator+(const Quaternion & other) const;
    Quaternion & operator+=(const Quaternion & other);

    Quaternion operator-(const Quaternion & other) const;
    Quaternion & operator-=(const Quaternion & other);

    Quaternion operator*(const Quaternion & other) const;
    Quaternion & operator*=(const Quaternion & other);

    Quaternion operator/(const Quaternion & other) const;
    Quaternion & operator/=(const Quaternion & other);

    Quaternion operator*(const float & other) const;
    Quaternion & operator*=(const float & other);

    Quaternion operator/(const float & other) const;
    Quaternion & operator/=(const float & other);



    // A Quaternion:
    // q = a + bi + cj + dk
    // q = (a, v); v = <b, c, d>
    // q = (w, v); v = <x, y, z>
    union
    {
        float m_data[4];
        struct
        {
            float a, b, c, d;
        };
        struct
        {
            float w;
            union
            {
                float v[3];
                struct
                {
                    float x, y, z;
                };
            };
        };
    };
};