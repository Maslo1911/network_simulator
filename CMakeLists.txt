cmake_minimum_required(VERSION 3.16) # Рекомендуется 3.16+ для полноценной работы с Qt6
project(NetworkSimulator VERSION 1.0 LANGUAGES CXX)

# Включаем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем автоматическую генерацию файлов Qt (MOC, UIC, RCC)
# Важно: эти строки должны идти ДО find_package или создания целей
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Ищем Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# 1. Список файлов для ядра симулятора (библиотеки)
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

# Указываем пути к заголовкам для библиотеки (все, кто линкуется с ней, тоже их увидят)
target_include_directories(sim_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 3. Создаем консольное приложение (CLI-версия)
add_executable(network_sim
    src/main.cpp
    ${CORE_SOURCES}
)
target_link_libraries(network_sim PRIVATE sim_core)

# 4. Создаем графическое приложение (GUI-версия)
set(GUI_SOURCES
    src/main_gui.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ${CORE_SOURCES}
)

# WIN32 скрывает консольное окно на Windows при запуске GUI приложения
add_executable(network_sim_gui WIN32 ${GUI_SOURCES})

# ИСПРАВЛЕНИЕ: Вместо дублирования исходников просто линкуем ядро sim_core!
target_link_libraries(network_sim_gui PRIVATE sim_core Qt6::Widgets)

# Подключаем тесты (если папка tests существует)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
    add_subdirectory(tests)
endif()
