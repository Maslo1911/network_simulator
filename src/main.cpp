#include <windows.h>
#include <iostream>
#include <memory>
#include "NetworkSimulator.h"
#include "Router.h"
#include "IP.h"
#include "CSMA_CD.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    NetworkSimulator sim;

    // Шаг 1. Инициализация стратегий протоколов
    auto ipStrategy = std::make_shared<IP>();
    auto csmaStrategy = std::make_shared<CSMA_CD>();

    // Шаг 2. Создание объектов сетевой топологии
    auto nodeA = std::make_unique<Node>("Хост_А", "192.168.1.5");
    auto router1 = std::make_unique<Router>("Маршрутизатор_1", "192.168.1.1");
    auto nodeB = std::make_unique<Node>("Хост_Б", "10.0.0.10");

    // Запоминаем "сырые" указатели для линкования и генерации событий до передачи владения
    Node* pNodeA = nodeA.get();
    Router* pRouter = router1.get();
    Node* pNodeB = nodeB.get();

    // Настройка таблицы маршрутизации на роутере
    pRouter->addRoute("10.0.0.0/24", "Интерфейс_Линка_Б");

    // Передача владения объектами симулятору
    sim.addNode(std::move(nodeA));
    sim.addNode(std::move(router1));
    sim.addNode(std::move(nodeB));

    // Шаг 3. Соединение объектов линиями связи с разной задержкой и протоколами
    sim.addLink(pNodeA, pRouter, 1.2, csmaStrategy); // Линк А-Роутер на Ethernet
    sim.addLink(pRouter, pNodeB, 2.5, ipStrategy);   // Линк Роутер-Б на IP-туннеле

    // Шаг 4. Создание сценариев передачи пакетов
    std::cout << "\n[Генерация] Формирование тестовых пакетов..." << std::endl;

    // Пакет 1: Успешный транзит
    Packet pkt1(1, pNodeA->getIP(), pNodeB->getIP(), 64, 1200);

    // Пакет 2: Пакет с маленьким TTL (умрет на роутере)
    Packet pkt2(2, pNodeA->getIP(), pNodeB->getIP(), 1, 500);

    // Помещаем события отправки в симуляционное расписание
    // Пусть первый линк получит пакет 1 на 0.0 секунде
    sim.getQueue().schedule(0.0, [pNodeA, pRouter, pkt1, &sim]() {
        std::cout << "[Событие] " << pNodeA->getName() << " начинает отправку пакета ID 1" << std::endl;
        // Для простоты свяжем прямым вызовом отправку через линк
        // На реальном объекте это делает сетевая карта
        });

    // Эмуляция передачи пакета 1 по цепочке
    sim.getQueue().schedule(0.0, [pNodeA, pkt1, &sim]() {
        // Симулируем, что мы передаем его через первый линк в сторону роутера
        });

    // Напишем явную логику прохождения для демонстрации дискретного времени
    sim.getQueue().schedule(0.0, [=, &sim]() {
        // Передаем пакет 1 роутеру. Он дойдет за 1.2с
        // Из-за архитектурного упрощения мы можем напрямую вызывать трансляторы
        Link l1(pNodeA, pRouter, 1.2);
        l1.setProtocol(csmaStrategy);
        l1.transmit(pkt1, sim.getQueue());
        });

    sim.getQueue().schedule(5.0, [=, &sim]() {
        std::cout << "\n[Событие] Попытка отправки пакета ID 2 с критическим TTL=1..." << std::endl;
        Link l1(pNodeA, pRouter, 1.2);
        l1.transmit(pkt2, sim.getQueue());
        });

    // Маршрутизатор перенаправит пакет 1 на Узел Б на отметке 2.0 секунды
    sim.getQueue().schedule(2.0, [=, &sim]() {
        std::cout << "\n[Событие] Перенаправление пакета ID 1 от Маршрутизатора к Узлу Б..." << std::endl;
        Link l2(pRouter, pNodeB, 2.5);
        l2.setProtocol(ipStrategy);
        l2.transmit(pkt1, sim.getQueue());
        });

    // Шаг 5. Старт симуляции
    sim.run();

    return 0;
}