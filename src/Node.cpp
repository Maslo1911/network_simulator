#include "Node.h"
#include <iostream>

Node::Node(std::string nodeName, std::string ip) : name(nodeName), ipAddress(ip) {}

std::string Node::getName() const { return name; }
std::string Node::getIP() const { return ipAddress; }

void Node::receive(const Packet& pkt) {
    std::cout << "[Узел " << name << "] Успешно доставлен пакет ID: "
        << pkt.getId() << " от " << pkt.getSrcIP() << std::endl;
}