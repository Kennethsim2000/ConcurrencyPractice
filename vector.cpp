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
        data_ = ::operator new(sizeof(T) * capacity); //  this does not call constructor of T yet, it just gives u a raw memory block.
        // data_ = new T[capacity];
        // This implementation constructs capacity objects right away
    }

    ~vector()
    {
        // delete[] data_;
        // The delete operator deallocates memory and calls the destructor for a single object created with new.
        //  The delete [] operator deallocates memory and calls destructors for an array of objects created with new []

        for (size_t i = 0; i < len_; i++) // calls the destructor of each of the elements in the vector
        {
            data_[i].~T();
        }

        ::operator delete(data_); // frees raw memory previously allocated by ::operator new, does not call destructor
    }

    // TODO: Implement this resize method to reserve capacity amount of memory, and copy over the elements of the previous
    void resize(size_t capacity)
    {
        void *raw = ::operator new(sizeof(T) * capacity);
        T *new_data = static_cast<T *>(raw);
        for (size_t i = 0; i < len_; i++)
        {
            new (new_data + i) T std::move(data_[i]); // because we are working with raw memory, you need to construct objects explicitly with in new_allocated
            data_[i].~T();
        }
        ::operator delete(data_);
        data_ = new_data;
        capacity_ = capacity;
    }

    // TODO: Complete indexing
    T &operator[](size_t index)
    { // operator[] is an array index operator, used to access elements within a collection
    }

    // TODO: Complete push_back method
    void push_back(T elem)
    {
    }

    // TODO: print vector
    void print_vector()
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

int main()
{
    return 0;
}
// size(), capacity(), operator[] (unchecked), push_back, pop_back.
