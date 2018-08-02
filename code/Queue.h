/* ==========================================================================
   >File: Queue.h
   >Date: 20180515
   >Author: Vik Pandher
   >Details: 
   ========================================================================== */

#pragma once
#include "ErrorCodes.h"

template <class T>
class Queue
{
public:
    Queue();
    Queue(const T other);
    Queue(const Queue<T>& other);
    Queue(const T* other, size_t size);
    ~Queue();

    // how many things in this
    inline size_t size() const { return m_size; }

    // This does nothing if the new capacity is less then the current size.
    void setCapacity(unsigned capacity);

    // Add an element to the back of the queue.
    void add(const T other);

    // Remove an element from the front oof the queue.
    T remove();

protected:
    // capacity starts at 1 and will double when growing
    size_t m_capacity;
    size_t m_size;
    T * m_data;
    size_t m_front; // index where remove occurs
    size_t m_back; // index where add occurs
};


template <class T>
Queue<T>::Queue()
{
    m_capacity = 1;
    m_size = 0;
    m_data = new T[m_capacity];
    m_front = 0;
    m_back = 0;
}


template <class T>
Queue<T>::Queue(const T other)
{
    m_capacity = 1;
    m_size = 1;
    m_data = new T[m_capacity];
    m_data[0] = other;
    m_front = 0;
    m_back = 1;
}


template <class T>
Queue<T>::Queue(const Queue<T>& other)
{
    m_capacity = other.m_capacity;
    m_size = other.m_size;
    m_data = new T[m_capacity];
    for (unsigned i = 0; i < m_size; i++)
    {
        m_data[i] = other.m_data[i];
    }
    m_front = other.m_front;
    m_back = other.m_back;
}


template <class T>
Queue<T>::Queue(const T* other, size_t size)
{
    m_capacity = size;
    m_size = size;
    m_data = new T[m_capacity];
    for (unsigned i = 0; i < size; i++)
    {
        m_data[i] = other[i];
    }
    m_front = 0;
    m_back = size;
}


template <class T>
Queue<T>::~Queue()
{
    delete[] m_data;
}


template <class T>
void Queue<T>::setCapacity(unsigned capacity)
{
    if (capacity >= m_size)
    {
        T* newData = new T[capacity];
        int j = 0;
        int i = m_front;
        if (m_size == 1)
        {
            newData[0] = m_data[0];
        }
        else if (m_size != 0)
        {
            do
            {
                newData[j] = m_data[i];
                j++;
                if (i < m_capacity - 1) i++;
                else i = 0;
            } while (i != m_back);
        }
        delete[] m_data;
        m_capacity = capacity;
        m_data = newData;
        m_front = 0;
        m_back = m_size;
    }
}


template <class T>
void Queue<T>::add(const T other)
{
    if (m_back == m_front && m_size != 0)
    {
        setCapacity(m_capacity * 2);
    }
    
    m_data[m_back] = other;
    m_back++;
    if (m_back == m_capacity) m_back = 0;
    m_size++;
}


template <class T>
T Queue<T>::remove()
{
    if (m_size == 0)
        throw ERROR_OUTSIDE_ARRAY_BOUNDS;
    T result = m_data[m_front];
    m_front++;
    if (m_front == m_capacity) m_front = 0;
    m_size--;
    return result;
}

///void test()
///{
///    int list[3] = { 1, 2, 3 };
///    Queue<int> q0;
///    Queue<int> q1(1);
///    Queue<int> q2(list, 3);
///
///    q0.add(99);
///    q0.remove();
///
///    for (int i = 0; i < 7; i++)
///    {
///        q0.add(i);
///    }
///    for (int i = 0; i < 3; i++)
///    {
///        q0.remove();
///    }
///    for (int i = 7; i < 16; i++)
///    {
///        q0.add(i);
///    }
///}