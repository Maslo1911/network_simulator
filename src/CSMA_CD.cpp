#include "CSMA_CD.h"

Frame CSMA_CD::encapsulate(const Packet& pkt) {
    return Frame{ "Ethernet_II_Header[Type:0x0800,Preamble:CSMA/CD]", pkt };
}