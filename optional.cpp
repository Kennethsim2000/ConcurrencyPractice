#include <new>
#include <iostream>
#include <cstring>

struct Tracker
{
    static int ctorCount;
    static int dtorCount;

    Tracker()
    {
        ++ctorCount;
    }

    Tracker(Tracker &&)
    {
        ++ctorCount;
    }

    ~Tracker()
    {
        ++dtorCount;
    }
};

int Tracker::ctorCount = 0;
int Tracker::dtorCount = 0;

template <typename T>
class Optional
{
public:
    Optional(const T &elem)
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

    Optional(T &&value)
    {
        new (buffer) T(std::move(value));
        isPresent = true;
        pointer = reinterpret_cast<T *>(buffer);
    }

    // destructor does not need to free stack memory. Destructor is in charge of running cleanup logic, release any resources the object owns(heap memory, file handles)
    ~Optional()
    {
        if (isPresent)
        { // call the destructor of the object we own
            pointer->~T();
            poitner = nullptr;
            isPresent = false;
        }
    }

    void reset() noexcept
    {
        if (isPresent)
        { // call the destructor of the object we own
            pointer->~T();
        }
    }

    T *operator->()
    {
        if (!isPresent)
            throw std::runtime_error("Optional is empty");
        return pointer;
    }

    const T *operator->() const
    {
        if (!isPresent)
            throw std::runtime_error("Optional is empty");
        return pointer;
    }

    T &operator*()
    {
        if (!isPresent)
            throw std::runtime_error("Optional is empty");
        return *pointer;
    }

    const T &operator*() const
    {
        if (!isPresent)
            throw std::runtime_error("Optional is empty");
        return *pointer;
    }

    T &value()
    {
        if (!isPresent)
            throw std::runtime_error("Optional is empty");
        return *pointer;
    }

    const T &value() const
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
                *pointer = *other.pointer; // directly set the value in other.pointer to pointer(copy assignment)
                // calling the copy assignment of T, so we do not need to destruct the current element
                // Since pointer already points to the buffer, we can just construct the element directly
            }
            else if (!isPresent && other.isPresent)
            {
                // place the new elem in the buffer
                // get the pointer to point to our buffer
                new (buffer) T(*other.pointer);
                pointer = reinterpret_cast<T *>(buffer);
            }
            else if (isPresent && !other.isPresent)
            {
                pointer->~T(); // destruct our current element(since our element belongs to our Optional)
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
            new (buffer) T(std::move(*other.pointer)); // call move to our new buffer
            pointer = reinterpret_cast<T *>(buffer);   // assign our new pointer
            isPresent = true;
            // other.pointer->~T();

            // make sure that we do not call the destructor of the other Optional as it is still a live object
            // and its destructor will run later when it goes out of scope. Calling its destructor will lead to
            // double freeing.
        }
        else
        {
            pointer = nullptr;
            isPresent = false;
        }
        // other.pointer = nullptr;
        // other.isPresent = false;
        // we also do not want to set the other Optional.isPresent = false as this means that the destructor for the other object will not free.
    }

    // move assignment
    Optional<T> &operator=(Optional<T> &&other)
    {
        if (this != &other)
        {
            // free up internal
            if (isPresent && other.isPresent)
            {
                *pointer = std::move(*other.pointer);
            }
            else if (!isPresent && other.isPresent)
            {
                new (buffer) T(std::move(*other.pointer));
                pointer = reinterpret_cast<T *>(buffer);
                isPresent = true;
            }
            else if (isPresent && !other.isPresent)
            {
                pointer->~T(); // destruct the current object
                isPresent = false;
                pointer = nullptr;
            }
            // DON'T MODIFY OTHER
            // other.pointer = nullptr;
            // other.isPresent = false;
        }
        return *this;
    }

    bool has_value() const noexcept
    {
        return isPresent;
    }

private:
    // allocate a char array in stack
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
    // std::cout << "elem is " << *hasObj2 << std::endl;

    {
        Optional<Tracker> a(Tracker{});
        Optional<Tracker> b(std::move(a));
    }

    std::cout
        << "ctors = " << Tracker::ctorCount
        << "\n"
        << "dtors = " << Tracker::dtorCount
        << "\n";
}

// g++ -std=c++20 -g -fsanitize=address -fno-omit-frame-pointer optional.cpp -o output && ./output && rm output

// EXTRA FIELDS TO ADD IN Optional
//  Bool has_value const noexcept;

// Explit operator bool() const noexcept;
// T& value();
// Const t& value() const;

// Const t& operator*() const; // non const version exists

// T* operator->();
// Const T* operator->() const;
// Void reset() noexcept;