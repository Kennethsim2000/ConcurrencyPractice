#include <iostream>

template <typename T>
class vector
{
    T *data_; // address of the first element in the vector, so data_ + 1 actually points to the next element in memory.
    size_t len_;
    size_t capacity_;

public:
    vector() // default constructor
    {
        len_ = 0;
        data_ = nullptr;
        capacity_ = 0;
    }

    vector(int capacity)
    {
        capacity_ = capacity;
        len_ = 0;
        data_ = new T[capacity];
    }

    ~vector()
    {
        delete[] data_;
    }

    size_t size()
    {
        return len_;
    }

    size_t capacity()
    {
        return capacity_;
    }
};

// size(), capacity(), operator[] (unchecked), push_back, pop_back.
