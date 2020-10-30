#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H


#include <QThread>
#include <unistd.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <string>

class MemoryManager : public QThread
{
    Q_OBJECT
public:
    explicit MemoryManager(QObject *parent=0);
    void run();
    void proccesMemoryUsage(double& vmUsage, double& residentSet);

signals:
    void memUsage(double);

};

#endif // MEMORYMANAGER_H
