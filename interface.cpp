#include <string>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

const double PI = 3.14159;

class Shape
{
public:
    std::string name;
    virtual double getArea()
    {
        return 0;
    };
    virtual ~Shape() = default;
    virtual double getPerimeter()
    {
        return 0;
    }; // one question, what if we do not put = 0? will it override?

    void printDetails()
    {
        std::cout << "Area is " << getArea() << " and perimeter is " << getPerimeter() << std::endl;
    };
};

class Circle : public Shape
{

private:
    double radius_;

public:
    ~Circle()
    {
        std::cout << "Deconstructing circle" << std::endl;
    }
    Circle(double radius) : radius_(radius) {};

    double getArea() override // override is used for compiler checks to ensure that this method does indeed override the base method
    {
        return PI * radius_ * radius_;
    }

    double getPerimeter() override
    {
        return 2 * PI * radius_;
    }
};

class Rectangle : public Shape
{
private:
    double width_;
    double height_;

public:
    ~Rectangle()
    {
        std::cout << "Deconstructing rectangle" << std::endl;
    }
    Rectangle(double width, double height) : width_(width), height_(height) {};

    double getArea() override
    {
        return width_ * height_;
    }

    double getPerimeter() override
    {
        return 2 * (width_ + height_);
    }
};

class Square : public Shape
{
public:
    Square(double side) : side_(side) {}

    ~Square()
    {
        std::cout << "Deconstructing square" << std::endl;
    }

    double getArea() override
    {
        return side_ * side_;
    }

    double getPerimeter() override
    {
        return side_ * 4;
    }

private:
    double side_;
};

class ShapeManager
{
public:
    ShapeManager()
    {
    }
    void addShape(std::unique_ptr<Shape> shape)
    {
        shapes.push_back(std::move(shape));
    }

    double getTotalArea()
    {
        double res = 0;
        for (const auto &shape : shapes)
        {
            res += shape->getArea();
        }
        return res;
    }

    double getTotalPerimeter()
    {
        double res = 0;
        for (const auto &shape : shapes)
        {
            res += shape->getPerimeter();
        }
        return res;
    }

private:
    std::vector<std::unique_ptr<Shape>>
        shapes;
};

int main()
{
    ShapeManager manager;
    std::unique_ptr<Shape> squareObj = std::make_unique<Square>(5.0);
    squareObj->printDetails();
    std::unique_ptr<Shape> circleObj = std::make_unique<Circle>(5.0);
    circleObj->printDetails();
    std::unique_ptr<Shape> rectangleObj = std::make_unique<Rectangle>(2.0, 5.0);
    rectangleObj->printDetails();
    return 0;
}
// g++ interface.cpp -o output && ./output && rm output

/* Base Class: Shape
All shapes must:
Have a name
Be able to compute their area
Be able to compute their perimeter
Be able to print their details
The base class must:
Be abstract
Contain at least one pure virtual function
Have a virtual destructor

Derived Classes

Implement at least three:

A) Circle
Properties:
Radius
Area formula:
πr²
Perimeter formula:
2πr

B) Rectangle

Properties:
Width
Height

Area:
width × height
Perimeter:
2(width + height)


Create a manager class (e.g., ShapeManager) that:

Stores multiple shapes polymorphically

Can:
Add shapes
Remove shapes by name
Print all shapes

Compute total area of all shapes
You must store shapes via base-class pointers or smart pointers.
*/