#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // 1. Инициализация графической сцены (холста)
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing); // Сглаживание

    // 2. Создание элементов управления
    btnNextStep = new QPushButton("Следующий шаг", this);
    btnAutoPlay = new QPushButton("Авто-запуск", this);
    logConsole = new QTextEdit(this);
    logConsole->setReadOnly(true);

    // 3. Верстка интерфейса (Layouts)
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    buttonLayout->addWidget(btnNextStep);
    buttonLayout->addWidget(btnAutoPlay);
    
    mainLayout->addWidget(view, 4);       // Холст занимает 80% пространства
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(logConsole, 1);  // Консоль логов занимает 20%

    setCentralWidget(centralWidget);
    resize(800, 600);
    setWindowTitle("Симулятор компьютерной сети");

    // 4. Настройка таймера для авто-игры
    autoPlayTimer = new QTimer(this);

    // 5. Соединение сигналов кнопок со слотами обработки
    connect(btnNextStep, &QPushButton::clicked, this, &MainWindow::onNextStep);
    connect(btnAutoPlay, &QPushButton::clicked, this, &MainWindow::onAutoPlayToggle);
    connect(autoPlayTimer, &QTimer::timeout, this, &MainWindow::onNextStep);

    // 6. Построение тестовой топологии
    setupTopology();
}

MainWindow::~MainWindow() {}

void MainWindow::setupTopology() {
    // Демонстрационное добавление узлов на холст
    // В реальном проекте вы можете обходить sim.getNodes() и вычислять их координаты
    
    // Рисуем Маршрутизатор А (круг)
    QGraphicsEllipseItem *routerA = scene->addEllipse(100, 200, 50, 50, QPen(Qt::black), QBrush(Qt::blue));
    scene->addText("Router_A")->setPos(100, 175);

    // Рисуем Маршрутизатор Б
    QGraphicsEllipseItem *routerB = scene->addEllipse(400, 200, 50, 50, QPen(Qt::black), QBrush(Qt::green));
    scene->addText("Router_B")->setPos(400, 175);

    // Рисуем линк между ними (линия)
    scene->addLine(150, 225, 400, 225, QPen(Qt::red, 2));
    
    logConsole->append("Топология сети успешно инициализирована.");
}

void MainWindow::onNextStep() {
    // Проверяем, есть ли события в вашей EventQueue
    if (sim.getQueue().size() > 0) {
        // Чтобы логи выводились в интерфейс, временно перехватим вывод, 
        // либо вызовем метод обработки, который возвращает строку лога.
        
        bool success = sim.getQueue().processNext();
        if (success) {
            double currentTime = sim.getQueue().getCurrentTime();
            logConsole->append(QString("Системное время: %1 c. Обработано событие.").arg(currentTime));
            
            // Тут можно добавить логику изменения цвета линка, 
            // если по нему летит пакет, или рисовать летящий кружок пакета!
        }
    } else {
        logConsole->append("Очередь событий пуста. Симуляция завершена.");
        autoPlayTimer->stop();
        btnAutoPlay->setText("Авто-запуск");
    }
}

void MainWindow::onAutoPlayToggle() {
    if (autoPlayTimer->isActive()) {
        autoPlayTimer->stop();
        btnAutoPlay->setText("Авто-запуск");
    } else {
        autoPlayTimer->start(500); // Шаг каждые 500 мс
        btnAutoPlay->setText("Пауза");
    }
}

void MainWindow::updateSimulationTime() {
    // Пока оставим метод пустым, чтобы проект успешно собирался.
    // В будущем здесь можно обновлять текстовый счётчик времени на экране:
    // double t = sim.getQueue().getCurrentTime();
    // timeLabel->setText(QString("Текущее время: %1 сек").arg(t));
}