#pragma once
#include <string>
#include <map>

class RoutingTable {
private:
    // Пара: Сеть/Маска -> Шлюз (Next Hop)
    std::map<std::string, std::string> table;

public:
    void insert(const std::string& network, const std::string& nextHop);
    std::string lookup(const std::string& ip) const;
    void clear();
    size_t size() const;
};