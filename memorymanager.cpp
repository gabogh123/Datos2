#include "memorymanager.h"

#include <QtCore>

MemoryManager::MemoryManager(QObject *parent):
    QThread(parent)
{

}

void MemoryManager::run()
{
    double vm, rss;
    proccesMemoryUsage(vm,rss);
    emit memUsage(vm);
}

void MemoryManager::proccesMemoryUsage(double& vmUsage, double& residentSet)
{
    using std::ios_base;
    using std::ifstream;
    using std::string;

    vmUsage     = 0.0;
    residentSet = 0.0;

    // 'file' stat seems to give the most reliable results
    //
    ifstream stat_stream("/proc/self/stat",ios_base::in);

    // dummy vars for leading entries in stat that we don't care about
    //
    string pid, comm, state, ppid, pgrp, session, tty_nr;
    string tpgid, flags, minflt, cminflt, majflt, cmajflt;
    string utime, stime, cutime, cstime, priority, nice;
    string O, itrealvalue, starttime;

    // the two fields we want
    //
    unsigned long vsize;
    long rss;

    stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
                >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
                >> utime >> stime >> cutime >> cstime >> priority >> nice
                >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

    stat_stream.close();

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vmUsage     = vsize / 1024.0;
    residentSet = rss * page_size_kb;
}
