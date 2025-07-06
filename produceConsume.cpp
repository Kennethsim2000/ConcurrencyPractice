#include <chrono>
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

std::mutex mtx;
std::mutex cout_mtx; // Separate mutex for cout
std::condition_variable cv;
std::queue<int> q;
std::vector<std::thread> lst;
std::atomic<bool> finished = false;

void produce()
{
    for (int i = 0; i < 10; i++)
    {
        {
            std::unique_lock<std::mutex> lock(mtx);
            q.push(i);
        }
        {
            std::unique_lock<std::mutex> cout_lock(cout_mtx);
            std::cout << "Producer is producing " << i << std::endl;
        }
        cv.notify_one(); // Wake up one waiting consumer
        
        if (i % 10 == 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    finished = true;
    cv.notify_all(); // Wake up all waiting consumers to let them exit
}

void consume()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        
        // Wait until there's work or we're finished
        cv.wait(lock, [] { return !q.empty() || finished; });
        
        // Check if we should exit
        if (finished && q.empty())
        {
            lock.unlock(); // Release main lock before cout
            std::unique_lock<std::mutex> cout_lock(cout_mtx);
            std::cout << "finished consumption by thread " << std::this_thread::get_id() << std::endl;
            break;
        }
        
        // If there's work to do
        if (!q.empty())
        {
            int num = q.front();
            q.pop();
            lock.unlock(); // Release lock while doing work
            
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            
            // Protect cout with separate mutex
            {
                std::unique_lock<std::mutex> cout_lock(cout_mtx);
                std::cout << "consuming " << num << " by thread " << std::this_thread::get_id() << std::endl;
            }
        }
    }
}

int main()
{
    std::thread t1(produce);
    for (int i = 0; i < 10; i++)
    {
        std::thread t(consume);
        lst.push_back(std::move(t));
    }
    t1.join();
    for (std::thread &t : lst)
    {
        t.join();
    }
    return 0;
}