#include <new>
#include <iostream>
#include <cstring>

template <typename T>
class Optional
{
public:
    Optional(T elem)
    {
        // placement new
        new (buffer) T(elem);
        isPresent = true;
        pointer = reinterpret_cast<T *>(buffer);
    }

    // empty optional
    Optional()
    {
        isPresent = false;
        pointer = nullptr;
    }

    // destructor does not need to free stack memory. Destructor is in charge of running cleanup logic, release any resources the object owns(heap memory, file handles)
    ~Optional()
    {
        if (isPresent)
        { // call the destructor of the object we own
            pointer->~T();
        }
    }

    T &operator*()
    {
        if (!isPresent)
            throw std::runtime_error("Optional is empty");
        return *pointer;
    }

    Optional(const Optional<T> &other)
    { // copy constructor
        // copy the element to our current buffer
        if (other.isPresent)
        {
            new (buffer) T(*(other.pointer));
            pointer = reinterpret_cast<T *>(buffer);
        }
        else
        {
            pointer = nullptr;
        }
        isPresent = other.isPresent;
    }

    // copy assignment
    Optional<T> &operator=(const Optional<T> &other)
    {
        if (this != &other)
        {
            if (isPresent && other.isPresent)
            {
                *pointer = *other.pointer; // directly set the value in other.pointer to pointer(copy constructor)
            }
            else if (!isPresent && other.isPresent)
            {
                new (buffer) T(*other.pointer);
                pointer = reinterpret_cast<T *>(buffer);
            }
            else if (isPresent && !other.isPresent)
            {
                pointer->~T();
                pointer = nullptr;
            }
            isPresent = other.isPresent;
        }

        return *this;
    }

    // move constructor
    // since it is not dynamically allocated memory, we will have to move that object over to our current buffer
    Optional<T>(Optional<T> &&other) noexcept
    {
        if (other.isPresent)
        {
            new (buffer) T(std::move(*other.pointer));
            pointer = reinterpret_cast<T *>(buffer);
            isPresent = true;

            // Destroy moved-from object inside other
            other.pointer->~T();
            other.pointer = nullptr;
            other.isPresent = false;
        }
        else
        {
            pointer = nullptr;
            isPresent = false;
        }
    }

    // move assignment

private:
    // allocate a char array in stack
    // The way the memory is
    alignas(T) char buffer[sizeof(T)];
    T *pointer;
    bool isPresent;
};

int main()
{
    Optional<int> hasObj(1);
    std::cout << "elem is " << *hasObj << std::endl;
    Optional<std::string> hasObj2("hello");
    std::cout << "elem is " << *hasObj2 << std::endl;
    Optional<int> copyOpt = hasObj;
    std::cout << "elem is " << *copyOpt << std::endl;
    Optional<int> hasObj3(2);
    copyOpt = hasObj3;
    std::cout << "elem is " << *copyOpt << std::endl;
    Optional<std::string> moveObj = std::move(hasObj2);
    std::cout << "elem is " << *moveObj << std::endl;
    std::cout << "elem is " << *hasObj2 << std::endl;
}

// g++ -std=c++20 -fsanitize=address -fno-omit-frame-pointer optional.cpp -o output && ./output && rm output