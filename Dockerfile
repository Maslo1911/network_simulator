# ==========================================
# СТАДИЯ 1: Сборка и тестирование (builder)
# ==========================================
FROM ubuntu:22.04 AS builder

# Отключение интерактивных диалогов при установке пакетов
ENV DEBIAN_FRONTEND=noninteractive

# Установка всех необходимых зависимостей (C++17, CMake, Git)
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    make \
    git \
    && rm -rf /var/lib/apt/lists/*

# Создание рабочей директории
WORKDIR /app

# Копирование структуры проекта
COPY CMakeLists.txt .
COPY include/ ./include/
COPY src/ ./src/
COPY tests/ ./tests/

# Генерация файлов сборки CMake с включением флага тестирования
RUN cmake -S . -B build -DBUILD_TESTING=ON

# Компиляция основного проекта и тестов
RUN cmake --build build

# Запуск системы unit-тестов внутри изолированного контейнера.
# В случае падения тестов процесс сборки образа завершится с ошибкой (код != 0)
WORKDIR /app/build
RUN ctest --output-on-failure


# ==========================================
# СТАДИЯ 2: Легковесный образ (runtime)
# ==========================================
FROM ubuntu:22.04 AS runtime

# Установка базовых библиотек для запуска C++ программ
RUN apt-get update && apt-get install -y \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Копирование только бинарных файлов из стадии 'builder'
COPY --from=builder /app/build/network_sim .
COPY --from=builder /app/build/tests/run_unit_tests ./tests/

# Настройка точки входа. Позволяет передавать аргументы командной строки
# из хост-машины напрямую в симулятор (например: docker run img --nodes 100)
ENTRYPOINT ["./network_sim"]

# Аргументы по умолчанию (если пользователь запустил контейнер без флагов)
CMD ["--help"]