#include <chrono>
#include <iostream>
#include <semaphore>
#include <thread>
#include <mutex>
#include <queue>
using namespace std;

mutex mtx;
counting_semaphore<10> sem{0};
queue<int> q;
vector<thread> lst;
atomic<bool> finished = false; // Use atomic to avoid reordering issues

void produce()
{
    for (int i = 0; i < 1000; i++)
    {
        {
            unique_lock<mutex> lock(mtx); // acquire the mutex
            q.push(i);
            cout << "Producer is producing " << i << endl;
        } // destroys mutex after pushing
        if (i % 10 == 0)
        {
            this_thread::sleep_for(chrono::milliseconds(10));
        }
        sem.release();
    }
    finished = true; // set finished without locking mutex, (atomic prevents data race)
}

void consume()
{
    while (true)
    {
        {
            unique_lock<mutex> lock(mtx); // acquire a lock here when accessing finished, a shared variable
            if (finished && q.empty())
            {
                cout << "finished consumption" << endl;
                break;
            }
        }

        sem.acquire();
        unique_lock<mutex> lock(mtx); // make sure to acquire sem first because if we acquire mutex and wait for sem, it may lead to deadlock
        int num = q.front();
        q.pop();
        this_thread::sleep_for(chrono::milliseconds(5)); // simulate some work
        cout << "consuming " << num << " by thread " << this_thread::get_id() << endl;
    }
}

int main()
{
    thread t1(produce);
    for (int i = 0; i < 10; i++)
    {
        thread t(consume);
        lst.push_back(move(t));
        // std::thread::thread cannot be referenced, it is a deleted function if i just push_back t
    }
    t1.join();
    for (thread &t : lst) // threads cannot be copied
    {                     // ✅ Use a reference to avoid copying
        t.join();
    }
    return 0;
}

// mutex to ensure mutual exclusion when accessing a buffer,
//  semaphore(int consumers) to track the number of events avaiable
// we can use a std::queue as the buffer

// c_cpp_properties.json is used to configures IntelliSense, include paths, and the compiler VS Code should use.
// tasks.json is used to define how your code is compiled and executed inside VS Code. Used to set compiler flags, standard versions (e.g., C++20), and output binaries.

// Experiment with producer and consumer concurrency