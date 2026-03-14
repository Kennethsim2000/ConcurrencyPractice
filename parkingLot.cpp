#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <algorithm>

enum class VehicleType
{
    Motorcycle,
    Car,
    Truck
};

enum class Size
{
    Small,
    Medium,
    Large
};

class Vehicle
{
public:
    Vehicle(VehicleType type, std::string licensePlate, Size size) : type_(type), licensePlate_(std::move(licensePlate)), size_(size) {};
    // virtual constructors are not allowed in C++.
    // A virtual function call requires an existing object with a virtual pointer (vptr) that points to a virtual table (vtable) to determine the correct function to
    // call at runtime (dynamic dispatch). During the execution of a constructor, the object is still in the process of being created, and its vtable is not yet fully
    // set up for polymorphic behavior across the entire class hierarchy.
    virtual ~Vehicle() = default;

    VehicleType getVehicleType() const
    {
        return type_;
    }

    const std::string &getLicensePlate() const
    {
        return licensePlate_;
    }

    Size getSize() const
    {
        return size_;
    }

private:
    VehicleType type_;
    std::string licensePlate_;
    Size size_;
};

class Car : public Vehicle
{
public:
    Car(std::string licensePlate) : Vehicle(VehicleType::Car, std::move(licensePlate), Size::Medium) {};
    ~Car()
    {
        std::cout << "Destructing a car" << std::endl;
    }
};

class Motorcycle : public Vehicle
{
public:
    Motorcycle(std::string licensePlate) : Vehicle(VehicleType::Motorcycle, std::move(licensePlate), Size::Small) {};
    ~Motorcycle()
    {
        std::cout << "Destructing a motorcycle" << std::endl;
    }
};

class Truck : public Vehicle
{
public:
    Truck(std::string licensePlate) : Vehicle(VehicleType::Truck, std::move(licensePlate), Size::Large) {};
    ~Truck()
    {
        std::cout << "Destructing a truck" << std::endl;
    }
};

class Spot
{
public:
    Spot(uint64_t spotId, Size size) : spotId_(spotId), size_(size), isOccupied_(false) {};

    bool reserveSpot(const std::shared_ptr<Vehicle> &vec)
    {
        if (canReserve(vec->getSize()) && !isOccupied_)
        {
            isOccupied_ = true;
            vehicle_ = vec;
            return true;
        }

        return false;
    };

    bool canReserve(const Size vehicleSize) const
    {
        return size_ >= vehicleSize;
    }

    void leaveSpot()
    {
        isOccupied_ = false;
        vehicle_.reset();
    }

    bool isOccupied() const
    {
        return isOccupied_;
    }

    Size getSize() const
    {
        return size_;
    }

    const std::weak_ptr<Vehicle> &getVehicleParked() const
    {
        return vehicle_;
    }

private:
    uint64_t spotId_;
    Size size_;
    bool isOccupied_;
    std::weak_ptr<Vehicle> vehicle_; // use a shared_ptr because the vehicle object cannot be freed up till it leaves the spot
};

class ParkingLot
{
public:
    ParkingLot(std::vector<Spot> spots) : spots_(std::move(spots)) {}

    bool parkVehicle(const std::shared_ptr<Vehicle> &ptr)
    {
        int bestSpot = findBestSpot(ptr->getSize());
        if (bestSpot == -1)
        {
            return false;
        }
        Spot &spot = spots_[bestSpot]; // take the first available spot
        spot.reserveSpot(ptr);
        vehicleToLotMap[ptr->getLicensePlate()] = bestSpot;
        return true;
    }

    void exitVehicle(const std::shared_ptr<Vehicle> &ptr)
    {
        auto it = vehicleToLotMap.find(ptr->getLicensePlate());
        if (it == vehicleToLotMap.end())
        {
            throw std::out_of_range("License plate not found in parking lot");
        }
        int index = it->second;
        Spot &spot = spots_[index];
        spot.leaveSpot();
        vehicleToLotMap.erase(ptr->getLicensePlate());
    }

    int findBestSpot(Size size)
    {
        int res = -1;
        for (int i = 0; i < spots_.size(); i++)
        {
            if (spots_[i].getSize() >= size && !spots_[i].isOccupied())
            {
                if (res == -1 || spots_[i].getSize() < spots_[res].getSize())
                {
                    res = i;
                }
            }
        }

        return res;
    }

    int checkAvailableLots()
    {
        int res = 0;
        for (const auto &spot : spots_)
        {
            if (!spot.isOccupied())
            {
                res++;
            }
        }
        return res;
    }

private:
    std::vector<Spot> spots_;
    std::unordered_map<std::string, int> vehicleToLotMap;
};

int main()
{
    std::vector<Spot> spots;
    spots.emplace_back(1, Size::Large);
    spots.emplace_back(2, Size::Medium);
    ParkingLot parking(spots);
    std::shared_ptr<Truck> truck1 = std::make_shared<Truck>("truck1");
    std::shared_ptr<Car> car1 = std::make_shared<Car>("car1");
    parking.parkVehicle(car1);
    parking.parkVehicle(truck1);
    std::cout << "Number of references for car is " << car1.use_count() << std::endl;
    std::cout << "Number of references for truck is " << truck1.use_count() << std::endl;
    parking.exitVehicle(car1);
    std::cout << "Number of parking lots free is " << parking.checkAvailableLots() << std::endl;
    parking.exitVehicle(truck1);
    std::cout << "Number of parking lots free is " << parking.checkAvailableLots() << std::endl;
}

/*
Design considerations:

Vehicles are owned by the caller using std::shared_ptr<Vehicle>.
The parking lot should not own the vehicle; it only needs to reference
the parked vehicle while it occupies a spot.

Therefore, each Spot stores a std::weak_ptr<Vehicle>. This allows the
Spot to reference the vehicle without increasing the reference count
or affecting the vehicle's lifetime.

When accessing the vehicle, weak_ptr::lock() is used to safely obtain
a temporary shared_ptr. If the vehicle has already been destroyed,
lock() returns nullptr.

g++ parkingLot.cpp -o output && ./output && rm output

*/