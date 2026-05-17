cmake_minimum_required(VERSION 3.10)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# Изоляция настроек CRT для Windows среды MSVC
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Сборка исполняемого файла тестов
add_executable(run_unit_tests
    test_main.cpp
    utf8_console.cpp
    test_packet.cpp
    test_event_queue.cpp
    test_routing_table.cpp
    scenario_routing.cpp
    test_nodes.cpp
    test_protocols.cpp
    test_infrastructure.cpp
)

target_link_libraries(run_unit_tests PRIVATE 
    sim_core
    gtest
    gtest_main
)

# Ensure MSVC compiles sources as UTF-8 so string literals are interpreted correctly
target_compile_options(run_unit_tests PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/utf-8>)

# Интеграция с подсистемой CTest
add_test(NAME AllProjectTests COMMAND run_unit_tests)