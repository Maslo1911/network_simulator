#pragma once
#include "Packet.h"

struct Frame {
    std::string protocolHeader;
    Packet payload;
};

// Абстрактный класс стратегии инкапсуляции протокола
class Protocol {
public:
    virtual ~Protocol() = default;
    virtual Frame encapsulate(const Packet& pkt) = 0;
};