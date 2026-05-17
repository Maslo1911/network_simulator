#include "Packet.h"
#include <stdexcept>

Packet::Packet(uint32_t packetId, std::string src, std::string dst, int timeToLive, size_t packetSize)
    : id(packetId), srcIP(src), dstIP(dst), ttl(timeToLive), size(packetSize) {
    if (timeToLive < 0) throw std::invalid_argument("TTL cannot be negative");
    if (packetSize == 0) throw std::invalid_argument("Size must be greater than 0");
}

bool Packet::decrementTTL() {
    if (ttl <= 1) {
        ttl = 0;
        return false; // Пакет должен быть уничтожен
    }
    ttl--;
    return true;
}

int Packet::getTTL() const { return ttl; }
uint32_t Packet::getId() const { return id; }
std::string Packet::getDstIP() const { return dstIP; }
std::string Packet::getSrcIP() const { return srcIP; }
size_t Packet::getSize() const { return size; }

std::vector<Packet> Packet::fragment(size_t mtu) const {
    if (mtu == 0) throw std::invalid_argument("MTU cannot be zero");
    std::vector<Packet> fragments;
    if (size <= mtu) {
        fragments.push_back(*this);
        return fragments;
    }

    size_t remaining = size;
    uint32_t fragId = id * 1000;
    while (remaining > 0) {
        size_t currentSize = (remaining > mtu) ? mtu : remaining;
        fragments.emplace_back(fragId++, srcIP, dstIP, ttl, currentSize);
        remaining -= currentSize;
    }
    return fragments;
}