#include "mainwindow.h"

#include <QApplication>

/**
 * @brief main
 * Función main que corre el programa
 * @param argc
 * @param argv
 * @return
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w; /// Se inicializa la ventana principal
    w.adjustSize(); /// Da a la ventana la capacidad de ajustar el tamaño
    w.resize(1000,1000); /// Se cambia el tamaño de la ventana al hacerla pequeña
    w.show(); /// Se muestra la ventana principal
    return a.exec(); /// Ejecuta el programa
}
