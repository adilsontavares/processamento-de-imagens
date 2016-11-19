#include <QApplication>

#include <iostream>

#include "MainWindow.hpp"
#include "ImageFilterConfig.hpp"

#include "InputWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ImageFilterConfig::configure();

    auto window = new MainWindow();
    window->resize(800, 600);
    window->setWindowTitle("Adilson Matheus Kalva Tavares");
    window->setMinimumSize(800, 500);
    window->show();

    return app.exec();
}
