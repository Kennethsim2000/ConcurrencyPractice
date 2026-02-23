#include <new>
#include <iostream>

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

    ~Optional() {}

    T &operator*()
    {
        return *pointer;
    }

    Optional(const Optional &other)
    { // copy constructor
        // copy the element to our current buffer
        new (buffer) T(other.buffer);
    }

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
}

// g++ -std=c++20 -fsanitize=address -fno-omit-frame-pointer optional.cpp -o output && ./output && rm output