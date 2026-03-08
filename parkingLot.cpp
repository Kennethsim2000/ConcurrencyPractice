#include <string>
#include <iostream>

enum class VehicleType
{
    Motorcycle,
    Car,
    Truck
};

enum Size
{
    Small,
    Medium,
    Large
};

class Vehicle
{
public:
    Vehicle(VehicleType type, std::string licensePlate, Size size) : type_(type), licensePlate_(licensePlate), size_(size) {};
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
    Car(std::string licensePlate) : Vehicle(VehicleType::Car, licensePlate, Size::Medium) {};
    ~Car()
    {
        std::cout << "Destructing a car" << std::endl;
    }
};

class Motorcycle : public Vehicle
{
public:
    Motorcycle(std::string licensePlate) : Vehicle(VehicleType::Motorcycle, licensePlate, Size::Small) {};
    ~Motorcycle()
    {
        std::cout << "Destructing a motorcycle" << std::endl;
    }
};

class Truck : public Vehicle
{
public:
    Truck(std::string licensePlate) : Vehicle(VehicleType::Truck, licensePlate, Size::Large) {};
    ~Truck()
    {
        std::cout << "Destructing a truck" << std::endl;
    }
};

int main()
{
}

// g++ parkingLot.cpp -o output && ./output && rm output