#include <QCoreApplication>
#include <QThread>
#include "uhv4simulator.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QThread * aThread = new QThread();
    UHV4Simulator * aTest = new UHV4Simulator();
    aTest->moveToThread(aThread);
    QObject::connect(aThread, &QThread::started, aTest, &UHV4Simulator::start);
    aThread->start();
    return a.exec();
}
