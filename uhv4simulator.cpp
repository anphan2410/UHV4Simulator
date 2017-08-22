#include "UHV4simulator.h"

UHV4Simulator::UHV4Simulator(QObject *parent) : QObject(parent)
{
    timer.setParent(this);
    timer.setInterval(200);
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, this, [&](){isTimedOut=true;});
}

void UHV4Simulator::initSerialPort()
{
    if (SerialPort)
    {
        SerialPort->close();
        delete SerialPort;
        SerialPort = Q_NULLPTR;
    }
    SerialPort = new QSerialPort();
    SerialPort->setParent(this);
    SerialPort->setReadBufferSize(64);
    SerialPort->setDataBits(QSerialPort::Data8);
    SerialPort->setBaudRate(QSerialPort::Baud9600);
    SerialPort->setStopBits(QSerialPort::OneStop);
    SerialPort->setParity(QSerialPort::NoParity);
    SerialPort->setFlowControl(QSerialPort::NoFlowControl);
    SerialPort->setPortName("COM3");
    if (SerialPort->open(QIODevice::ReadWrite))
    {
        anIf(UHV4SimulatorDbgEn, anAck("Successfully Open A Serial Port !"));
    }
    else
    {
        anIf(UHV4SimulatorDbgEn, anWarn("Failed To Open A Serial Port !"));
    }
    QObject::connect(SerialPort, &QSerialPort::readyRead, this, [&](){
        isValid = isTimedOut;
    });
}

void UHV4Simulator::work()
{
    if (isRunning)
    {
        initSerialPort();
        while (isRunning)
        {
            qApp->processEvents();
            if (SerialPort->waitForReadyRead(500))
            {
                currentRead=SerialPort->readAll();
                while (SerialPort->waitForReadyRead(700))
                {
                    currentRead+=SerialPort->readAll();
                }
                WindowProtocol & currentWP = WindowProtocol::fromQByteArray(currentRead);
                anIf(UHV4SimulatorDbgEn,
                     anAck("New Command Received !");
                     anInfo(currentWP.getMSGMean()););
                pendingSend.clear();
                if (currentWP.getWINMean().contains("HVOnOff", Qt::CaseInsensitive))
                {
                    if (currentWP.getCOM()==0x31)//WR
                    {
                        pendingSend=currentWP.setWIN(0).setDATA(QByteArray().append(0x06)).genMSG();
                    }

                }
                else if (currentWP.getWINMean().contains("VMeasured", Qt::CaseInsensitive))
                {
                    if (currentWP.getCOM()==0x30)//RD
                        pendingSend=currentWP.setDATA(IntStr2QBArr0Pad(qrand()%10000,6)).genMSG();
                }
                else if (currentWP.getWINMean().contains("IMeasured", Qt::CaseInsensitive))
                {
                    if (currentWP.getCOM()==0x30)//RD
                        pendingSend=currentWP.setDATA(QString(QString::number(quint8(qrand()%9))+"E-"+QString::number(quint8(qrand()%10))).toUpper().toLocal8Bit()).genMSG();
                }
                else if (currentWP.getWINMean().contains("PMeasured", Qt::CaseInsensitive))
                {
                    if (currentWP.getCOM()==0x30)//RD
                        pendingSend=currentWP.setDATA(QString(QString::number(quint8(qrand()%9))+"."+QString::number(quint8(qrand()%9))+"E-"+QString::number(quint8(qrand()%99))).toUpper().toLocal8Bit()).genMSG();
                }
                isTimedOut=false;
                timer.start();
                while(!isTimedOut)
                {
                    qApp->processEvents();
                }
                if (isValid)
                {
                    SerialPort->write(pendingSend);
                    if (SerialPort->waitForBytesWritten(500))
                    {
                        anIf(UHV4SimulatorDbgEn, anAck("Successfully Reply !"));
                    }
                    else
                    {
                        anIf(UHV4SimulatorDbgEn, anWarn("Failed To Reply !"));
                    }
                    anIf(UHV4SimulatorDbgEn, anInfo(pendingSend.toHex()));
                }
                else
                {
                    anIf(UHV4SimulatorDbgEn, anWarn("Invalid Command Transmission !"));                    
                    SerialPort->write(currentWP.setWIN(0).setDATA(QByteArray().append(0x15)).genMSG());
                }
                isValid = true;
            }
        }
    }
}

void UHV4Simulator::start()
{
    isRunning = true;
    work();
}

void UHV4Simulator::stop()
{
    isRunning = false;
}
