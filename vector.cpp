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

    void resize(size_t capacity)
    {
        void *raw = ::operator new(sizeof(T) * capacity); // allocate capacity * sizeof(T) bytes
        T *new_data = static_cast<T *>(raw);
        for (size_t i = 0; i < len_; i++)
        {
            new (new_data + i) T(std::move(data_[i])); // because we are working with raw memory, you need to construct objects explicitly with in new_allocated
            data_[i].~T();                             // call the destructor of the previous objects
        }
        ::operator delete(data_); // free up memory
        data_ = new_data;
        capacity_ = capacity;
    }

    const T &operator[](size_t index) // including a const T& means the function returns a reference to a const T, you can read the element but cannot modify if through the function
    {                                 // operator[] is an array index operator, used to access elements within a collection
        if (index >= len_)
        {
            throw std::out_of_range("Index is larger than size of vector");
        }
        else
        {
            return data_[index];
        }
    }

    // if size < capacity: write the new value at index size, then increment size.
    // Else, if current capacity is 0 → make it 1, otherwise double it
    // allocate a new buffer with that capacity.
    // copy existing size elements from old buffer to new buffer.
    // release the old buffer.
    // update pointer and capacity to the new buffer.
    // place the new value at index size, increment size.
    void push_back(T elem)
    {
        if (len_ == capacity_)
        { // we need to resize
            size_t newCapacity = capacity_ == 0 ? 1 : capacity_ * 2;
            resize(newCapacity);
        }
        new (data_ + len_) T(std::move(elem));
        len_++;
    }

    void print_vector()
    {
        for (size_t i = 0; i < len_; i++)
        {
            std::cout << data_[i] << " ";
        }
        std::cout << std::endl;
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
    vector<int> customVector;
    customVector.push_back(3);
    customVector.push_back(5);
    customVector.push_back(7);
    customVector.push_back(9);
    customVector.push_back(9);
    customVector.print_vector();
    std::cout << "capacity is " << customVector.capacity() << std::endl;
    std::cout << "size is " << customVector.size() << std::endl;
    return 0;
}
// size(), capacity(), operator[] (unchecked), push_back, pop_back.
