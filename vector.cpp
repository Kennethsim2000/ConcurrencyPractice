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

    vector(const vector<T> &other) // we need to allocate a buffer, do a deep copy
    {                              // copy constructor
        data_ = static_cast<T *>(::operator new(sizeof(T) * other.capacity_));
        try
        {
            for (size_t i = 0; i < other.len_; i++)
            {
                new (data_ + i) T(other.data_[i]); // calls the copy constructor of T
                                                   // data_ + i: Specifies the memory address where the new object will be constructed
            }
        }
        catch (...)
        {
            for (size_t i = 0; i < other.len_; i++)
            {
                data_[i].~T();
            }
            ::operator delete(data_);
            throw;
        }
        len_ = other.len_;
        capacity_ = other.capacity_;
    }

    vector &operator=(const vector<T> &other)
    {
        if (this != &other) // compare memory address first to see if it is the same object
        {
            // allocate new buffer first, if fail, throw exception
            T *newData_ = static_cast<T *>(::operator new(sizeof(T) * other.capacity_));
            size_t constructed = 0;
            try
            {
                for (; constructed < other.len_; constructed++)
                {
                    new (newData_ + constructed) T(other.data_[constructed]);
                }
            }
            catch (...)
            {
                for (size_t i = 0; i < constructed; i++)
                {
                    newData_[i].~T();
                }
                ::operator delete(newData_);
                throw;
            }

            // deallocate old buffer
            for (size_t i = 0; i < len_; i++)
            {
                data_[i].~T();
            }
            ::operator delete(data_);
            data_ = newData_;
            len_ = other.len_;
            capacity_ = other.capacity_;
        }
        return *this;
    }

    // move constructor
    vector(vector &&other) noexcept
    {
        data_ = other.data_;
        capacity_ = other.capacity_;
        len_ = other.len_;
        other.data_ = nullptr;
        other.len_ = 0;
        other.capacity_ = 0;
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
    vector<int> copyVector = customVector;
    vector<int> copyAssVector;
    copyAssVector.push_back(1);
    copyAssVector = customVector;
    copyAssVector.print_vector();
    copyVector.print_vector();
    customVector.print_vector();
    vector<int> movedVec = std::move(customVector);
    std::cout << "After moving" << std::endl;
    movedVec.print_vector();
    std::cout << "Undefined vector" << std::endl;
    customVector.print_vector();
    std::cout << "capacity is " << customVector.capacity() << std::endl;
    std::cout << "size is " << customVector.size() << std::endl;
    return 0;
}
// size(), capacity(), operator[] (unchecked), push_back, pop_back.

// g++ vector.cpp -o output && ./output && rm output