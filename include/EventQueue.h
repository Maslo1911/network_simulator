#pragma once
#include <functional>
#include <queue>
#include <vector>

struct Event {
    double time;
    std::function<void()> action;

    bool operator>(const Event& other) const {
        return time > other.time;
    }
};

class EventQueue {
private:
    std::priority_queue<Event, std::vector<Event>, std::greater<Event>> queue;
    double currentTime = 0.0;

public:
    void schedule(double delay, std::function<void()> action);
    bool processNext();
    double getCurrentTime() const;
    void clear();
    size_t size() const;
};