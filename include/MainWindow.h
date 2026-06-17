#pragma once

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QTimer>
#include <QTextEdit>

// Подключаем ваше ядро симулятора
#include "NetworkSimulator.h"

class MainWindow : public QMainWindow {
    Q_OBJECT // Макрос Qt для поддержки сигналов и слотов

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNextStep();       // Слот для пошагового выполнения симуляции
    void onAutoPlayToggle();  // Слот для запуска автоматической симуляции по таймеру
    void updateSimulationTime(); // Обновление UI

private:
    void setupTopology();    // Метод для создания графа сети на холсте

    // Компоненты интерфейса
    QGraphicsScene *scene;
    QGraphicsView *view;
    QPushButton *btnNextStep;
    QPushButton *btnAutoPlay;
    QTextEdit *logConsole;   // Сюда будем перенаправлять логи вместо std::cout

    // Логика
    NetworkSimulator sim;
    QTimer *autoPlayTimer;   // Таймер для автоматического продвижения времени
};