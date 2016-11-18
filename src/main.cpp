#include <QApplication>
#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.resize(800, 600);
    window.setWindowTitle("Adilson Tavares - Fast Adaptive Contrast");
    window.setMinimumSize(800, 500);
    window.show();

    return app.exec();
}
