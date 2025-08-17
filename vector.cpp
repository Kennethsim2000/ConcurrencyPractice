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
        // data_ = ::operator new(sizeof(T) * capacity);  this does not call constructor of T yet, it just gives u a raw memory block.
        data_ = new T[capacity];
        // This implementation constructs capacity objects right away
    }

    ~vector()
    {
        delete[] data_;
        // The delete operator deallocates memory and calls the destructor for a single object created with new.
        //  The delete [] operator deallocates memory and calls destructors for an array of objects created with new []
    }

    // TODO: Implement this resize method to reserve capacity amount of memory, and copy over the
    void resize(size_t capacity)
    {
    }

    // TODO: Complete push_back method
    void push_back(T elem)
    {
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
