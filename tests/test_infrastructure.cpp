#include <gtest/gtest.h>
#include "Link.h"
#include "NetworkSimulator.h"
#include "Node.h"
#include "IP.h"

// --- Тесты для класса Link ---
TEST(LinkTest, TransmissionAndDelay) {
    Node src("Source", "10.0.0.1");
    Node dst("Dest", "10.0.0.2");
    EventQueue eq;
    Packet pkt(1, "10.0.0.1", "10.0.0.2", 64, 100);

    // Case 1: Инициализация линка с задержкой 3.5 секунды
    Link link(&src, &dst, 3.5);

    // Case 2: Передача без протокола (чистый пакет)
    testing::internal::CaptureStdout();
    link.transmit(pkt, eq);
    std::string out_raw = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(out_raw.find("без L2 заголовков") != std::string::npos);

    // Убеждаемся, что событие добавилось в очередь с правильным временем
    EXPECT_EQ(eq.size(), 1);

    // Case 3: Передача с установленным протоколом (Стратегия)
    link.setProtocol(std::make_shared<IP>());
    testing::internal::CaptureStdout();
    link.transmit(pkt, eq);
    std::string out_proto = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(out_proto.find("IPv4_Header") != std::string::npos);
    EXPECT_EQ(eq.size(), 2);

    // Case 4: Выполнение события доставки (должно сработать на метке 3.5 с.)
    eq.processNext();
    EXPECT_DOUBLE_EQ(eq.getCurrentTime(), 3.5);
}

// --- Тесты для класса NetworkSimulator ---
TEST(NetworkSimulatorTest, CoreOrchestration) {
    NetworkSimulator sim;

    // Case 1: Добавление узлов
    sim.addNode(std::make_unique<Node>("N1", "1.1.1.1"));
    sim.addNode(std::make_unique<Node>("N2", "2.2.2.2"));

    // Case 2: Очередь событий изначально пуста
    EXPECT_EQ(sim.getQueue().size(), 0);

    // Case 3: Имитация планирования событий в симулятор
    sim.getQueue().schedule(5.0, []() { /* dummy */ });
    sim.getQueue().schedule(1.0, []() { /* dummy */ });
    EXPECT_EQ(sim.getQueue().size(), 2);

    // Case 4: Запуск симулятора (run() должен обработать всё)
    testing::internal::CaptureStdout();
    sim.run();
    std::string out_run = testing::internal::GetCapturedStdout();

    // Проверяем, что симулятор отработал и время изменилось хронологически
    EXPECT_TRUE(out_run.find("Системное время: 1 c.") != std::string::npos);
    EXPECT_TRUE(out_run.find("Системное время: 5 c.") != std::string::npos);
    EXPECT_TRUE(out_run.find("Очередь событий пуста") != std::string::npos);

    // Case 5: После run() очередь должна быть пуста
    EXPECT_EQ(sim.getQueue().size(), 0);
}