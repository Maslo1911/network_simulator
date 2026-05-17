#pragma once
#include "Protocol.h"

class IP : public Protocol {
public:
    Frame encapsulate(const Packet& pkt) override;
};