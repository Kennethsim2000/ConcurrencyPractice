#include <cstring>
#include <stdexcept>

class String
{
public:
    String()
    { // constructor
        str_ = nullptr;
        len_ = 0;
    }

    String(const char *str)
    { // constructor
        len_ = strlen(str);
        str_ = new char[len_ + 1];
        strcpy(str_, str);
    }

    ~String()
    {
        delete[] str_;
    }

    String(const String &other)
    { // copy constructor
        str_ = new char[other.len_ + 1];
        strcpy(str_, other.str_);
        len_ = other.len_;
    }

    String &operator=(const String &other)
    { // copy assignment
        if (this != &other)
        {
            char *tmp = new char[other.len_ + 1]; // allocate first
            strcpy(tmp, other.str_);
            delete[] str_; // only delete if alloc succeeded, new throws bad_alloc if it fails
            str_ = tmp;
            len_ = other.len_;
        }
        return *this;
    }

    // move constructor
    String(String &&other) noexcept
    {
        str_ = other.str_;
        len_ = other.len_;
        other.str_ = nullptr;
        other.len_ = 0;
    }

    // move assignment
    String &operator=(String &&other) noexcept
    {
        if (this != &other)
        {
            delete[] str_;
            str_ = other.str_;
            len_ = other.len_;
            other.str_ = nullptr;
            other.len_ = 0;
        }
        return *this;
    }

    size_t length() const
    {
        return len_;
    }

    const char *c_str() const
    { // read only view
        return str_;
    }

    char &operator[](size_t index)
    {
        if (index >= len_)
            throw std::out_of_range("Index out of range");
        return str_[index];
    }

    const char &operator[](size_t index) const
    {
        if (index >= len_)
            throw std::out_of_range("Index out of range");
        return str_[index];
    }

    String substr(size_t index, size_t len) const
    {
        if (index >= len_ || index + len > len_)
        {
            throw std::out_of_range("Index or length out of range");
        }
        char *res = new char[len + 1];
        strncpy(res, str_ + index, len);
        res[len] = '\0';
        String result(res);
        delete[] res;
        return result;
    }

private:
    char *str_;
    size_t len_;
};

int main()
{
}

// EXTRA FIELDS TO ADD IN Optional
// Optional(const T& value) //(already present, but assign a reference)
// Optional(T&& value) // construct from rvalue
//  Bool has_value const noexcept;
// Explit operator bool() const noexcept;
// T& value();
// Const t& value() const;
// Const t& operator*() const; // non const version exists
// T* operator->();
// Const T* operator->() const;
// Void reset() noexcept;
