#include "mainwindow.h"

#include <QtGui/QApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


    MainWindow mainWindow;
    mainWindow.setOrientation(MainWindow::ScreenOrientationAuto);
    mainWindow.showExpanded();

    return app.exec();
}
