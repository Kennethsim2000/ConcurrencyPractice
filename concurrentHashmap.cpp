#include <vector>
#include <shared_mutex>
#include <unordered_map>
#include <mutex>
#include <iostream>
#include <thread>

using namespace std;

template <typename k, typename v, size_t BUCKETS>
class ConcurrentHashMap
{
private:
    struct Bucket
    { // need to define a struct for each individual bucket
        shared_mutex mtx;
        unordered_map<k, v> map;
    };

    vector<Bucket> buckets;

    Bucket &getBucket(const k &key)
    {
        hash<k> h;
        size_t bucket = h(key) % BUCKETS;
        return buckets[bucket];
    }

public:
    ConcurrentHashMap() : buckets(BUCKETS) {}

    void insert(k &key, v &value)
    {
        Bucket &b = getBucket(key); // obtain the reference of the bucket
        unique_lock lock(b.mtx);    // lock the bucket mutex
        unordered_map<k, v> &map = b.map;
        map.insert({key, value});
    }

    void removeKey(k &key)
    {
        Bucket &b = getBucket(key);
        unique_lock lock(b.mtx);
        unordered_map<k, v> &map = b.map;
        map.erase(key);
    }

    v &get(k &key)
    {
        Bucket &b = getBucket(key);
        shared_lock lock(b.mtx);
        unordered_map<k, v> &map = b.map;
        auto it = map.find(key);
        if (it != map.end())
        {
            return it->second;
        }
        throw runtime_error("Key not found");
    }
};

int main()
{
    // Write C++ code here
    ConcurrentHashMap<int, string, 10> map;
    auto insertTask = [&map](int key, string value)
    {
        map.insert(key, value);
    };

    auto readTask = [&map](int key)
    {
        try
        {
            cout << "Value for key " << key << " in thread: " << map.get(key) << endl;
        }
        catch (const runtime_error &e)
        {
            cout << "Key " << key << " not found in thread" << endl;
        }
    };

    thread t1(insertTask, 3, "Value3");
    thread t2(readTask, 3);
    thread t3(insertTask, 4, "Value4");
    thread t4(readTask, 4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    return 0;
}

// use generics, key K, value V, template
// has a struct for Buckets
// this concurrentHashMap contains a vector<Bucket>
// within each Bucket, there is a mtx inside the struct
// insert which takes in a K& key and a V& value, delete which takes in a K&, and get which takes in a
// k& key and v& value to move the value inside
// getBucket() which make use of std::hash to hash the K& key