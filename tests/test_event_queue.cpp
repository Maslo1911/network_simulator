#include <gtest/gtest.h>
#include "EventQueue.h"

// Тест-сеть для планировщика событий (6 тестов)
TEST(EventQueueTest, Simulation_Subcases) {
    EventQueue eq;
    int executionCount = 0;

    // Case 1: Проверка хронологической сортировки (событие 2 ставится позже, но выполнится раньше)
    eq.schedule(10.0, [&]() { executionCount += 10; });
    eq.schedule(5.0, [&]() { executionCount += 5; });

    ASSERT_EQ(eq.size(), 2);

    // Сначала извлекается событие с delay=5.0
    EXPECT_TRUE(eq.processNext());
    EXPECT_EQ(eq.getCurrentTime(), 5.0);
    EXPECT_EQ(executionCount, 5);

    // Следом идет событие с delay=10.0
    EXPECT_TRUE(eq.processNext());
    EXPECT_EQ(eq.getCurrentTime(), 10.0);
    EXPECT_EQ(executionCount, 15);

    // Case 2: Попытка выполнения на пустой очереди
    EXPECT_FALSE(eq.processNext());

    // Case 3: Регистрация события с нулевой задержкой
    eq.schedule(0.0, [&]() { executionCount += 1; });
    EXPECT_EQ(eq.size(), 1);
    eq.processNext();
    EXPECT_EQ(eq.getCurrentTime(), 10.0); // Время не изменилось

    // Case 4: Отрицательная задержка (сбрасывается в 0.0 автоматом)
    eq.schedule(-5.0, [&]() { executionCount += 1; });
    eq.processNext();
    EXPECT_EQ(eq.getCurrentTime(), 10.0);

    // Case 5: Очистка пула событий планировщика
    eq.schedule(10.0, nullptr);
    eq.clear();
    EXPECT_EQ(eq.size(), 0);
}