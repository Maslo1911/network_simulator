#pragma once
#include <vector>
#include <memory>
#include "Node.h"
#include "Link.h"
#include "EventQueue.h"

class NetworkSimulator {
private:
    std::vector<std::unique_ptr<Node>> nodes;
    std::vector<std::unique_ptr<Link>> links;
    EventQueue eventQueue;

public:
    void addNode(std::unique_ptr<Node> node);
    void addLink(Node* src, Node* dst, double delay, std::shared_ptr<Protocol> proto = nullptr);
    EventQueue& getQueue();
    void run();
};