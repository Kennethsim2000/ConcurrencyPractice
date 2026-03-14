#include <string>
#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>

enum class Priority
{
    Low,
    Medium,
    High
};

struct Ticket
{
    std::string description_;
    Priority priority_;
    std::weak_ptr<User> assigned_to;
};

class User
{
    std::uint32_t userId;
    std::string name;
};

class WorkSpace // does not destructor due to RAII for std::vector
{
public:
    WorkSpace()
    {
    }

    void addUser(User &user)
    {
    }

    void addTicket(Ticket &ticket)
    {
    }

private:
    std::vector<User> users_;
    std::vector<Ticket> tickets_;
};

class Jira
{

public:
private:
};

int main()
{
}

/*
g++ Jira.cpp -o output && ./output && rm output

Creating a Jira ticketing system, Key components: Tasks, Users, Priority
*/