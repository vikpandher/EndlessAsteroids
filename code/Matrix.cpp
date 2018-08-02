/* ==========================================================================
   >File: Matrix.cpp
   >Date: 20180713
   >Author: Vik Pandher
   >Details: This class represents a matrix in 3D space.
   ========================================================================== */

#include "Matrix.h"

Matrix::Matrix()
{
    setToIdentity();
}


Matrix::Matrix(const Matrix & other)
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            m_data[row][col] = other.m_data[row][col];
        }
    }
}


Matrix::Matrix(float a, float b, float c, float d,
                   float e, float f, float g, float h,
                   float i, float j, float k, float l,
                   float m, float n, float o, float p)
{
    m_data[0][0] = a;
    m_data[0][1] = b;
    m_data[0][2] = c;
    m_data[0][3] = d;
    m_data[1][0] = e;
    m_data[1][1] = f;
    m_data[1][2] = g;
    m_data[1][3] = h;
    m_data[2][0] = i;
    m_data[2][1] = j;
    m_data[2][2] = k;
    m_data[2][3] = l;
    m_data[3][0] = m;
    m_data[3][1] = n;
    m_data[3][2] = o;
    m_data[3][3] = p;
}


Matrix::~Matrix()
{
    // nothing to do here
}


void Matrix::setToIdentity()
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (row != col)
            {
                m_data[row][col] = 0;
            }
            else
            {
                m_data[row][col] = 1;
            }
        }
    }
}


void Matrix::setToZero()
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            m_data[row][col] = 0;
        }
    }
}


float Matrix::getDeterminant() const
{
    // [ a b c d ]
    // [ e f g h ]
    // [ i j k l ]
    // [ m n o p ]
    // 
    // a * [f g h] - b * [e g h] + c * [e f h] - d * [e f g]
    //     [j k l]       [i k l]       [i j l]       [i j k]
    //     [n o p]       [m o p]       [m n p]       [m n o]
    // 
    // a * ( f * [k l] - g * [j l] + h * [j k] ) -
    //           [o p]       [n p]       [n o]
    // b * ( e * [k l] - g * [i l] + h * [i k] ) +
    //           [o p]       [m p]       [m o]
    // c * ( e * [j l] - f * [i l] + h * [i j] ) -
    //           [n p]       [m p]       [m n]
    // d * ( e * [j k] - f * [i k] + g * [i j] )
    //           [n o]       [m o]       [m n]
    // 
    // a * (f * (k * p - l * o) - g * (j * p - l * n) + h * (j * o - k * n)) -
    // b * (e * (k * p - l * o) - g * (i * p - l * m) + h * (i * o - k * m)) +
    // c * (e * (j * p - l * n) - f * (i * p - l * m) + h * (i * n - j * m)) -
    // d * (e * (j * o - k * n) - f * (i * o - k * m) + g * (i * n - j * m))
    return
        m_data[0][0] * (m_data[1][1] * (m_data[2][2] * m_data[3][3] - m_data[2][3] * m_data[3][2]) - m_data[1][2] * (m_data[2][1] * m_data[3][3] - m_data[2][3] * m_data[3][1]) + m_data[1][3] * (m_data[2][1] * m_data[3][2] - m_data[2][2] * m_data[3][1])) -
        m_data[0][1] * (m_data[1][0] * (m_data[2][2] * m_data[3][3] - m_data[2][3] * m_data[3][2]) - m_data[1][2] * (m_data[2][0] * m_data[3][3] - m_data[2][3] * m_data[3][0]) + m_data[1][3] * (m_data[2][0] * m_data[3][2] - m_data[2][2] * m_data[3][0])) +
        m_data[0][2] * (m_data[1][0] * (m_data[2][1] * m_data[3][3] - m_data[2][3] * m_data[3][1]) - m_data[1][1] * (m_data[2][0] * m_data[3][3] - m_data[2][3] * m_data[3][0]) + m_data[1][3] * (m_data[2][0] * m_data[3][1] - m_data[2][1] * m_data[3][0])) -
        m_data[0][3] * (m_data[1][0] * (m_data[2][1] * m_data[3][2] - m_data[2][2] * m_data[3][1]) - m_data[1][1] * (m_data[2][0] * m_data[3][2] - m_data[2][2] * m_data[3][0]) + m_data[1][2] * (m_data[2][0] * m_data[3][1] - m_data[2][1] * m_data[3][0]));
}


void Matrix::invert()
{
    // cofactor matrix: (thanks WolframAlpha)
    // index 0, 0 = f * (k * p - l * o) + g * (l * n - j * p) + h * (j * o - k * n)
    // index 0, 1 = b * (l * o - k * p) + c * (j * p - l * n) + d * (k * n - j * o)
    // index 0, 2 = b * (g * p - h * o) + c * (h * n - f * p) + d * (f * o - g * n)
    // index 0, 3 = b * (h * k - g * l) + c * (f * l - h * j) + d * (g * j - f * k)
    // index 1, 0 = e * (l * o - k * p) + g * (i * p - l * m) + h * (k * m - i * o)
    // index 1, 1 = a * (k * p - l * o) + c * (l * m - i * p) + d * (i * o - k * m)
    // index 1, 2 = a * (h * o - g * p) + c * (e * p - h * m) + d * (g * m - e * o)
    // index 1, 3 = a * (g * l - h * k) + c * (h * i - e * l) + d * (e * k - g * i)
    // index 2, 0 = e * (j * p - l * n) + f * (l * m - i * p) + h * (i * n - j * m)
    // index 2, 1 = a * (l * n - j * p) + b * (i * p - l * m) + d * (j * m - i * n)
    // index 2, 2 = a * (f * p - h * n) + b * (h * m - e * p) + d * (e * n - f * m)
    // index 2, 3 = a * (h * j - f * l) + b * (e * l - h * i) + d * (f * i - e * j)
    // index 3, 0 = e * (k * n - j * o) + f * (i * o - k * m) + g * (j * m - i * n)
    // index 3, 1 = a * (j * o - k * n) + b * (k * m - i * o) + c * (i * n - j * m)
    // index 3, 2 = a * (g * n - f * o) + b * (e * o - g * m) + c * (f * m - e * n)
    // index 3, 3 = a * (f * k - g * j) + b * (g * i - e * k) + c * (e * j - f * i)
    // 
    // invert(A) = cofactor(A) / determinant(A)
    float determinant = getDeterminant();
    if (determinant == 0) return; // Can't invert a matrix if the determinant == 0

    float newData[4][4];
    newData[0][0] = (m_data[1][1] * (m_data[2][2] * m_data[3][3] - m_data[2][3] * m_data[3][2]) + m_data[1][2] * (m_data[2][3] * m_data[3][1] - m_data[2][1] * m_data[3][3]) + m_data[1][3] * (m_data[2][1] * m_data[3][2] - m_data[2][2] * m_data[3][1])) / determinant;
    newData[0][1] = (m_data[0][1] * (m_data[2][3] * m_data[3][2] - m_data[2][2] * m_data[3][3]) + m_data[0][2] * (m_data[2][1] * m_data[3][3] - m_data[2][3] * m_data[3][1]) + m_data[0][3] * (m_data[2][2] * m_data[3][1] - m_data[2][1] * m_data[3][2])) / determinant;
    newData[0][2] = (m_data[0][1] * (m_data[1][2] * m_data[3][3] - m_data[1][3] * m_data[3][2]) + m_data[0][2] * (m_data[1][3] * m_data[3][1] - m_data[1][1] * m_data[3][3]) + m_data[0][3] * (m_data[1][1] * m_data[3][2] - m_data[1][2] * m_data[3][1])) / determinant;
    newData[0][3] = (m_data[0][1] * (m_data[1][3] * m_data[2][2] - m_data[1][2] * m_data[2][3]) + m_data[0][2] * (m_data[1][1] * m_data[2][3] - m_data[1][3] * m_data[2][1]) + m_data[0][3] * (m_data[1][2] * m_data[2][1] - m_data[1][1] * m_data[2][2])) / determinant;
    newData[1][0] = (m_data[1][0] * (m_data[2][3] * m_data[3][2] - m_data[2][2] * m_data[3][3]) + m_data[1][2] * (m_data[2][0] * m_data[3][3] - m_data[2][3] * m_data[3][0]) + m_data[1][3] * (m_data[2][2] * m_data[3][0] - m_data[2][0] * m_data[3][2])) / determinant;
    newData[1][1] = (m_data[0][0] * (m_data[2][2] * m_data[3][3] - m_data[2][3] * m_data[3][2]) + m_data[0][2] * (m_data[2][3] * m_data[3][0] - m_data[2][0] * m_data[3][3]) + m_data[0][3] * (m_data[2][0] * m_data[3][2] - m_data[2][2] * m_data[3][0])) / determinant;
    newData[1][2] = (m_data[0][0] * (m_data[1][3] * m_data[3][2] - m_data[1][2] * m_data[3][3]) + m_data[0][2] * (m_data[1][0] * m_data[3][3] - m_data[1][3] * m_data[3][0]) + m_data[0][3] * (m_data[1][2] * m_data[3][0] - m_data[1][0] * m_data[3][2])) / determinant;
    newData[1][3] = (m_data[0][0] * (m_data[1][2] * m_data[2][3] - m_data[1][3] * m_data[2][2]) + m_data[0][2] * (m_data[1][3] * m_data[2][0] - m_data[1][0] * m_data[2][3]) + m_data[0][3] * (m_data[1][0] * m_data[2][2] - m_data[1][2] * m_data[2][0])) / determinant;
    newData[2][0] = (m_data[1][0] * (m_data[2][1] * m_data[3][3] - m_data[2][3] * m_data[3][1]) + m_data[1][1] * (m_data[2][3] * m_data[3][0] - m_data[2][0] * m_data[3][3]) + m_data[1][3] * (m_data[2][0] * m_data[3][1] - m_data[2][1] * m_data[3][0])) / determinant;
    newData[2][1] = (m_data[0][0] * (m_data[2][3] * m_data[3][1] - m_data[2][1] * m_data[3][3]) + m_data[0][1] * (m_data[2][0] * m_data[3][3] - m_data[2][3] * m_data[3][0]) + m_data[0][3] * (m_data[2][1] * m_data[3][0] - m_data[2][0] * m_data[3][1])) / determinant;
    newData[2][2] = (m_data[0][0] * (m_data[1][1] * m_data[3][3] - m_data[1][3] * m_data[3][1]) + m_data[0][1] * (m_data[1][3] * m_data[3][0] - m_data[1][0] * m_data[3][3]) + m_data[0][3] * (m_data[1][0] * m_data[3][1] - m_data[1][1] * m_data[3][0])) / determinant;
    newData[2][3] = (m_data[0][0] * (m_data[1][3] * m_data[2][1] - m_data[1][1] * m_data[2][3]) + m_data[0][1] * (m_data[1][0] * m_data[2][3] - m_data[1][3] * m_data[2][0]) + m_data[0][3] * (m_data[1][1] * m_data[2][0] - m_data[1][0] * m_data[2][1])) / determinant;
    newData[3][0] = (m_data[1][0] * (m_data[2][2] * m_data[3][1] - m_data[2][1] * m_data[3][2]) + m_data[1][1] * (m_data[2][0] * m_data[3][2] - m_data[2][2] * m_data[3][0]) + m_data[1][2] * (m_data[2][1] * m_data[3][0] - m_data[2][0] * m_data[3][1])) / determinant;
    newData[3][1] = (m_data[0][0] * (m_data[2][1] * m_data[3][2] - m_data[2][2] * m_data[3][1]) + m_data[0][1] * (m_data[2][2] * m_data[3][0] - m_data[2][0] * m_data[3][2]) + m_data[0][2] * (m_data[2][0] * m_data[3][1] - m_data[2][1] * m_data[3][0])) / determinant;
    newData[3][2] = (m_data[0][0] * (m_data[1][2] * m_data[3][1] - m_data[1][1] * m_data[3][2]) + m_data[0][1] * (m_data[1][0] * m_data[3][2] - m_data[1][2] * m_data[3][0]) + m_data[0][2] * (m_data[1][1] * m_data[3][0] - m_data[1][0] * m_data[3][1])) / determinant;
    newData[3][3] = (m_data[0][0] * (m_data[1][1] * m_data[2][2] - m_data[1][2] * m_data[2][1]) + m_data[0][1] * (m_data[1][2] * m_data[2][0] - m_data[1][0] * m_data[2][2]) + m_data[0][2] * (m_data[1][0] * m_data[2][1] - m_data[1][1] * m_data[2][0])) / determinant;

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            m_data[row][col] = newData[row][col];
        }
    }
}


void Matrix::transpose()
{
    for (int col = 1; col < 4; col++)
    {
        for (int row = 0; row < col; row++)
        {
            float temp = m_data[row][col];
            m_data[row][col] = m_data[col][row];
            m_data[col][row] = temp;
        }
    }
}


void Matrix::addTranslation(float x, float y, float z)
{
    Matrix other;
    other.m_data[3][0] = x;
    other.m_data[3][1] = y;
    other.m_data[3][2] = z;
    *this *= other;
}


void Matrix::addTranslationX(float x)
{
    Matrix other;
    other.m_data[3][0] = x;
    *this *= other;
}


void Matrix::addTranslationY(float y)
{
    Matrix other;
    other.m_data[3][1] = y;
    *this *= other;
}


void Matrix::addTranslationZ(float z)
{
    Matrix other;
    other.m_data[3][2] = z;
    *this *= other;
}


void Matrix::addRotationV(Vector v, float r)
{
    if (!v.isUnitVector()) v.normalize();
    
    // A Quaternion:
    // q = a + bi + cj + dk
    // q = (a, v); v = <b, c, d>
    // 
    // Rotation counterclockwise around unit vector v:
    // q = (cos(r), sin(r)(v))
    float a = cos(r / 2);
    float b = sin(r / 2) * v.x;
    float c = sin(r / 2) * v.y;
    float d = sin(r / 2) * v.z;

    // Unit quaternion to 3x3 matrix:
    //     [ (1 - 2 * c^2 - 2 * d^2) (2 * b * c - 2 * d * a) (2 * b * d + 2 * c * a) ]
    // Q = [ (2 * b * c + 2 * d * a) (1 - 2 * b^2 - 2 * d^2) (2 * c * d - 2 * b * a) ]
    //     [ (2 * b * d - 2 * c * a) (2 * c * d + 2 * b * a) (1 - 2 * b^2 - 2 * c^2) ]
    Matrix other;
    other.m_data[0][0] = 1 - 2 * c * c - 2 * d * d;
    other.m_data[0][1] = 2 * b * c - 2 * d * a;
    other.m_data[0][2] = 2 * b * d + 2 * c * a;
    other.m_data[1][0] = 2 * b * c + 2 * d * a;
    other.m_data[1][1] = 1 - 2 * b * b - 2 * d * d;
    other.m_data[1][2] = 2 * c * d - 2 * b * a;
    other.m_data[2][0] = 2 * b * d - 2 * c * a;
    other.m_data[2][1] = 2 * c * d + 2 * b * a;
    other.m_data[2][2] = 1 - 2 * b * b - 2 * c * c;

    *this *= other;
}


void Matrix::addRotationX(float x)
{
    Matrix other;
    other.m_data[1][1] = cos(x);
    other.m_data[1][2] = sin(x);
    other.m_data[2][1] = (-1) * sin(x);
    other.m_data[2][2] = cos(x);
    *this *= other;
}


void Matrix::addRotationY(float y)
{
    Matrix other;
    other.m_data[0][0] = cos(y);
    other.m_data[0][2] = (-1) * sin(y);
    other.m_data[2][0] = sin(y);
    other.m_data[2][2] = cos(y);
    *this *= other;
}


void Matrix::addRotationZ(float z)
{
    Matrix other;
    other.m_data[0][0] = cos(z);
    other.m_data[0][1] = sin(z);
    other.m_data[1][0] = (-1) * sin(z);
    other.m_data[1][1] = cos(z);
    *this *= other;
}


void Matrix::addScale(float x, float y, float z)
{
    Matrix other;
    other.m_data[0][0] = x;
    other.m_data[1][1] = y;
    other.m_data[2][2] = z;
    *this *= other;
}


void Matrix::addScaleX(float x)
{
    Matrix other;
    other.m_data[0][0] = x;
    *this *= other;
}


void Matrix::addScaleY(float y)
{
    Matrix other;
    other.m_data[1][1] = y;
    *this *= other;
}


void Matrix::addScaleZ(float z)
{
    Matrix other;
    other.m_data[2][2] = z;
    *this *= other;
}


void Matrix::addCameraTransform(const Point & p, const Vector & direction, const Vector & up)
{
    // If the direction vector and the up vector are parallel or either is (0, 0, 0)
    if (direction.isParallel(up))
    {
        // invalid camera transform
        return;
    }
    
    // Translate the camera down to the origin
    Matrix cameraTransform;
    cameraTransform.addTranslation(-1 * p.x, -1 * p.y, -1 * p.z);

    // Rotate the vector around the Y-is, into the YZ plane, pointing toward
    // the -z direction.
    float theta;
    // vector is aligned to the Y-axis
    if (equalULP(direction.x, 0) && equalULP(direction.z, 0))
    {
        theta = 0;
    }
    // vector is on the YZ plane, but not aligned to the Y-axis
    else if (equalULP(direction.x, 0))
    {
        // vector is pointing towards +Z
        if (direction.z > 0) theta = _PI;
        // vector is pointing towards -Z
        else theta = 0;
    }
    // vector is on the XY plane, but not aligned to the Y-axis
    else if (equalULP(direction.z, 0))
    {
        // vector is pointing towards +X
        if (direction.x > 0) theta = _PI / 2;
        // vector is pointing towards -X
        else theta = (-1) * _PI / 2;
    }
    // vector is pointing towards +Z
    else if (direction.z > 0)
    {
        // vector is pointing towards +X
        if (direction.x > 0) theta = arcTan(direction.z / direction.x) + _PI / 2;
        // vector is pointing towards -X
        else theta = arcTan(direction.z / direction.x) - _PI / 2;
    }
    // vector is pointing towards -Z
    else
    {
        theta = (-1) * arcTan(direction.x / direction.z);
    }

    cameraTransform.addRotationY(theta);
    Vector directionYZ = direction * cameraTransform;
     
    // Rotate the vector around the X axis so it is now overlapping the Z axis
    // in the -z direction.
    float phi;
    // vector is aligned to the Z-axis and pointing towards -Z
    if (equalULP(directionYZ.y, 0))
    {
        phi = 0;
    }
    // vector is aligned to the Y-axis
    else if (equalULP(directionYZ.z, 0))
    {
        // vector is pointing towards +Y
        if (directionYZ.y > 0) phi = (-1) * _PI / 2;
        // vector is pointing towards -Y
        else phi = _PI / 2;
    }
    // vector is pointing towards -Z
    else
    {
        phi = arcTan(directionYZ.y / directionYZ.z);
    }

    cameraTransform.addRotationX(phi);
    Vector upZ = up * cameraTransform;
    
    // Move the up vector to be in the YZ plane pointing in a positive y
    // direction.
    float mu;
    // vector is on the YZ plane
    if (equalULP(upZ.x, 0))
    {
        // vector is pointing towards +Y
        if (upZ.y > 0) mu = 0;
        // vector is pointing towards -Y
        else mu = _PI;
    }
    // vector is on the XZ plane
    else if (equalULP(upZ.y, 0))
    {
        // vector is pointing towards +X
        if (upZ.x > 0) mu = _PI / 2;
        // vector is pointing towards -X
        else mu = (-1) * _PI / 2;
    }
    // vector is pointing towards +Y
    else if (upZ.y > 0)
    {
        mu = arcTan(upZ.x / upZ.y);
    }
    // vector is pointing towards -Y
    else
    {
        // vector is pointing towards +X
        if (upZ.x > 0) mu = (-1) * arcTan(upZ.y / upZ.x) + _PI / 2;
        // vector is pointing towards -X
        else mu = (-1) * arcTan(upZ.y / upZ.x) - _PI / 2;
    }
    
    cameraTransform.addRotationZ(mu);
    *this *= cameraTransform;
}


void Matrix::addViewingTransform(float r, float n, float f)
{
    Matrix viewingTransform;
    viewingTransform.m_data[2][2] = (f + n) / (f - n);
    viewingTransform.m_data[3][2] = 2 * n * f / (f - n);
    viewingTransform.m_data[2][3] = -1;
    viewingTransform.m_data[3][3] = 0;
    *this *= viewingTransform;
}


bool Matrix::operator==(const Matrix & other) const
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {  
            if (!equalULP(m_data[row][col], other.m_data[row][col])) return false;
        }
    }
    
    return true;
}


bool Matrix::operator!=(const Matrix & other) const
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (!equalULP(m_data[row][col], other.m_data[row][col])) return true;
        }
    }

    return false;
}


Matrix Matrix::operator*(const Matrix & other) const
{
    float newData[4][4];
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            newData[row][col] = 0;
            for (int i = 0; i < 4; i++)
            {
                newData[row][col] += this->m_data[row][i] * other.m_data[i][col];
            }
        }
    }

    Matrix newMatrix;

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            newMatrix.m_data[row][col] = newData[row][col];
        }
    }

    return newMatrix;
}


Matrix & Matrix::operator*=(const Matrix & other)
{
    float newData[4][4];
    for (int row = 0; row < 4; row++) // rows of the first matrix
    {
        for (int col = 0; col < 4; col++) // collumns of the second
        {
            newData[row][col] = 0; // zero out the junk memory
            for (int i = 0; i < 4; i++)
            {
                newData[row][col] += this->m_data[row][i] * other.m_data[i][col];
            }
        }
    }

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            m_data[row][col] = newData[row][col];
        }
    }

    return *this;
}


Matrix Matrix::operator*(const float & other) const
{
    Matrix newMatrix;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            newMatrix.m_data[i][j] = m_data[i][j] * other;
        }
    }
    return newMatrix;
}


Matrix & Matrix::operator*=(const float & other)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m_data[i][j] *= other;
        }
    }
    return *this;
}


Matrix Matrix::operator/(const float & other) const
{
    Matrix newMatrix;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            newMatrix.m_data[i][j] = m_data[i][j] / other;
        }
    }
    return newMatrix;
}


Matrix & Matrix::operator/=(const float & other)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m_data[i][j] /= other;
        }
    }
    return *this;
}


void Matrix::addRotationV2V(Vector start, Vector end)
{
    if (!start.isUnitVector()) start.normalize();
    if (!end.isUnitVector()) end.normalize();

    // Theta being the angle between the vectors
    float cosTheta = start.dotProduct(end);
    Vector rotationAxis;

    if (equalULP(cosTheta, -1)) // vectors are in opposite directions
    {
        // Pick an arbitrary vector to cross with, but make sure it's not
        // parrallel. If it is the cross product will be zero. When this
        // happens, a different arbitrary vector needs to be selected.
        rotationAxis = Vector(1, 0, 0).crossProduct(start);
        if (equalULP(rotationAxis.x, 0) && equalULP(rotationAxis.y, 0) && equalULP(rotationAxis.z, 0))
        {
            rotationAxis = Vector(0, 0, 1).crossProduct(start);
        }

        rotationAxis.normalize();
        addRotationV(rotationAxis, _PI);
    }
    else
    {
        rotationAxis = end.crossProduct(start);

        float s = sqrt((1 + cosTheta) * 2);

        // A Quaternion:
        // q = a + bi + cj + dk
        float a = s * 0.5;
        float b = rotationAxis.x / s;
        float c = rotationAxis.y / s;
        float d = rotationAxis.z / s;

        // Quaternion to matrix
        Matrix other;
        other.m_data[0][0] = 1 - 2 * c * c - 2 * d * d;
        other.m_data[0][1] = 2 * b * c - 2 * d * a;
        other.m_data[0][2] = 2 * b * d + 2 * c * a;
        other.m_data[1][0] = 2 * b * c + 2 * d * a;
        other.m_data[1][1] = 1 - 2 * b * b - 2 * d * d;
        other.m_data[1][2] = 2 * c * d - 2 * b * a;
        other.m_data[2][0] = 2 * b * d - 2 * c * a;
        other.m_data[2][1] = 2 * c * d + 2 * b * a;
        other.m_data[2][2] = 1 - 2 * b * b - 2 * c * c;

        *this *= other;
    }
}


void Matrix::addRotationXYZ(float x, float y, float z)
{
    Matrix other;
    other.m_data[0][0] = cos(y) * cos(z);
    other.m_data[0][1] = cos(y) * sin(z);
    other.m_data[0][2] = (-1) * sin(y);
    other.m_data[1][0] = sin(x) * sin(y) * cos(z) - cos(x) * sin(z);
    other.m_data[1][1] = sin(x) * sin(y) * sin(z) + cos(x) * cos(z);
    other.m_data[1][2] = sin(x) * cos(y);
    other.m_data[2][0] = cos(x) * sin(y) * cos(z) + sin(x) * sin(z);
    other.m_data[2][1] = cos(x) * sin(y) * sin(z) - sin(x) * cos(z);
    other.m_data[2][2] = cos(x) * cos(y);
    *this *= other;
}


void Matrix::addRotationZYX(float z, float y, float x)
{
    Matrix other;
    other.m_data[0][0] = cos(y) * cos(z);
    other.m_data[0][1] = sin(x) * sin(y) * cos(z) + cos(x) * sin(z);
    other.m_data[0][2] = sin(x) * sin(z) - cos(x) * sin(y) * cos(z);
    other.m_data[1][0] = (-1) * cos(y) * sin(z);
    other.m_data[1][1] = cos(x) * cos(z) - sin(x) * sin(y) * sin(z);
    other.m_data[1][2] = cos(x) * sin(y) * sin(z) + sin(x) * cos(z);
    other.m_data[2][0] = sin(y);
    other.m_data[2][1] = (-1) * sin(x) * cos(y);
    other.m_data[2][2] = cos(x) * cos(y);
    *this *= other;
}


void Matrix::addRotationGL(Vector v, float r)
{
    // Translate to the origin
    Matrix other;
    other.addTranslation(-1 * v.x, -1 * v.y, -1 * v.z);

    // Rotate to the yz-plane
    float theta = arcTan(v.x / v.z);
    other.addRotationY(-1 * theta);

    // Rotate to the z-axis
    float phi = arcTan(v.y / sqrt(v.x * v.x + v.z * v.z));
    other.addRotationX(phi);

    // Rotate around the z-axis by r
    other.addRotationZ(r);

    // Undo the rotation to the z-axis
    other.addRotationX(-1 * phi);

    // Undo the rotation to the yz-plane
    other.addRotationY(theta);

    // Undo the translation to the origin
    other.addTranslation(v.x, v.y, v.z);

    *this *= other;
}
