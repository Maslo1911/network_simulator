#pragma once
#include <string>
#include <vector>

class Packet {
private:
    uint32_t id;
    std::string srcIP;
    std::string dstIP;
    int ttl;
    size_t size;

public:
    Packet(uint32_t packetId, std::string src, std::string dst, int timeToLive, size_t packetSize);

    bool decrementTTL();
    int getTTL() const;
    uint32_t getId() const;
    std::string getSrcIP() const;
    std::string getDstIP() const;
    size_t getSize() const;
    std::vector<Packet> fragment(size_t mtu) const;
};