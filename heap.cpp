#include <iostream>
#include <vector>
#include <queue>

/*
Heap stores elements in a vector, the root node will be at index 0, and for each element, the leftChild would be at (2 * i + 1) and the rightChild would be
at (2 * i + 2). To get the parent, we can then get (i - 1) / 2.

Method to implement: Push(int elem), pop(), top(),
*/

class MinHeap
{
public:
    MinHeap() {}

    /* The top element would be the smallest, and if we push, we will push to the last element. From the last element, we want to check its parent, and if it is
smaller, we will swap the elements, and continue traversing up the minHeap.*/
    void push(int elem)
    {
        vec.push_back(elem);
        int currIndex = vec.size() - 1;
        while (currIndex > 0)
        {
            // get its parent
            int parentIndex = (currIndex - 1) / 2;
            if (vec[currIndex] < vec[parentIndex])
            {
                int temp = vec[currIndex];
                vec[currIndex] = vec[parentIndex];
                vec[parentIndex] = temp;
                currIndex = parentIndex;
            }
            else
            { // no longer need to heapify
                break;
            }
        }
    }

    /*
    remove the top element, which essentially what it does is to swap the last element to the front of the vector, and then do a pop_back(), and then
    traverse downwards and ensure that the currentIndex is indeed smaller than its two children.

    So it is possible that it is smaller than both children, in this case we should take the smaller of the two, set it to the parent, and continue recursing
    on the tree that is smaller.
    */

    void pop()
    {
        if (size() == 0)
        {
            std::cout << "heap is empty" << std::endl;
            return;
        }
        vec[0] = vec[vec.size() - 1];
        vec.pop_back();
        heapify(0);
    }

    void heapify(int index)
    {
        int leftChild = (2 * index) + 1;
        int rightChild = (2 * index) + 2;
        int smallestIndex = leftChild < vec.size() && vec[leftChild] < vec[index] ? leftChild : index;
        smallestIndex = rightChild < vec.size() && vec[rightChild] < vec[smallestIndex] ? rightChild : smallestIndex;
        if (smallestIndex == index)
        { // do not need to heapify
            return;
        }

        int temp = vec[index];
        vec[index] = vec[smallestIndex];
        vec[smallestIndex] = temp;
        heapify(smallestIndex);
    }

    int top()
    {
        if (vec.empty())
        {
            throw std::out_of_range("Heap is empty");
        }
        return vec[0];
    }

    int size() const
    {
        return vec.size();
    }

    void printHeap()
    {
        for (const auto num : vec)
        {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

private:
    std::vector<int> vec;
};

int main()
{
    MinHeap heap;
    heap.push(4);
    heap.push(2);
    heap.push(3);

    std::cout << heap.top() << std::endl;                // should be 2
    std::cout << "size is " << heap.size() << std::endl; // should be 3
    heap.pop();
    std::cout << heap.top() << std::endl;                // should be 3
    std::cout << "size is " << heap.size() << std::endl; // should be 2
    heap.pop();
    std::cout << heap.top() << std::endl; // should be 4
}

// g++ heap.cpp -o output && ./output && rm output