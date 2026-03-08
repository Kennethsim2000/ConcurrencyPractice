#include <string>
#include <iostream>
#include <memory>
#include <vector>

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

    const std::shared_ptr<Vehicle> &getVehicleParked() const
    {
        return vehicle_;
    }

private:
    uint64_t spotId_;
    Size size_;
    bool isOccupied_;
    std::shared_ptr<Vehicle> vehicle_; // use a shared_ptr because the
};

class ParkingLot
{
public:
    ParkingLot(std::vector<Spot> spots) : spots_(std::move(spots)) {}

    bool parkVehicle(const std::shared_ptr<Vehicle> &ptr)
    {
        // TODO: implement
    }

    void exitVehicle(const std::shared_ptr<Vehicle> &ptr)
    {
        // TODO : implement
    }

    std::vector<Spot> availableSpots(Size size)
    {
        // TODO: implement
    }

private:
    std::vector<Spot> spots_;
};

int main()
{
}

// g++ parkingLot.cpp -o output && ./output && rm output