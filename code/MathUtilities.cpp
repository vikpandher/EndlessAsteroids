/* ==========================================================================
   >File: MathUtilities.cpp
   >Date: 20180713
   >Author: Vik Pandher
   >Details: Some helpfull math stuff.
   ========================================================================== */

#include "MathUtilities.h"



// equalULP
// ========================================================================== //
// Checks if two floats are about equal. This comparison compares floats in 
// terms of ULPs (unit in the last place) or (unit of least precision), which
// is the lowest spacing between two floats.
//
// Because this ULP becomes significantly small(almost zero) when comparing 
// very floats, a maxDiff value is used to compare these.
// 
// @params
// * float a, first float beign compared
// * float b, second float being compared
// * float maxDiff, maxDiff for vary large numbers
bool equalULP(float a, float b, float maxDiff /*= MAX_FLOAT_DIFF*/, int ulpsLimit /*= ULPS_LIMIT*/)
{
    // This comparison is required for numbers near zero becuase zero is
    // represented with all zeros in the exponent and fraction.
    //  S  E -----------------  E  F --------------------------------------------------------------  F
    // 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
    float diff = ABS(a - b);
    if (diff <= maxDiff)
    {
        return true;
    }

    // If the signs don't match.
    if ((a < 0 && b > 0) || (a > 0 && b < 0))
    {
        return false;
    }

    // The ULPS diff check how many possible float values away the two floats are.
    // If they differ by an exponent, the difference is huge compared to if they differ by some
    // fractional value.
    int aInt = *(int*)&a;
    int bInt = *(int*)&b;
    int ulpsDiff = ABS(aInt - bInt);
    if (ulpsDiff <= ulpsLimit)
    {
        return true;
    }

    return false;
}


// toThePowerOf
// ========================================================================== //
// Calculates x to the power of y.
// 
// @params
// * float x
// * float y
//
// @return
// x multiplied by itself y times.
float powerOf(float x, float y)
{
    if (equalULP(y, 0)) return 1;
    if (equalULP(x, 0)) return 0;
    float result = 1;
    if (y > 0)
    {
        for (int i = 0; i < y; i++)
        {
            result *= x;
        }
    }
    else if (y < 0)
    {
        for (int i = y; i < 0; i++)
        {
            result *= x;
        }
        result = 1 / result;
    }

    return result;
}


// factorial
// ========================================================================== //
// Calculates the factorial function for x (x!)
// Positive integers please.
// 
// @param
// * int x
//
// @return
// The result of x factorial
int factorial(int x)
{
    if (x < 2) return 1;
    int result = 1;
    for (int i = 2; i <= x; i++)
    {
        result *= i;
    }
    return result;
}


// sqrt
// ========================================================================== //
// Find the square root of x using the Babylonian method.
// 
// !Warning! This doesn't work with negative numbers!
// 
// @param
// * float x
// * float guess = 1,
// 
// @return
// The square root of x
float sqrt(float x)
{
    if (x < 0)
        throw ERROR_NEGATIVE_INPUT;
    if (equalULP(x, 0)) return 0;
    if (equalULP(x, 1)) return 1;
    float v0 = x;
    float v1 = 1;
    while (!equalULP(v0, v1))
    {
        v0 = (v0 + v1) / 2;
        v1 = x / v0;
    }
    return v0;
}


// logMaclaurinExpansion
// ========================================================================== //
// Calculate the natural log of x using the Taylor series expansion at x0 = 0 
// (a.k.a the Maclaurin Expansion).
// The input is assumed to be in raidians.
// 
// log(x) = (x-1)^1/1 - (x-1)^2/2 + (x-1)^3/3 - (x-1)^4/4 + (x-1)^5/5 - ...
// 
// @params
// * float x, value of log being calculated
// * float n, the degree of the taylor series
// 
// @return
// The log of x
float logMaclaurinExpansion(float x, float n /*= TAYLOR_SERIES_DEGREE*/)
{
    float sum = 0;
    for (int i = 0; i < n; i++)
    {
        float value;
        if (i % 2 == 0)
        {
            value = 1;
        }
        else
        {
            value = -1;
        }

        sum += value * powerOf(x - 1, i + 1) / (i + 1);
    }
    return sum;
}

// cosMaclaurinExpansion
// ========================================================================== //
// Calculate the cos of x using the Taylor series expansion at x0 = 0 (a.k.a
// the Maclaurin Expansion).
// The input is assumed to be in raidians.
// 
// cos(x) = 1 - (x^2)/(2!) + (x^4)/(4!) - (x^6)/(6!) + (x^8)/(8!) - ...
// 
// @params
// * float x, value of cos being calculated, in raidians
// * float n, the degree of the taylor series
// 
// @return
// The cos of x
float cosMaclaurinExpansion(float x, float n /*= TAYLOR_SERIES_DEGREE*/)
{
    float sum = 0;
    for (int i = 0; i < n; i++)
    {
        float value;
        if (i % 2 == 0)
        {
            value = 1;
        }
        else
        {
            value = -1;
        }

        sum += value * powerOf(x, (i * 2)) / factorial((i * 2));
    }
    return sum;
}


// sinMaclaurinExpansion
// ========================================================================== //
// Calculate the sin of x using the Taylor series expansion at x0 = 0 (a.k.a
// the Maclaurin Expansion).
// The input is assumed to be in raidians.
// 
// sin(x) = x - (x^3)/(3!) + (x^5)/(5!) - (x^7)/(7!) + (x^9)/(9!) - ...
// 
// @params
// * float x, value of sin being calculated, in raidians
// * float n, the degree of the taylor series
// 
// @return
// The sin of x
float sinMaclaurinExpansion(float x, float n /*= TAYLOR_SERIES_DEGREE*/)
{
    float sum = 0;
    for (int i = 0; i < n; i++)
    {
        float value;
        if (i % 2 == 0)
        {
            value = 1;
        }
        else
        {
            value = -1;
        }

        sum += value * powerOf(x, (i * 2) + 1) / factorial((i * 2) + 1);
    }
    return sum;
}


// arcTanMaclaurinExpansion
// ========================================================================== //
// Calculate the arcTan of x using the Taylor series expansion at x0 = 0
// (a.k.a the Maclaurin Expansion).
// 
// arcTan(x) = x - (x^3)/3 + (x^5)/5 - (x^7)/7 + (x^9)/9 - (x^11)/11 + ...
// 
// @params
// * float x, value of arcTan being calculated
// * float n, the degree of the taylor series
// 
// @return
// The arcTan of x in radians
float arcTanMaclaurinExpansion(float x, float n /*= TAYLOR_SERIES_DEGREE*/)
{
    float sum = 0;
    for (int i = 0; i < n; i++)
    {
        float value;
        if (i % 2 == 0)
        {
            value = 1;
        }
        else
        {
            value = -1;
        }

        sum += value * powerOf(x, (i * 2) + 1) / ((i * 2) + 1);
    }
    return sum;
}


// arcTanTaylorExpansionAt1
// ========================================================================== //
// Calculate the arcTan of x using the Taylor series expansion at x0 = 1.
// 
// arcTan(x) = pi/4 + [SUM n > 0]( (2^(-n/2) * (x - 1)^n * sin(n*3pi/4))/n )
// 
// @params
// * float x, value of arcTan being calculated
// * float n, the degree of the taylor series
// 
// @return
// The arcTan of x in radians
float arcTanTaylorExpansionAt1(float x, float n /*= TAYLOR_SERIES_DEGREE*/)
{
    float sum = _PI / 4;
    int sign = 1;
    for (int i = 1; i < n; i++)
    {
        if (i % 4 == 0) continue;
        sum += sign * powerOf(x - 1, i) / (i * (powerOf(2, (i + 1) / 2)));
        sign *= -1;
    }
    return sum;
}


// cos
// ========================================================================== //
// Calculate the cos of x with x in radians.
//
// Uses the Maclaurin Expansion and only looks at inputs ranging from -pi/4
// to pi/4 because the accuracy of the Maclaurin Expansion decreases as it
// approches -/+ infinity.
//
// @param
// * float, x
// 
// @return
// The result of cos(x).
float cos(float x)
{
    // cos is periodic and I only wana look at -pi to pi, not including -pi
    while (x <= (-1) * _PI || x > _PI)
    {
        if (x <= (-1) * _PI) x += 2 * _PI;
        if (x > _PI) x -= 2 * _PI;
    }

    // Keep the input into the maclaurin expansion between -pi/4 and pi/4.
    float result;
    // -pi < x <= -3pi/4
    if ((-1) * _PI < x && x <= (-1) * 3 * _PI / 4) result = (-1) * cosMaclaurinExpansion(x + _PI);
    // -3pi/4 < x <= -pi/4
    if ((-1) * 3 * _PI / 4 < x && x <= (-1) * _PI / 4) result = sinMaclaurinExpansion(x + _PI / 2);
    // -pi/4 < x <= pi/4
    if ((-1) * _PI / 4 < x && x <= _PI / 4) result = cosMaclaurinExpansion(x);
    // pi/4 < x <= 3pi/4
    if (_PI / 4 < x && x <= 3 * _PI / 4) result = (-1) * sinMaclaurinExpansion(x - _PI / 2);
    // 3pi/4 < x <= pi
    if (3 * _PI / 4 < x && x <= _PI) result = (-1) * cosMaclaurinExpansion(x - _PI);
    return result;
}


// sin
// ========================================================================== //
// Calculate the sin of x with x in radians.
//
// Uses the Maclaurin Expansion and only looks at 0 to pi / 2 because the
// accuracy of the Maclaurin Expansion decreases as it approches -/+ infinity.
//
// @param
// * float, x
// 
// @return
// The result of sin(x).
float sin(float x)
{
    // sin is periodic and I only wana look at -pi to pi, not including -pi
    while (x <= (-1) * _PI || x > _PI)
    {
        if (x <= (-1) * _PI) x += 2 * _PI;
        if (x > _PI) x -= 2 * _PI;
    }

    // Keep the input into the maclaurin expansion between -pi/4 and pi/4.
    float result;
    // -pi < x <= -3pi/4
    if ((-1) * _PI < x && x <= (-1) *  3 * _PI / 4) result = (-1) * sinMaclaurinExpansion(x + _PI);
    // -3pi/4 < x <= -pi/4
    if ((-1) * 3 * _PI / 4 < x && x <= (-1) * _PI / 4) result = (-1) * cosMaclaurinExpansion(x + _PI / 2);
    // -pi/4 < x <= pi/4
    if ((-1) * _PI / 4 < x && x <= _PI / 4) result = sinMaclaurinExpansion(x);
    // pi/4 < x <= 3pi/4
    if (_PI / 4 < x && x <= 3 * _PI / 4) result = cosMaclaurinExpansion(x - _PI / 2);
    // 3pi/4 < x <= pi
    if (3 * _PI / 4 < x && x <= _PI) result = (-1) * sinMaclaurinExpansion(x - _PI);
    return result;
}


// tan
// ========================================================================== //
// Calculate the tan of x with x in radians.
// 
// Note: tan(x) is sometimes undefined. There will be a divide by zero error
//       when that happens.
// 
// @param
// * float, x
// 
// @return
// The result of tan(x).
float tan(float x)
{
    return sin(x) / cos(x);
}


// csc
// ========================================================================== //
// Calculate the csc of x with x in radians.
// 
// Note: csc(x) is sometimes undefined. There will be a divide by zero error
//       when that happens.
// 
// @param
// * float, x
// 
// @return
// The result of csc(x).
float csc(float x)
{
    return 1 / sin(x);
}


// sec
// ========================================================================== //
// Calculate the sec of x with x in radians.
// 
// Note: sec(x) is sometimes undefined. There will be a divide by zero error
//       when that happens.
// 
// @param
// * float, x
// 
// @return
// The result of sec(x).
float sec(float x)
{
    return 1 / cos(x);
}


// cot
// ========================================================================== //
// Calculate the cot of x with x in radians.
// 
// Note: cot(x) is sometimes undefined. There will be a divide by zero error
//       when that happens.
// 
// @param
// * float, x
// 
// @return
// The result of cot(x).
float cot(float x)
{
    return cos(x) / sin(x);
}


// arcTan
// ========================================================================== //
// Calculate the inverse tan of x.
// 
// Uses taylor series approximations and symmetry.
// 
// @param
// * float, x
// 
// @return
// The result of arcTan(x) in radians.
float arcTan(float x)
{
    float result;
    if (x < -1)
    {
        x *= -1; // arcTan((-1) * x) = (-1) * arcTan(x)
        x = 1 / x; // if x > 0; arcTan(x) = pi/2 - arcTan(1/x)
        if (x <= 0.5)
        {
            result = (-1) * (_PI / 2 - arcTanMaclaurinExpansion(x)); // accurate near 0
        }
        else
        {
            result = (-1) * (_PI / 2 - arcTanTaylorExpansionAt1(x)); // accurate near 1
        }
    }
    else if (-1 <= x && x < 0)
    {
        x *= -1; // arcTan((-1) * x) = (-1) * arcTan(x)
        if (x <= 0.5)
        {
            result = (-1) * arcTanMaclaurinExpansion(x); // accurate near 0
        }
        else
        {
            result = (-1) * arcTanTaylorExpansionAt1(x); // accurate near 1
        }
    }
    else if (0 <= x && x <= 1)
    {
        if (x <= 0.5)
        {
            result = arcTanMaclaurinExpansion(x); // accurate near 0
        }
        else
        {
            result = arcTanTaylorExpansionAt1(x); // accurate near 1
        }
    }
    else if (1 < x)
    {
        x = 1 / x; // if x > 0; arcTan(x) = pi/2 - arcTan(1/x)
        if (x <= 0.5)
        {
            result = _PI / 2 - arcTanMaclaurinExpansion(x); // accurate near 0
        }
        else
        {
            result = _PI / 2 - arcTanTaylorExpansionAt1(x); // accurate near 1
        }
    }

    return result;
}


// arcSin
// ========================================================================== //
// Calculate the inverse sin of x.
// 
// Uses the arcTan function above.
// 
// @param
// * float, x
// 
// @return
// The result of arcSin(x) in radians.
float arcSin(float x)
{
    if (equalULP(x, -1)) return -_PI / 2;
    else if (equalULP(x, 1)) return _PI / 2;
    else if (equalULP(x, 0)) return 0;
    else if (-1 < x && x > 1)
        throw ERROR_INPUT_OUT_OF_BOUNDS;
    else return arcTan(x / sqrt(1 - x * x));
}


// arcCos
// ========================================================================== //
// Calcualte the inverse cos of x.
// 
// Uses the arcTan function above.
// 
// @param
// * float, x
// 
// @return
// The result of arcCos(x) in raidians.
float arcCos(float x)
{
    if (equalULP(x, -1)) return _PI;
    else if (equalULP(x, 1)) return 0;
    else if (equalULP(x, 0)) return _PI / 2;
    else if (-1 < x && x > 1)
        throw ERROR_INPUT_OUT_OF_BOUNDS;
    else return arcTan(sqrt(1 - x * x) / x);
}