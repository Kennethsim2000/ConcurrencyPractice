#include <iostream>
#include <queue>
#include <mutex>
#include <unordered_map>
#include <string>
#include <vector>
#include <thread>

struct Order
{
    double price;
    int quantity;

    Order(double p, int q) : price(p), quantity(q) {}
};

struct SellOrderComparator
{
    bool operator()(const Order &a, const Order &b)
    {
        return a.price > b.price;
    }
};

struct BuyOrderComparator
{
    bool operator()(const Order &a, const Order &b)
    {
        return a.price < b.price;
    }
};

class OrderBook
{

private:
    std::priority_queue<Order, std::vector<Order>, BuyOrderComparator> buyQueue;
    std::priority_queue<Order, std::vector<Order>, SellOrderComparator> sellQueue;
    std::mutex buyMtx;
    std::mutex sellMtx;

public:
    void placeBuyOrder(Order &buyOrder)
    {
        std::cout << "[BUY] Order received: Price " << buyOrder.price << ", Quantity " << buyOrder.quantity << "\n";
        {
            std::lock_guard<std::mutex> lock(sellMtx);

            while (buyOrder.quantity > 0 && !sellQueue.empty() && sellQueue.top().price <= buyOrder.price)
            {
                Order sellOrder = sellQueue.top();
                sellQueue.pop();

                int matchedQty = std::min(buyOrder.quantity, sellOrder.quantity);
                buyOrder.quantity -= matchedQty;
                sellOrder.quantity -= matchedQty;

                std::cout << "[MATCH] Buy Order " << buyOrder.price << " matches Sell Order " << sellOrder.price
                          << " for " << matchedQty << " units\n";

                if (sellOrder.quantity > 0)
                {
                    sellQueue.push(sellOrder); // Push back remaining sell order
                }
            }

            if (buyOrder.quantity > 0)
            {
                std::lock_guard<std::mutex> buyLock(buyMtx);
                buyQueue.push(buyOrder);
                std::cout << "[BUY] Order added to queue: Price " << buyOrder.price << ", Quantity " << buyOrder.quantity << "\n";
            }
        }
    }

    void placeSellOrder(Order &sellOrder)
    {
        std::cout << "[SELL] Order received: Price " << sellOrder.price << ", Quantity " << sellOrder.quantity << "\n";
        {
            std::lock_guard<std::mutex> lock(buyMtx);

            while (sellOrder.quantity > 0 && !buyQueue.empty() && buyQueue.top().price >= sellOrder.price)
            {
                Order buyOrder = buyQueue.top();
                buyQueue.pop();

                int matchedQty = std::min(sellOrder.quantity, buyOrder.quantity);
                sellOrder.quantity -= matchedQty;
                buyOrder.quantity -= matchedQty;

                std::cout << "[MATCH] Sell Order " << sellOrder.price << " matches Buy Order " << buyOrder.price
                          << " for " << matchedQty << " units\n";

                if (buyOrder.quantity > 0)
                {
                    buyQueue.push(buyOrder); // Push back remaining buy order
                }
            }
        }

        if (sellOrder.quantity > 0)
        {
            std::lock_guard<std::mutex> sellLock(sellMtx);
            sellQueue.push(sellOrder);
            std::cout << "[SELL] Order added to queue: Price " << sellOrder.price << ", Quantity " << sellOrder.quantity << "\n";
        }
    }

    void buyOrders(OrderBook &book)
    {
        std::vector<Order> orders = {
            {100.0, 5},
            {105.0, 10},
            {102.5, 7},
            {101.0, 8}};
        for (auto &order : orders)
        {
            book.placeBuyOrder(order);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    // Function to simulate multiple sell orders
    void sellOrders(OrderBook &book)
    {
        std::vector<Order> orders = {
            {101.0, 6},
            {103.0, 5},
            {100.5, 10},
            {99.5, 4}};
        for (auto &order : orders)
        {
            book.placeSellOrder(order);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
};

int main()
{
    OrderBook book;

    // Launch buy and sell order threads using lambda functions
    std::thread buyer([&book]()
                      { book.buyOrders(book); });

    std::thread seller([&book]()
                       { book.sellOrders(book); });

    // Wait for both threads to finish
    buyer.join();
    seller.join();

    std::cout << "Order matching completed.\n";
    return 0;
}