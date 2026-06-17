#include <QApplication>
#include "MainWindow.h"
#include <clocale>

int main(int argc, char* argv[]) {
    std::setlocale(LC_ALL, "");

    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}