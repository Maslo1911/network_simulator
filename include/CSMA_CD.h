#pragma once
#include "Protocol.h"

class CSMA_CD : public Protocol {
public:
    Frame encapsulate(const Packet& pkt) override;
};