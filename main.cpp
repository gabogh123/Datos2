

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion");
    MainWindow w;
    w.adjustSize();
    w.resize(1000,1000);
    w.show();
    return a.exec();
}
