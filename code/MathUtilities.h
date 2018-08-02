/* ==========================================================================
   >File: MathUtilities.h
   >Date: 20180713
   >Author: Vik Pandher
   >Details: Some helpfull math stuff.
   ========================================================================== */

#pragma once
#include "ErrorCodes.h"



// -------------------------------------------------------------------------- //
#define SWAP(x,y) { x = x + y; y = x - y; x = x - y; }
#define SIGN(x) ((x > 0) - (x < 0))
#define ABS(x) (((x)<0) ? -(x) : (x))
#define MAX(x,y) (x >= y ? x : y)
#define MIN(x,y) (x <= y ? x : y)

#define _PI 3.14159265358979323846 
#define _E  2.71828182845904523536 // e

// If floats differ by less than this value, I'll consider them equal.
#define MAX_FLOAT_DIFF 0.000001

/* --------------------------------------------------------------------------
   NOTES:
   The ULPS_LIMIT has a significant affect on big numbers.

   float          memory                               ULPs
   1            = 0b00111111100000000000000000000000
   1.0000305    = 0b00111111100000000000000100000000
                  0b00000000000000000000000100000000 = 256
   1.0002441    = 0b00111111100000000000100000000000
                  0b00000000000000000000100000000000 = 2048
   
   1.0E-6       = 0b00110101100001100011011110111101
   1.0000291E-6 = 0b00110101100001100011100010111101
                  0b00000000000000000000000100000000 = 256
   1.0002328E-6 = 0b00110101100001100011111110111101
                  0b00000000000000000000100000000000 = 2048
   
   100          = 0b01000010110010000000000000000000

   100.00098    = 0b01000010110010000000000010000000
                  0b01000010110010000000000010000000 = 256
   100.015625   = 0b01000010110010000000100000000000
                  0b00000000000000000000100000000000 = 2048
   
   1000000      = 0b01001001011101000010010000000000
   1000016.0    = 0b01001001011101000010010100000000
                  0b00000000000000000000000100000000 = 256
   1000128.0    = 0b01001001011101000010110000000000
                  0b00000000000000000000100000000000 = 2048
   -------------------------------------------------------------------------- */
#define ULPS_LIMIT 2048 // 256

/* --------------------------------------------------------------------------
   NOTES:
   When n = 6 for the cos and sin calculations the maximum error (at pi/2 while
   using cosMaclaurinExpansion) is about 0.000000464766.
   -------------------------------------------------------------------------- */
#define TAYLOR_SERIES_DEGREE 6
#define TWEEK_FOR_TANGENT 3

// ... these arn't the most accurate calculations, but they work ...

// -------------------------------------------------------------------------- //
bool equalULP(float a, float b, float maxDiff = MAX_FLOAT_DIFF, int ulpsLimit = ULPS_LIMIT);
float powerOf(float x, float y);
int factorial(int x);
float sqrt(float x);
float logMaclaurinExpansion(float x, float n = TAYLOR_SERIES_DEGREE);
float cosMaclaurinExpansion(float x, float n = TAYLOR_SERIES_DEGREE);
float sinMaclaurinExpansion(float x, float n = TAYLOR_SERIES_DEGREE);
float arcTanMaclaurinExpansion(float x, float n = TAYLOR_SERIES_DEGREE + TWEEK_FOR_TANGENT);
float arcTanTaylorExpansionAt1(float x, float n = TAYLOR_SERIES_DEGREE + TWEEK_FOR_TANGENT);
float cos(float x);
float sin(float x);
float tan(float x);
float csc(float x);
float sec(float x);
float cot(float x);
float arcTan(float x);
float arcSin(float x);
float arcCos(float x);

// -------------------------------------------------------------------------- //
template <class T>
struct Pair
{
    Pair() : a(0), b(0) {}
    Pair(T _a, T _b) : a(_a), b(_b) {}

    bool operator==(const Pair<T>& other) const;

    union
    {
        struct
        {
            T a, b;
        };
        struct
        {
            T x, y;
        };
    };
};

template <class T>
bool Pair<T>::operator==(const Pair<T>& other) const
{
    return (a == other.a && b == other.b) || (a == other.b && b == other.a);
}


template <class T>
struct Triad
{
    Triad() : a(0), b(0), c(0) {}
    Triad(T _a, T _b, T _c) : a(_a), b(_b), c(_c) {}

    bool operator==(const Triad<T>& other) const;

    union
    {
        struct
        {
            T a, b, c;
        };
        struct
        {
            T x, y, z;
        };
    };
};

// possible combinations
// aa bb cc
// aa bc cb
// ac bb ca
// ab ba cc
// ab bc ca
// ac ba cb
template <class T>
bool Triad<T>::operator==(const Triad<T>& other) const
{
    return (a == other.a && b == other.b && c == other.c) ||
        (a == other.a && b == other.c && c == other.b) ||
        (a == other.c && b == other.b && c == other.a) ||
        (a == other.b && b == other.a && c == other.c) ||
        (a == other.b && b == other.c && c == other.a) ||
        (a == other.c && b == other.a && c == other.b);
}


struct SimplePoint
{
    SimplePoint() : x(0), y(0), z(0) {}
    SimplePoint(float x, float y, float z) : x(x), y(y), z(z) {}

    float x, y, z;
};