
#include "./H_files/Utils/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    QObject::connect(&a, &QApplication::aboutToQuit, &w, &MainWindow::logout);

    w.show();
    return a.exec();
}
