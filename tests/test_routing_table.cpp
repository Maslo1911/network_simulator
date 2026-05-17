#include <gtest/gtest.h>
#include "RoutingTable.h"

// Тест-сеть для проверки алгоритма битового LPM (Longest Prefix Match)
TEST(RoutingTableTest, Lookup_Subcases) {
    RoutingTable rt;
    rt.insert("192.168.0.0/16", "Router_A");
    rt.insert("192.168.1.0/24", "Router_B");
    rt.insert("10.5.5.5/32", "Host_Specific"); // Специфичный хост
    rt.insert("0.0.0.0/0", "Gateway_Default");

    // Case 1: Точное попадание в узкую подсеть /24 (должен выбрать Router_B, а не Router_A)
    EXPECT_EQ(rt.lookup("192.168.1.5"), "Router_B");
    EXPECT_EQ(rt.lookup("192.168.1.255"), "Router_B");

    // Case 2: Попадание в широкую подсеть /16 (выходит за рамки /24)
    EXPECT_EQ(rt.lookup("192.168.5.12"), "Router_A");
    EXPECT_EQ(rt.lookup("192.168.0.1"), "Router_A");

    // Case 3: Точное совпадение с маской /32 (конкретный IP)
    EXPECT_EQ(rt.lookup("10.5.5.5"), "Host_Specific");
    EXPECT_EQ(rt.lookup("10.5.5.6"), "Gateway_Default"); // Соседний IP уходит на дефолт

    // Case 4: Использование дефолтного шлюза (0.0.0.0/0)
    EXPECT_EQ(rt.lookup("8.8.8.8"), "Gateway_Default");

    // Case 5: Защита от некорректных IP-адресов в lookup
    EXPECT_EQ(rt.lookup("invalid_ip"), "DROP");
    EXPECT_EQ(rt.lookup("256.256.256.256"), "DROP"); // Превышение байта
    EXPECT_EQ(rt.lookup("192.168.1"), "DROP");       // Неполный адрес

    // Case 6: Защита от некорректных CIDR в таблице
    rt.insert("999.999.999.999/40", "Bad_Route"); // Не должно сломать поиск
    EXPECT_EQ(rt.lookup("192.168.1.5"), "Router_B"); // Нормальный роутинг всё ещё работает

    // Case 7: Проверка работы без дефолтного шлюза
    RoutingTable strictRt;
    strictRt.insert("10.0.0.0/8", "Internal");
    EXPECT_EQ(strictRt.lookup("192.168.1.1"), "DROP"); // Дефолта нет, подсеть не совпала

    // Case 8: Очистка таблицы
    rt.clear();
    EXPECT_EQ(rt.size(), 0);
    EXPECT_EQ(rt.lookup("192.168.1.5"), "DROP"); // Таблица пуста, всё дропается
}