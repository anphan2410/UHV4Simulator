#ifndef UHV4SIMULATOR_H
#define UHV4SIMULATOR_H

#define UHV4SimulatorDbgEn 1

#include <QCoreApplication>
#include <QObject>
#include <QSerialPort>
#include <QByteArray>
#include <QTimer>
#include "anlogger.h"
#include "windowprotocol.h"


class UHV4Simulator : public QObject
{
    Q_OBJECT
public:
    explicit UHV4Simulator(QObject *parent = nullptr);
    void initSerialPort();

    void work();
    QSerialPort * SerialPort = Q_NULLPTR;
    bool isRunning = false;
    bool isTimedOut = true;
    bool isValid = true;
    QByteArray currentRead;
    QByteArray pendingSend;
    QTimer timer;
signals:

public slots:
    void start();
    void stop();
};

#endif // UHV4SIMULATOR_H
