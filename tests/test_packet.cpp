#include <gtest/gtest.h>
#include "Packet.h"

// Тест-сеть для метода декремента TTL (7 тестов)
TEST(PacketTest, DecrementTTL_Subcases) {
    // Case 1: Стандартный декремент
    Packet p1(1, "10.0.0.1", "10.0.0.2", 64, 1500);
    EXPECT_TRUE(p1.decrementTTL());
    EXPECT_EQ(p1.getTTL(), 63);

    // Case 2: Граничное состояние TTL = 2 (уменьшается до 1, жить остается)
    Packet p2(2, "10.0.0.1", "10.0.0.2", 2, 1500);
    EXPECT_TRUE(p2.decrementTTL());
    EXPECT_EQ(p2.getTTL(), 1);

    // Case 3: Граничное состояние TTL = 1 (умирает при попытке декремента)
    Packet p3(3, "10.0.0.1", "10.0.0.2", 1, 1500);
    EXPECT_FALSE(p3.decrementTTL());
    EXPECT_EQ(p3.getTTL(), 0);

    // Case 4: Экстремально высокое начальное значение TTL
    Packet p4(4, "10.0.0.1", "10.0.0.2", 9999, 500);
    EXPECT_TRUE(p4.decrementTTL());
    EXPECT_EQ(p4.getTTL(), 9998);

    // Case 5: Исключительная ситуация - генерация с отрицательным TTL
    EXPECT_THROW(Packet(5, "10.0.0.1", "10.0.0.2", -5, 100), std::invalid_argument);

    // Case 6: Повторный декремент уничтоженного пакета
    Packet p6(6, "10.0.0.1", "10.0.0.2", 1, 100);
    p6.decrementTTL(); // стал 0
    EXPECT_FALSE(p6.decrementTTL()); // проверка стабильности состояния
    EXPECT_EQ(p6.getTTL(), 0);

    // Case 7: Создание пакета с нулевым TTL
    Packet p7(7, "10.0.0.1", "10.0.0.2", 0, 100);
    EXPECT_FALSE(p7.decrementTTL());
}

// Тест-сеть для функции фрагментации пакета (6 тестов)
TEST(PacketTest, Fragment_Subcases) {
    Packet base(100, "192.168.1.1", "192.168.1.2", 10, 3000);

    // Case 1: MTU больше размера пакета (нет деления)
    auto frags1 = base.fragment(4000);
    ASSERT_EQ(frags1.size(), 1);
    EXPECT_EQ(frags1[0].getTTL(), 10);

    // Case 2: Точное деление пакета пополам
    auto frags2 = base.fragment(1500);
    ASSERT_EQ(frags2.size(), 2);
    EXPECT_EQ(frags2[0].getTTL(), 10);

    // Case 3: Некратное деление с остатком
    auto frags3 = base.fragment(1200);
    ASSERT_EQ(frags3.size(), 3); // 1200 + 1200 + 600

    // Case 4: Экстремально мелкий MTU
    Packet smallPacket(101, "1.1.1.1", "2.2.2.2", 5, 10);
    auto frags4 = smallPacket.fragment(2);
    EXPECT_EQ(frags4.size(), 5);

    // Case 5: Ошибочный параметр MTU = 0
    EXPECT_THROW(base.fragment(0), std::invalid_argument);

    // Case 6: Проверка генерации невалидного размера пакета
    EXPECT_THROW(Packet(102, "1.1.1.1", "2.2.2.2", 5, 0), std::invalid_argument);
}