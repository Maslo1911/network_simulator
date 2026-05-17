#include "NetworkSimulator.h"
#include <iostream>

void NetworkSimulator::addNode(std::unique_ptr<Node> node) {
    nodes.push_back(std::move(node));
}

void NetworkSimulator::addLink(Node* src, Node* dst, double delay, std::shared_ptr<Protocol> proto) {
    auto link = std::make_unique<Link>(src, dst, delay);
    if (proto) {
        link->setProtocol(proto);
    }
    links.push_back(std::move(link));
}

EventQueue& NetworkSimulator::getQueue() {
    return eventQueue;
}

void NetworkSimulator::run() {
    std::cout << "=== Запуск дискретно-событийного симулятора компьютерной сети ===" << std::endl;
    while (eventQueue.processNext()) {
        std::cout << "--- [Системное время: " << eventQueue.getCurrentTime() << " c.] ---" << std::endl;
    }
    std::cout << "=== Моделирование завершено. Очередь событий пуста. ===" << std::endl;
}