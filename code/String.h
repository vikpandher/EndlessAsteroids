/* ==========================================================================
   >File: String.h
   >Date: 20180713
   >Author: Vik Pandher
   >Details: A Basic String Class
   ========================================================================== */

#pragma once
#include "Array.h"
#include "MemoryManager.h"



class String : public Array<char>
{
public:
    String() : Array<char>() {}
    String(const char c) : Array<char>(c) {}
    String(const char* s); // Array better be null terminated!!!
    String(int n); // Does not support long long's

    unsigned countNewLines() const; // Pritty self explanitory

    String& operator+=(const String& s);
    String& operator+=(char c);
    String& operator+=(const char* s);

    inline friend String operator+(const String& lhs, const String& rhs)
    {
        return String(lhs) += rhs;
    }


    inline friend String operator+(const String& lhs, const char rhs)
    {
        return String(lhs) += rhs;
    }


    inline friend String operator+(const String& lhs, const char* rhs)
    {
        return String(lhs) += rhs;
    }


    inline friend String operator+(const char lhs, const String& rhs)
    {
        return String(lhs) += rhs;
    }


    inline friend String operator+(const char* lhs, const String& rhs)
    {
        return String(lhs) += rhs;
    }

    ///void* operator new(size_t size);
    ///void operator delete(void * p);

    static String stringFromInt(int x)
    {
        String s;
        bool negative;
        if (x < 0)
        {
            negative = true;
            x *= -1;
        }
        else
        {
            negative = false;
        }

        do
        {
            s += '0' + (x % 10);
            x /= 10;
        } while (x != 0);
        if (negative) s += '-';

        s.flip();
        return s;
    }
};