#include <gtest/gtest.h>
#include "Packet.h"
#include "RoutingTable.h"
#include "EventQueue.h"

TEST(IntegrationScenario, MultiHopRoutingAndTTLDrop) {
    EventQueue simEngine;
    RoutingTable nodeA_Table;
    RoutingTable nodeB_Table;

    // Конфигурация топологии: NodeA -> NodeB -> NodeC
    nodeA_Table.insert("10.0.2.0/24", "Link_To_B");
    nodeB_Table.insert("10.0.2.0/24", "Link_To_C");

    // Создаем отправляемый пакет, адресованный NodeC (10.0.2.55) с начальным TTL = 2
    Packet testPacket(777, "10.0.1.5", "10.0.2.55", 2, 500);

    bool reachedB = false;
    bool reachedC = false;

    // Шаг 1: Инициация отправки с узла А (Задержка передачи по кабелю 1.5 сек)
    simEngine.schedule(1.5, [&]() {
        std::string nextHop = nodeA_Table.lookup(testPacket.getDstIP());
        if (nextHop == "Link_To_B") {
            if (testPacket.decrementTTL()) {
                reachedB = true; // Пакет успешно дошел до интерфейса B
            }
        }
        });

    // Шаг 2: Маршрутизация с узла B далее к C (Вторая задержка 2.0 сек)
    simEngine.schedule(3.5, [&]() {
        if (reachedB) {
            std::string nextHop = nodeB_Table.lookup(testPacket.getDstIP());
            if (nextHop == "Link_To_C") {
                // Здесь декремент должен вернуть false, так как TTL стал равен 1
                if (testPacket.decrementTTL()) {
                    reachedC = true;
                }
            }
        }
        });

    // Запуск конвейера симуляции событий
    while (simEngine.processNext()) {}

    // Верификация результатов интеграционного теста
    EXPECT_TRUE(reachedB);               // До узла B пакет успешно добрался
    EXPECT_FALSE(reachedC);              // До узла C пакет НЕ ДОШЕЛ, так как TTL исчерпался
    EXPECT_EQ(testPacket.getTTL(), 0);   // Значение счетчика жизни занулилось
    EXPECT_EQ(simEngine.getCurrentTime(), 3.5); // Симуляция остановилась на метке времени 3.5
}