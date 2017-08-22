#ifndef WINDOWPROTOCOL_H
#define WINDOWPROTOCOL_H

#define WindowProtocolDbgEn 1

#include <QByteArray>
#include "anlogger.h"
#include "commonthings.h"

class WindowProtocol
{
public:
    explicit WindowProtocol();
    explicit WindowProtocol(const quint8 WPNo);

    //Get Original Protocol Parts
    quint8 getADDR() const;
    quint16 getWINCode() const;
    quint8 getCOM() const;
    const QByteArray &getDATA() const;
    quint16 getCRC() const;
    const QByteArray &getMSG() const;

    //Get Verboselly Translated Protocol Parts
    quint8 getWPNo() const;
    const QString getAddress() const;
    const QString getWINMean() const;
    const QString getCOMMean() const;
    const QString getDATAMean() const;
    const QString getMSGMean() const;
    bool isCMDFlagSet() const;



    //Set Protocol Parts
    WindowProtocol &setWPNo(const quint8 num);
    WindowProtocol &setADDR(const quint8 addr);

    WindowProtocol &setWIN(const quint16 win);
    WindowProtocol &HVOnOffCh1();
    WindowProtocol &HVOnOffCh2();
    WindowProtocol &HVOnOffCh3();
    WindowProtocol &HVOnOffCh4();
    WindowProtocol &BaudRate();
    WindowProtocol &Status();
    WindowProtocol &ErrorCode();
    WindowProtocol &ControllerModel();
    WindowProtocol &ControllerSerialNumber();
    WindowProtocol &RS485SerialAddress();
    WindowProtocol &SerialTypeSelect();
    WindowProtocol &ChannelSelection();
    WindowProtocol &UnitPressure();
    WindowProtocol &Protect();
    WindowProtocol &FixedStep();
    WindowProtocol &DeviceNumberCh1();
    WindowProtocol &PowerMaxCh1();
    WindowProtocol &VTargetCh1();
    WindowProtocol &IProtectCh1();
    WindowProtocol &SetPointCh1();
    WindowProtocol &DeviceNumberCh2();
    WindowProtocol &PowerMaxCh2();
    WindowProtocol &VTargetCh2();
    WindowProtocol &IProtectCh2();
    WindowProtocol &SetPointCh2();
    WindowProtocol &DeviceNumberCh3();
    WindowProtocol &PowerMaxCh3();
    WindowProtocol &VTargetCh3();
    WindowProtocol &IProtectCh3();
    WindowProtocol &SetPointCh3();
    WindowProtocol &DeviceNumberCh4();
    WindowProtocol &PowerMaxCh4();
    WindowProtocol &VTargetCh4();
    WindowProtocol &IProtectCh4();
    WindowProtocol &SetPointCh4();
    WindowProtocol &TempFAN();
    WindowProtocol &TempHV1();
    WindowProtocol &TempHV2();
    WindowProtocol &InterlockStatus();
    WindowProtocol &StatusSetPoint();
    WindowProtocol &TempHV3();
    WindowProtocol &TempHV4();
    WindowProtocol &VMeasuredCh1();
    WindowProtocol &IMeasuredCh1();
    WindowProtocol &PMeasuredCh1();
    WindowProtocol &VMeasuredCh2();
    WindowProtocol &IMeasuredCh2();
    WindowProtocol &PMeasuredCh2();
    WindowProtocol &VMeasuredCh3();
    WindowProtocol &IMeasuredCh3();
    WindowProtocol &PMeasuredCh3();
    WindowProtocol &VMeasuredCh4();
    WindowProtocol &IMeasuredCh4();
    WindowProtocol &PMeasuredCh4();

    WindowProtocol &setCOM(const quint8 com);
    WindowProtocol &Read();
    WindowProtocol &Write();

    WindowProtocol &setDATA(const quint8 aByte);
    WindowProtocol &setDATA(const QByteArray &data);
    WindowProtocol &setON();
    WindowProtocol &setOFF();
    WindowProtocol &clearDATA();

    WindowProtocol &setCRC(const quint16 crc);

    const QByteArray &genMSG();
    static WindowProtocol &fromQByteArray(const QByteArray &aMsg);

    WindowProtocol &setCMDFlag(const bool isACMD);

private:
    /*Define Class Constants***************************************************************************/
    //Fixed Size in Byte of Protocol Parameters (if provided)
    static constexpr const quint8 szSTX = 1;
    static constexpr const quint8 szADDR = 1;
    static constexpr const quint8 szWIN = 3;
    static constexpr const quint8 szCOM = 1;
    static constexpr const quint8 szETX = 1;
    static constexpr const quint8 szCRC = 2;

    //Protocol Parameter Offset (if provided)
    static constexpr const quint8 OffsetADDR = 0x80;

    //WIN Set Indexed In Two Fields: WIN Meaning & WIN Code, Optimizing For Fast Performance
    //All Details About WINs Defined In Source File
    static const QHash<QString, quint16> &WINMean2WINCode;
    static const QHash<quint16, QString> &WINCode2WINMean;

    //Miscellany (Just A Bunch Of Other Constants)
    static constexpr const quint8 RD = 0x30;
    static constexpr const quint8 WR = 0x31;
    static const QByteArray HVON;
    static const QByteArray HVOFF;

    /*Protocol Parameter***********************************************************************/
    //<STX> | <ADDR> | <WIN> | <COM> | <DATA> | <ETX> | <CRC>
    static constexpr const quint8 STX = 0x02;
    quint8 ADDR;
    quint16 WIN;
    quint8 COM;
    QByteArray DATA;
    static constexpr const quint8 ETX = 0x03;
    quint16 CRC;
    bool isCMD = true;
    QByteArray MSG;

    //_________________________________________________________________________________________
};
#endif // WINDOWPROTOCOL_H
