#include <atomic>
#include <array>
#include <cstddef>
#include <thread>
#include <iostream>

template <typename T, size_t Capacity>
class SPSCQueue
{
    static_assert((Capacity & (Capacity - 1)) == 0, "Capacity must be power of 2");

private:
    alignas(64) std::atomic<int> head_{0};
    alignas(64) std::atomic<int> tail_{0};
    std::array<T, Capacity> buffer_;

    static constexpr size_t MASK = Capacity - 1;

public:
    bool push(const T &item)
    {
        size_t tail = tail_.load(std::memory_order_relaxed);
        size_t next_tail = tail + 1;
        if (next_tail - head_.load(std::memory_order_acquire) > Capacity)
        {
            return false; // full
        }

        buffer_[tail & MASK] = item;

        tail_.store(next_tail, std::memory_order_release);
        return true;
    }

    bool pop(T &item)
    {
        size_t head = head_.load(std::memory_order_relaxed);
        if (head == tail_.load(std::memory_order_acquire))
        {
            return false; // empty
        }
        item = std::move(buffer_[head & MASK]);
        head_.store(head + 1, std::memory_order_release);
        return true;
    }

    size_t size()
    {
        return tail_.load(std::memory_order_acquire) - head_.load(std::memory_order_acquire);
    }

    bool empty() const
    {
        return size() == 0;
    }
};

int main()
{
    SPSCQueue<int, 1024> queue;
    std::thread producer([&]()
                         {
        for(int i = 0; i < 100000; ++i) {
            while(!queue.push(i)){} // spin
        } });
    std::thread consumer([&]()
                         {
        int value;
        for(int i = 0; i < 10000; ++i) {
            while(!queue.pop(value)) {} // spin
            std::cout << value << '\n';
        } });
    producer.join();
    consumer.join();
}

// g++ -std=c++20 -fsanitize=address -fno-omit-frame-pointer spsc.cpp -o output && ./output && rm output
