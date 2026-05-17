#pragma once
#include "Node.h"
#include "RoutingTable.h"

class Router : public Node {
private:
    RoutingTable routingTable;

public:
    Router(std::string nodeName, std::string ip);

    void addRoute(const std::string& network, const std::string& nextHop);
    void receive(const Packet& pkt) override;
};