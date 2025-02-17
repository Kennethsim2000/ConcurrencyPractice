#include <iostream>
#include <shared_mutex>
#include <mutex>
#include <thread>
#include <vector>
using namespace std;

int shared_resource = 0;
shared_mutex mtx;
mutex cout_mtx;

void readFn()
{
    {
        shared_lock<shared_mutex> lock(mtx); // acquire the shared lock, which allows multiple reader threads to read at once.
        lock_guard<mutex> coutMtx(cout_mtx);
        cout << "shared_resource is " << shared_resource << endl;
    }
    this_thread::sleep_for(chrono::milliseconds(100));
}

void writeFn()
{
    {
        unique_lock<shared_mutex> lock(mtx);
        shared_resource++;
        lock_guard<mutex> coutMtx(cout_mtx);
        cout << "writer has written " << shared_resource << endl;
    }
    this_thread::sleep_for(chrono::milliseconds(100));
}

int main()
{
    vector<thread> writerThreads;
    vector<thread> readerThreads;
    const size_t numWriters = 5;
    const size_t numReaders = 2;

    for (size_t i = 0; i < numWriters; i++)
    {
        writerThreads.emplace_back(thread(writeFn));
    }

    for (size_t i = 0; i < numReaders; i++)
    {
        readerThreads.emplace_back(thread(readFn));
    }

    for (auto &reader : readerThreads)
    {
        reader.join();
    }

    for (auto &writer : writerThreads)
    {
        writer.join();
    }

    return 0;
}