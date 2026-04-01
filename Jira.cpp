#include <string>
#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>
#include <chrono>
#include <unordered_map>

enum class Priority
{
    Low,
    Medium,
    High
};

using UserId = std::uint32_t;
using TicketId = std::uint32_t;

class User // a user can belong to multiple workspaces
{
public:
    User(std::uint32_t userId, std::string name) : userId_(userId), name_(std::move(name)) {}

    std::uint32_t getUserId() const
    {
        return userId_;
    }

    friend std::ostream &operator<<(std::ostream &stream, const User &user);

private:
    UserId userId_;
    std::string name_;
};

std::ostream &operator<<(std::ostream &stream, const User &user)
{
    stream << "user id is " << user.userId_ << " and name is " << user.name_;
    return stream;
}

struct Ticket
{
    TicketId ticketId_;
    std::string description_;
    Priority priority_;
    std::weak_ptr<User> assigned_to;
    std::chrono::system_clock::time_point deadline_;
};

class WorkSpace // does not destructor due to RAII for std::vector
{
public:
    WorkSpace() = default;

    void addUser(const User &user)
    {
        auto [it, inserted] = userIdToTicketsMap_.emplace(user.getUserId(), std::vector<uint32_t>{});
        if (inserted)
        {
            users_.emplace_back(user);
        }
    }

    void addUsers(const std::vector<User> &users)
    {
        for (const auto &user : users)
        {
            addUser(user);
        }
    }

    void addTicket(const Ticket &ticket)
    {
    }

    void assignTicket(std::uint32_t userId, std::uint32_t ticketId)
    {
    }

    const std::vector<User> &getUsers() const
    {
        return users_;
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
    User user1(1, "Kenneth Sim");
    User user2(2, "Thomas edison");
    WorkSpace workspace;
    workspace.addUser(user1);
    workspace.addUser(user2);
    std::vector<User> users = workspace.getUsers();
    for (const auto &user : users)
    {
        std::cout << user << std::endl;
    }
}

/*
g++ Jira.cpp -o output && ./output && rm output

Creating a Jira ticketing system, Key components: Tasks, Users, Priority

Features yet to support: Identify the tasks that is assigned to a user
*/