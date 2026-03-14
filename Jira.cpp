#include <string>
#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>
#include <chrono>

enum class Priority
{
    Low,
    Medium,
    High
};

class User // a user can belong to multiple workspaces
{
    std::uint32_t userId;
    std::string name;
};

struct Ticket
{
    std::uint32_t ticketId;
    std::string description_;
    Priority priority_;
    std::weak_ptr<User> assigned_to;
    std::chrono::system_clock::time_point deadline_;
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

    void assignTicket(std::uint32_t userId, std::uint32_t ticketId)
    {
    }

    std::vector<User> getUsers()
    {
    }

    std::vector<Ticket> getTickets(Priority priority)
    {
    }

private:
    std::vector<User> users_;
    std::vector<Ticket> tickets_;
    std::unordered_map<uint32_t, std::vector<uint32_t>> userIdToTicketsMap_;
};

class Jira
{

public:
private:
    std::vector<WorkSpace> workspaces_;
};

int main()
{
}

/*
g++ Jira.cpp -o output && ./output && rm output

Creating a Jira ticketing system, Key components: Tasks, Users, Priority

Features yet to support: Identify the tasks that is assigned to a user
*/