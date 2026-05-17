#include "RoutingTable.h"

#include <sstream>
#include <cstdint>
#include <vector>
#include <algorithm>

static bool parseIP(const std::string& s, uint32_t& out) {
    // Преобразовать dotted-decimal IPv4 в uint32_t (network order as host uint)
    std::istringstream iss(s);
    std::string token;
    uint32_t parts[4] = {0,0,0,0};
    int i = 0;
    while (std::getline(iss, token, '.') && i < 4) {
        try {
            int v = std::stoi(token);
            if (v < 0 || v > 255) return false;
            parts[i++] = static_cast<uint32_t>(v);
        } catch (...) {
            return false;
        }
    }
    if (i != 4) return false;
    out = (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8) | parts[3];
    return true;
}

static bool parseCIDR(const std::string& cidr, uint32_t& networkAddr, int& prefixLen) {
    // Ожидается формат: a.b.c.d/N
    size_t slash = cidr.find('/');
    if (slash == std::string::npos) return false;
    std::string ipPart = cidr.substr(0, slash);
    std::string prefixPart = cidr.substr(slash + 1);
    if (!parseIP(ipPart, networkAddr)) return false;
    try {
        prefixLen = std::stoi(prefixPart);
    } catch (...) {
        return false;
    }
    if (prefixLen < 0 || prefixLen > 32) return false;
    return true;
}

void RoutingTable::insert(const std::string& network, const std::string& nextHop) {
    if (network.empty() || nextHop.empty()) return;
    table[network] = nextHop;
}

std::string RoutingTable::lookup(const std::string& ip) const {
    // Корректный LPM: парсинг CIDR и сравнение по маске
    uint32_t ipAddr = 0;
    if (!parseIP(ip, ipAddr)) return "DROP";

    std::string bestMatch = "DROP";
    int bestPrefix = -1;

    for (const auto& [network, nextHop] : table) {
        // Обработка дефолтного маршрута
        if (network == "0.0.0.0/0") {
            if (bestPrefix < 0) {
                bestMatch = nextHop;
                bestPrefix = 0;
            }
            continue;
        }

        uint32_t netAddr = 0;
        int prefix = 0;
        if (!parseCIDR(network, netAddr, prefix)) continue; // некорректная запись пропускается

        uint32_t mask = (prefix == 0) ? 0u : (0xFFFFFFFFu << (32 - prefix));
        if ((ipAddr & mask) == (netAddr & mask)) {
            if (prefix > bestPrefix) {
                bestPrefix = prefix;
                bestMatch = nextHop;
            }
        }
    }

    return bestMatch;
}

void RoutingTable::clear() { table.clear(); }
size_t RoutingTable::size() const { return table.size(); }
