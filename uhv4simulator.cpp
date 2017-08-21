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
                BinaryProtocol & currentWC = BinaryProtocol::FromQByteArray(currentRead);
                anIf(UHV4SimulatorDbgEn,
                     anAck("New Command Received !");
                     anInfo(currentWC.GetMessageTranslation()););
                pendingSend.clear();
                if (currentWC.GetCommand()=="HVSwitch")
                {
                    pendingSend.append(0x06);
                }
                else if ((currentWC.GetCommand()=="ReadP")||(currentWC.GetCommand()=="ReadV")||(currentWC.GetCommand()=="ReadI"))
                {
                    pendingSend=currentWC.HdrRsp().Data(qrand()%0xff).GenMsg();
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
                    SerialPort->write(QByteArray().append(0x15));
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
