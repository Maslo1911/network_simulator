#include "Router.h"
#include <iostream>

Router::Router(std::string nodeName, std::string ip) : Node(nodeName, ip) {}

void Router::addRoute(const std::string& network, const std::string& nextHop) {
    routingTable.insert(network, nextHop);
}

void Router::receive(const Packet& pkt) {
    std::cout << "[Маршрутизатор " << name << "] Получен транзитный пакет ID: " << pkt.getId() << std::endl;

    Packet mutablePkt = pkt;
    if (!mutablePkt.decrementTTL()) {
        std::cout << "  ❌ [DROP] Время жизни пакета (TTL) истекло на маршрутизаторе " << name << std::endl;
        return;
    }

    std::string targetInterface = routingTable.lookup(mutablePkt.getDstIP());
    std::cout << "  -> Поиск LPM. Назначение: " << mutablePkt.getDstIP()
        << " | Выбранный путь: " << targetInterface << " | Оставшийся TTL: " << mutablePkt.getTTL() << std::endl;
}