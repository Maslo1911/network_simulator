#include <gtest/gtest.h>
#include "Node.h"
#include "Router.h"
#include "Packet.h"

// --- Тесты для базового класса Node ---
TEST(NodeTest, BasicFunctionality) {
    // Case 1: Инициализация и геттеры
    Node n("Host_A", "192.168.1.10");
    EXPECT_EQ(n.getName(), "Host_A");
    EXPECT_EQ(n.getIP(), "192.168.1.10");

    // Case 2-4: Проверка приема пакета (перехват консоли)
    Packet p(101, "10.0.0.1", "192.168.1.10", 64, 500);

    testing::internal::CaptureStdout();
    n.receive(p);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Успешно доставлен") != std::string::npos);
    EXPECT_TRUE(output.find("101") != std::string::npos);
    EXPECT_TRUE(output.find("Host_A") != std::string::npos);
}

// --- Тесты для класса Router ---
TEST(RouterTest, RoutingAndDrops) {
    Router r("Core_Router", "10.0.0.1");
    EXPECT_EQ(r.getName(), "Core_Router");

    // Настраиваем маршрут
    r.addRoute("192.168.2.0/24", "Link_To_Net2");

    // Case 1: Пакет с истекающим TTL (DROP)
    Packet p_ttl_die(201, "8.8.8.8", "192.168.2.5", 1, 100);
    testing::internal::CaptureStdout();
    r.receive(p_ttl_die);
    std::string out1 = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(out1.find("[DROP]") != std::string::npos);
    EXPECT_TRUE(out1.find("истекло") != std::string::npos || out1.find("равен 0") != std::string::npos);

    // Case 2: Успешный поиск маршрута (LPM)
    Packet p_good(202, "8.8.8.8", "192.168.2.50", 64, 100);
    testing::internal::CaptureStdout();
    r.receive(p_good);
    std::string out2 = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(out2.find("Link_To_Net2") != std::string::npos);
    EXPECT_TRUE(out2.find("Оставшийся TTL: 63") != std::string::npos || out2.find("LPM") != std::string::npos);

    // Case 3: Нет маршрута (DROP)
    Packet p_noroute(203, "8.8.8.8", "172.16.0.5", 64, 100);
    testing::internal::CaptureStdout();
    r.receive(p_noroute);
    std::string out3 = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(out3.find("DROP") != std::string::npos);
}