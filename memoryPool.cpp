#include <cstddef>
#include <cstdint>
#include <new>
#include <utility>
#include <iostream>

template <typename T, size_t N>
class ObjectPool
{
private:
    struct Node
    {
        Node *next;
    };

    alignas(T) char storage[N * sizeof(T)];
    Node *freeList;

public:
    ObjectPool()
    {
        freeList = nullptr;
        for (size_t i = 0; i < N; i++)
        {
            auto node = reinterpret_cast<Node *>(storage + i * sizeof(T));
            node->next = freeList;
            freeList = node;
        }
    }

    template <typename... Args>
    T *create(Args &&...args)
    {
        if (!freeList)
            throw std::bad_alloc();

        Node *node = freeList;
        freeList = freeList->next;

        return new (node) T(std::forward<Args>(args)...);
    }

    void destroy(T *obj)
    {
        obj->~T();
        Node *node = reinterpret_cast<Node *>(obj);
        node->next = freeList;
        freeList = node;
    }
};

struct Order
{
    uint64_t id_;
    double price_;
    uint32_t qty_;

    Order(uint64_t id, double price, uint32_t qty) : id_(id), price_(price), qty_(qty) {}
};

int main()
{
    ObjectPool<Order, 1000> pool;

    Order *o1 = pool.create(1, 100.5, 10);
    Order *o2 = pool.create(2, 101.0, 20);

    std::cout << o1->id_ << " " << o1->price_ << "\n";
    std::cout << o2->id_ << " " << o2->price_ << "\n";

    pool.destroy(o1);
    pool.destroy(o2);

    return 0;
}

// g++ -std=c++20 -fsanitize=address -fno-omit-frame-pointer memoryPool.cpp -o output && ./output && rm output