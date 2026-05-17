#include "EventQueue.h"

void EventQueue::schedule(double delay, std::function<void()> action) {
    if (delay < 0.0) delay = 0.0;
    queue.push(Event{ currentTime + delay, action });
}

bool EventQueue::processNext() {
    if (queue.empty()) return false;
    Event nextEvent = queue.top();
    queue.pop();
    currentTime = nextEvent.time;
    if (nextEvent.action) {
        nextEvent.action();
    }
    return true;
}

double EventQueue::getCurrentTime() const { return currentTime; }
void EventQueue::clear() {
    while (!queue.empty()) queue.pop();
    currentTime = 0.0;
}
size_t EventQueue::size() const { return queue.size(); }