cmake_minimum_required(VERSION 3.15)
project(NetworkSimulator VERSION 1.0 LANGUAGES CXX)

# Включаем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Явно перечисляем ВСЕ файлы исходного кода для ядра симулятора (библиотеки)
set(CORE_SOURCES
    src/Packet.cpp
    src/RoutingTable.cpp
    src/EventQueue.cpp
    src/IP.cpp
    src/CSMA_CD.cpp
    src/Node.cpp
    src/Router.cpp
    src/Link.cpp
    src/NetworkSimulator.cpp
)

# 2. Создаем статическую библиотеку ядра
add_library(sim_core STATIC ${CORE_SOURCES})

# Указываем, где искать заголовочные файлы (.h)
target_include_directories(sim_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 3. Создаем основной исполняемый файл (точку входа)
add_executable(network_sim src/main.cpp)

# 4. Связываем исполняемый файл с нашей библиотекой ядра
target_link_libraries(network_sim PRIVATE sim_core)

# Подключаем тесты (если папка tests существует)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
    add_subdirectory(tests)
endif()