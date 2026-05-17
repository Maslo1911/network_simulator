#pragma once
#include "Node.h"
#include "Packet.h"
#include "EventQueue.h"
#include "Protocol.h"
#include <memory>

class Link {
private:
    Node* src;
    Node* dst;
    double delay;
    std::shared_ptr<Protocol> protocolStrategy;

public:
    Link(Node* source, Node* destination, double propagationDelay);

    void setProtocol(std::shared_ptr<Protocol> proto);
    void transmit(const Packet& pkt, EventQueue& queue);
};