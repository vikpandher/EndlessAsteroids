/* ==========================================================================
   >File: Array.h
   >Date: 20180713
   >Author: Vik Pandher
   >Details: An automatically resizing array. Capacity starts at 1 and
             doubles every time its size surpasses it. When this happens, a
             new bigger chunk of memory is allocated and data is copied over.
             When an element is removed from a location in the array that
             isn't the end, everything else is shifted over.
   ========================================================================== */

#pragma once
#include "ErrorCodes.h"

template <class T>
class Array
{
public:
    Array();
    Array(const T other);
    Array(const Array<T>& other);
    Array(const T* other, size_t size);
    ~Array();

    /* --------------------------------------------------------------------------
    NOTE:
    Remeber the first parameter in a copy constructor can't have teh same type
    as the class, structure, or union for which it is defined. The first
    parameter can be a reference to the type but not the type itself.
    Otherwise you get the compiler Error C2652.
    -------------------------------------------------------------------------- */

    inline size_t size() const { return m_size; }

    // Compare each element in both arrays. Starting from index 0 in both,
    // if the lhs element is greater then the rhs element, return 1. If the
    // rhs element is greather then the lhs element, return -1. If they are
    // equal look at the next index. If both had the same... just look at the
    // code.
    int compare(const Array<T>& lhs, const Array<T>& rhs) const;

    // -1 if not found
    int index(const T other) const;

    T* getPointerTo(size_t i);

    // This does nothing if the new capacity is less then the current size.
    void setCapacity(unsigned capacity);

    void pushBack(const T other);
    void add(const T other, unsigned index);
    void remove(unsigned index);

    void flip();

    T& operator[](size_t i);
    const T& operator[](size_t i) const;

    Array& operator=(const Array<T>& other);

    inline bool operator==(const Array<T>& other) const { return compare(this, other) == 0; }
    inline bool operator!=(const Array<T>& other) const { return compare(this, other) != 0; }
    inline bool operator< (const Array<T>& other) const { return compare(this, other) < 0; }
    inline bool operator> (const Array<T>& other) const { return compare(this, other) > 0; }
    inline bool operator<=(const Array<T>& other) const { return compare(this, other) <= 0; }
    inline bool operator>=(const Array<T>& other) const { return compare(this, other) >= 0; }

    Array& operator+=(const Array& other);
    Array& operator+=(const T other);

    Array operator+(const Array& other);
    Array operator+(const T other);
    
    // Having this code here caused Array<Array<char> > and Array<String> to
    // fail. I'm not sure why, I got the compiler error C2995. So I just use
    // the operator+ overrides above and left the function definitions in the
    // header file
    ///template <class T>
    ///inline friend Array<T> operator+(const Array<T>& lhs, const Array<T>& rhs)
    ///{
    ///    return Array<T>(lhs) += rhs;
    ///}
    ///
    ///template <class T>
    ///inline friend Array<T> operator+(const Array<T>& lhs, const T rhs)
    ///{
    ///    return Array<T>(lhs) += rhs;
    ///}
    ///
    ///template <class T>
    ///inline friend Array<T> operator+(const T lhs, const Array<T>& rhs)
    ///{
    ///    return Array<T>(lhs) += rhs;
    ///}

protected:
    // The capacity starts at 1 and doubles whenever the size grows too big.
    size_t m_capacity;
    size_t m_size;
    T * m_data;
};


/* --------------------------------------------------------------------------
   NOTE:
   Its simpler to have template functions in the header file.
   -------------------------------------------------------------------------- */

template <class T>
Array<T>::Array()
{
    m_capacity = 1;
    m_size = 0;
    m_data = new T[m_capacity];
}


template <class T>
Array<T>::Array(const T other)
{
    m_capacity = 1;
    m_size = 1;
    m_data = new T[m_capacity];
    m_data[0] = other;
}


template <class T>
Array<T>::Array(const Array<T>& other)
{
    m_capacity = other.m_capacity;
    m_size = other.m_size;
    m_data = new T[m_capacity];
    for (unsigned i = 0; i < m_size; i++)
    {
        m_data[i] = other[i];
    }
}


template <class T>
Array<T>::Array(const T* other, size_t size)
{
    m_capacity = size;
    m_size = size;
    m_data = new T[m_capacity];
    for (unsigned i = 0; i < m_size; i++)
    {
        m_data[i] = other[i];
    }
}


template <class T>
Array<T>::~Array()
{
    delete[] m_data;
}


template <class T>
int Array<T>::compare(const Array<T>& lhs, const Array<T>& rhs) const
{

    for (unsigned i = 0; i < lhs->m_size && i < rhs->m_size; i++)
    {
        if (lhs[i] > rhs[i])
        {
            return 1;
        }
        else if (lhs[i] < rhs[i])
        {
            return -1;
        }
    }

    if (lhs->m_size == rhs->m_size)
    {
        return 0;
    }
    else if (lhs->m_size > rhs->m_size)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

template <class T>
int Array<T>::index(const T other) const
{
    for (unsigned i = 0; i < m_size; i++)
    {
        if (other == m_data[i])
        {
            return i;
        }
    }

    return -1;
}

template <class T>
T* Array<T>::getPointerTo(size_t i)
{
    return m_data + i;
}


template <class T>
void Array<T>::setCapacity(unsigned capacity)
{
    if (capacity >= m_size)
    {
        m_capacity = capacity;
        T* newData = new T[m_capacity];
        for (unsigned i = 0; i < m_size; i++)
        {
            newData[i] = m_data[i];
        }
        delete[] m_data;;
        m_data = newData;;
    }
}


template <class T>
void Array<T>::pushBack(const T other)
{
    unsigned newSize = m_size + 1;
    if (m_capacity >= newSize)
    {
        m_data[m_size] = other;
        m_size = newSize;
    }
    else
    {
        m_capacity *= 2;
        T * newData = new T[m_capacity];
        for (unsigned i = 0; i < m_size; i++)
        {
            newData[i] = m_data[i];
        }
        newData[m_size] = other;
        delete[] m_data;
        m_size = newSize;
        m_data = newData;
    }
}


template <class T>
void Array<T>::add(const T other, unsigned index)
{
    unsigned newSize = m_size + 1;
    if (m_capacity >= newSize)
    {
        for (unsigned i = m_size; i > index; i--)
        {
            m_data[i] = m_data[i - 1];
        }
        m_data[index] = other;
        m_size = newSize;
    }
    else
    {
        m_capacity *= 2;
        T * newData = new T[m_capacity];
        for (unsigned i = 0; i < index; i++)
        {
            newData[i] = m_data[i];
        }
        newData[index] = other;
        for (unsigned i = index + 1; i < newSize; i++)
        {
            newData[i] = m_data[i - 1];
        }
        delete[] m_data;
        m_size = newSize;
        m_data = newData;
    }
}


template <class T>
void Array<T>::remove(unsigned index)
{
    m_size -= 1;
    for (unsigned i = index; i < m_size; i++)
    {
        m_data[i] = m_data[i + 1];
    }
}


template <class T>
void Array<T>::flip()
{
    for (unsigned i = 0; i < m_size / 2; i++)
    {
        T temp = m_data[i];
        m_data[i] = m_data[m_size - 1 - i];
        m_data[m_size - 1 - i] = temp;
    }
}


template <class T>
Array<T>& Array<T>::operator=(const Array<T>& other)
{
    delete[] m_data;

    m_capacity = other.m_capacity;
    m_size = other.m_size;
    m_data = new T[m_capacity];
    for (unsigned i = 0; i < m_size; i++)
    {
        m_data[i] = other[i];
    }

    return *this;
}


template <class T>
T& Array<T>::operator[](size_t i)
{
    if (i >= m_size)
        throw ERROR_OUTSIDE_ARRAY_BOUNDS;
    return m_data[i];
}


template <class T>
const T& Array<T>::operator[](size_t i) const
{
    if (i >= m_size)
        throw ERROR_OUTSIDE_ARRAY_BOUNDS;
    return m_data[i];
}


template <class T>
Array<T>& Array<T>::operator+=(const Array<T>& other)
{
    unsigned newSize = m_size + other.m_size;
    if (m_capacity >= newSize)
    {
        for (unsigned i = 0; i < other.m_size; i++)
        {
            m_data[m_size + i] = other[i];
        }
        m_size = newSize;
    }
    else
    {
        while (m_capacity < newSize) m_capacity *= 2;
        T * newData = new T[m_capacity];
        for (unsigned i = 0; i < m_size; i++)
        {
            newData[i] = m_data[i];
        }
        for (unsigned i = 0; i < other.m_size; i++)
        {
            newData[m_size + i] = other[i];
        }
        delete[] m_data;
        m_size = newSize;
        m_data = newData;
    }
    return *this;
}


template <class T>
Array<T>& Array<T>::operator+=(const T other)
{
    unsigned newSize = m_size + 1;
    if (m_capacity >= newSize)
    {
        m_data[m_size] = other;
        m_size = newSize;
    }
    else
    {
        m_capacity *= 2;
        T * newData = new T[m_capacity];
        for (unsigned i = 0; i < m_size; i++)
        {
            newData[i] = m_data[i];
        }
        newData[m_size] = other;

        delete[] m_data;
        m_size = newSize;
        m_data = newData;
    }
    return *this;
}


template <class T>
Array<T> Array<T>::operator+(const Array<T>& other)
{
    Array<T> newArray(*this);
    return newArray += other;
}


template <class T>
Array<T> Array<T>::operator+(const T other)
{
    Array<T> newArray(*this);
    return newArray += other;
}