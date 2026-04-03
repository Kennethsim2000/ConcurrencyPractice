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
using StoryPoint = std::uint8_t;

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
    StoryPoint points_;
    std::chrono::system_clock::time_point deadline_;
};

class WorkSpace // does not destructor due to RAII for std::vector
{
public:
    WorkSpace() = default;

    void addUser(User user)
    {
        auto [it, inserted] = userIdToTicketsMap_.emplace(user.getUserId(), std::vector<uint32_t>{});
        if (inserted)
        {
            users_.emplace_back(std::move(user));
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
        ticketMap_[ticket.priority_].emplace_back(ticket);
    }

    void assignTicket(const User &user, const Ticket &ticket)
    {
        auto it = userIdToTicketsMap_.find(user.getUserId());
        if (it == userIdToTicketsMap_.end())
        {
            throw std::invalid_argument("UserId is not a valid user");
        }
        it->second.push_back(ticket.ticketId_);
    }

    const std::vector<User> &getUsers() const
    {
        return users_;
    }

    const std::vector<Ticket> &getTickets(Priority priority)
    {
        return ticketMap_[priority];
    }

private:
    std::vector<User> users_;
    std::unordered_map<Priority, std::vector<Ticket>> ticketMap_;
    std::unordered_map<UserId, std::vector<TicketId>> userIdToTicketsMap_;
};

class Jira
{

public:
private:
    std::vector<WorkSpace> workspaces_;
};

int main()
{
    UserId nextUserId = 1;
    TicketId nextTicketId = 1;
    User user1(nextUserId, "Kenneth Sim");
    nextUserId++;
    User user2(nextUserId, "Thomas edison");
    nextUserId++;

    WorkSpace workspace;
    workspace.addUser(user1);
    workspace.addUser(user2);

    const auto &users = workspace.getUsers();
    auto now = std::chrono::system_clock::now();
    auto deadline = now + std::chrono::hours(24 * 3);
    auto deadline2 = now + std::chrono::hours(24);
    Ticket tic1{nextTicketId, "Refactor code", Priority::Medium, 2, deadline};
    nextTicketId++;
    Ticket tic2{nextTicketId, "Complete migration of code", Priority::High, 5, deadline};
    nextTicketId++;
    workspace.addTicket(tic1);
    workspace.addTicket(tic2);
    for (const auto &user : users)
    {
        std::cout << user << std::endl;
    }
}

/*
g++ Jira.cpp -o output && ./output && rm output

Creating a Jira ticketing system, Key components: Tasks, Users, Priority

Features yet to support: Identify the tasks that is assigned to a user
Sort tasks by deadline

*/