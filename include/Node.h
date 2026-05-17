#pragma once
#include <string>
#include "Packet.h"

class Node {
protected:
    std::string name;
    std::string ipAddress;

public:
    Node(std::string nodeName, std::string ip);
    virtual ~Node() = default;

    std::string getName() const;
    std::string getIP() const;

    virtual void receive(const Packet& pkt);
};