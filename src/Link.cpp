#include "Link.h"
#include <iostream>

Link::Link(Node* source, Node* destination, double propagationDelay)
    : src(source), dst(destination), delay(propagationDelay) {
}

void Link::setProtocol(std::shared_ptr<Protocol> proto) {
    protocolStrategy = proto;
}

void Link::transmit(const Packet& pkt, EventQueue& queue) {
    if (protocolStrategy) {
        Frame f = protocolStrategy->encapsulate(pkt);
        std::cout << "[Канал " << src->getName() << " -> " << dst->getName()
            << "] Передача инкапсулированного фрейма (" << f.protocolHeader << ")" << std::endl;
    }
    else {
        std::cout << "[Канал " << src->getName() << " -> " << dst->getName()
            << "] Передача чистого пакета без L2 заголовков" << std::endl;
    }

    // Симуляция задержки: узел-получатель обработает пакет через время 'delay'
    // Захват dst по значению, чтобы избежать обращения к уничтоженному this
    queue.schedule(delay, [dst = this->dst, pkt]() {
        if (dst) {
            dst->receive(pkt);
        } else {
            std::cerr << "[Канал] Ошибка: dst == nullptr, пакет отброшен" << std::endl;
        }
    });
}