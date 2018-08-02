/* ==========================================================================
   >File: String.cpp
   >Date: 20180713
   >Author: Vik Pandher
   >Details: A Basic String Class
   ========================================================================== */

#include "String.h"



String::String(const char* s)
{
    int size = 0;
    while (s[size] != '\0') size++;
    
    if (size > 0) m_capacity = size; // im gonna keep the minimum capacity 1
    m_size = size;
    m_data = new char[m_capacity];
    for (unsigned int i = 0; i < size; i++)
    {
        m_data[i] = s[i];
    }
}


String::String(int n)
{
    bool negative = n < 0 ? true : false;
    if (negative) n = (-1) * n;
    while (n != 0)
    {
        char c = '0' + (n % 10);
        add(c, 0);
        n /= 10;
    }
    if (negative) add('-', 0);
}


unsigned String::countNewLines() const
{
    unsigned count = 0;
    for (unsigned i = 0; i < m_size; i++)
    {
        if (m_data[i] == '\n') count++;
    }
    return count;
}


String& String::operator+=(const String& s)
{
    unsigned newSize = m_size + s.m_size;
    if (m_capacity >= newSize)
    {
        for (unsigned i = 0; i < s.m_size; i++)
        {
            m_data[m_size + i] = s[i];
        }
        m_size = newSize;
    }
    else
    {
        while (m_capacity < newSize) m_capacity *= 2;
        char * newData = new char[m_capacity];
        for (unsigned i = 0; i < m_size; i++)
        {
            newData[i] = m_data[i];
        }
        for (unsigned i = 0; i < s.m_size; i++)
        {
            newData[m_size + i] = s[i];
        }
        delete[] m_data;
        m_size = newSize;
        m_data = newData;
    }
    return *this;
}


String& String::operator+=(char c)
{
    unsigned newSize = m_size + 1;
    if (m_capacity >= newSize)
    {
        m_data[m_size] = c;
        m_size = newSize;
    }
    else
    {
        while (m_capacity < newSize) m_capacity *= 2;
        char * newData = new char[m_capacity];
        for (unsigned i = 0; i < m_size; i++)
        {
            newData[i] = m_data[i];
        }
        newData[m_size] = c;

        delete[] m_data;
        m_size = newSize;
        m_data = newData;
    }
    return *this;
}


String& String::operator+=(const char* s)
{
    int size = 0;
    while (s[size] != '\0') size++;

    unsigned newSize = m_size + size;
    if (m_capacity >= newSize)
    {
        for (unsigned i = 0; i < size; i++)
        {
            m_data[m_size + i] = s[i];
        }
        m_size = newSize;
    }
    else
    {
        while (m_capacity < newSize) m_capacity *= 2;
        char * newData = new char[m_capacity];
        for (unsigned i = 0; i < m_size; i++)
        {
            newData[i] = m_data[i];
        }
        for (unsigned i = 0; i < size; i++)
        {
            newData[m_size + i] = s[i];
        }
        delete[] m_data;
        m_size = newSize;
        m_data = newData;
    }
    return *this;
}


///void * String::operator new(size_t size)
///{
///    return g_memoryManager.allocate(size);
///}


///oid String::operator delete(void * p)
///
///   g_memoryManager.free(p);
///
