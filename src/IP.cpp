#include "IP.h"

Frame IP::encapsulate(const Packet& pkt) {
    return Frame{ "IPv4_Header[Proto:6,DSCP:0]", pkt };
}