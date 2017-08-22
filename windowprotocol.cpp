#include "windowprotocol.h"

WindowProtocol::WindowProtocol()
{
    anIf(WindowProtocolDbgEn, anTrk("Object Constructed"));
}

WindowProtocol::WindowProtocol(const quint8 WPNo)
{
    anIf(WindowProtocolDbgEn, anTrk("Object Constructed With WPNo="<<WPNo));
    setWPNo(WPNo);
}

quint8 WindowProtocol::getWPNo() const
{
    return ADDR-OffsetADDR;
}

const QString WindowProtocol::getAddress() const
{
    return QString::number(ADDR-OffsetADDR);
}

const QString WindowProtocol::getWINMean() const
{
    if (WIN)
        return WINCode2WINMean.value(WIN);
    else
        return QStringLiteral("");
}

const QString WindowProtocol::getCOMMean() const
{
    if (COM==RD)
        return QStringLiteral("RD");
    else if (COM==WR)
        return QStringLiteral("WR");
    else
        return QStringLiteral("");
}

const QString WindowProtocol::getDATAMean() const
{
    QString tmpReturn;
    switch (WIN) {
    case 0:
    {
        QString tmpDataStr(DATA);
        if (tmpDataStr==QStringLiteral("\x06"))
            tmpReturn="ACK";
        else if (tmpDataStr==QStringLiteral("\x15"))
            tmpReturn="NACK";
        else if (tmpDataStr==QStringLiteral("\x32"))
            tmpReturn="Unknown Window";
        else if (tmpDataStr==QStringLiteral("\x33"))
            tmpReturn="Data Type Error";
        else if (tmpDataStr==QStringLiteral("\x35"))
            tmpReturn="Win Disabled";
        break;
    }
    case 11:
    case 12:
    case 13:
    case 14:
    {
        if (DATA==HVON)
            tmpReturn="ON";
        else if (DATA==HVOFF)
            tmpReturn="OFF";
        break;
    }
    case 810:
    case 820:
    case 830:
    case 840:
    {
        tmpReturn=QString::number(DATA.toInt());
        break;
    }
    case 811:
    case 812:
    case 821:
    case 822:
    case 831:
    case 832:
    case 841:
    case 842:
    {
        tmpReturn=QString(DATA).trimmed();
        break;
    }
    default:
        break;
    }
    return tmpReturn;
}

const QString WindowProtocol::getMSGMean() const
{
    QString tmpReturn = QStringLiteral("#");
    QString tmpWIN=getWINMean();
    QString tmpCOM=getCOMMean();
    QString tmpDATA=getDATAMean();
    tmpReturn+=getAddress();
    if (!tmpWIN.isEmpty())
        tmpReturn+=QStringLiteral(" ")+tmpWIN;
    if (!tmpCOM.isEmpty())
        tmpReturn+=QStringLiteral(" ")+tmpCOM;
    if (!tmpDATA.isEmpty())
        tmpReturn+=QStringLiteral(" ")+tmpDATA;
    return tmpReturn;
}

bool WindowProtocol::isCMDFlagSet() const
{
    return isCMD;
}

WindowProtocol &WindowProtocol::setWPNo(const quint8 num)
{
    ADDR=num+OffsetADDR;
    return *this;
}

WindowProtocol &WindowProtocol::setADDR(const quint8 addr)
{
    ADDR=addr;
    return *this;
}

WindowProtocol &WindowProtocol::setWIN(const quint16 win)
{
    if (win)
        WIN=win;
    else
    {
        WIN=0;
        COM=0;
    }
    return *this;
}

WindowProtocol &WindowProtocol::HVOnOffCh1(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::HVOnOffCh2(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::HVOnOffCh3(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::HVOnOffCh4(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::BaudRate(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::Status(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::ErrorCode(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::ControllerModel(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::ControllerSerialNumber(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::RS485SerialAddress(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::SerialTypeSelect(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::ChannelSelection(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::UnitPressure(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::Protect(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::FixedStep(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::DeviceNumberCh1(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::PowerMaxCh1(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::VTargetCh1(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::IProtectCh1(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::SetPointCh1(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::DeviceNumberCh2(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::PowerMaxCh2(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::VTargetCh2(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::IProtectCh2(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::SetPointCh2(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::DeviceNumberCh3(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::PowerMaxCh3(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::VTargetCh3(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::IProtectCh3(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::SetPointCh3(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::DeviceNumberCh4(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::PowerMaxCh4(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::VTargetCh4(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::IProtectCh4(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::SetPointCh4(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::TempFAN(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::TempHV1(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::TempHV2(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::InterlockStatus(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::StatusSetPoint(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::TempHV3(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::TempHV4(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::VMeasuredCh1(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::IMeasuredCh1(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::PMeasuredCh1(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::VMeasuredCh2(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::IMeasuredCh2(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::PMeasuredCh2(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::VMeasuredCh3(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::IMeasuredCh3(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::PMeasuredCh3(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::VMeasuredCh4(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::IMeasuredCh4(){return this->setWIN(WINMean2WINCode.value(__func__));}
WindowProtocol &WindowProtocol::PMeasuredCh4(){return this->setWIN(WINMean2WINCode.value(__func__));}

WindowProtocol &WindowProtocol::setCOM(const quint8 com)
{
    if (com)
        COM=com;
    else
    {
        COM=0;
        WIN=0;
    }
    return *this;
}

WindowProtocol &WindowProtocol::Read()
{
    COM=RD;
    return *this;
}

WindowProtocol &WindowProtocol::Write()
{
    COM=WR;
    return *this;
}

WindowProtocol &WindowProtocol::setDATA(const quint8 aByte)
{
    DATA.clear();
    DATA<<aByte;
    return *this;
}

WindowProtocol &WindowProtocol::setDATA(const QByteArray &data)
{
    DATA.clear();
    DATA<<data;
    return *this;
}

WindowProtocol &WindowProtocol::setON()
{
    if ((WIN>=11)&&(WIN<=14))
    {
        this->setDATA(HVON);
    }
    return *this;
}

WindowProtocol &WindowProtocol::setOFF()
{
    if ((WIN>=11)&&(WIN<=14))
    {
        this->setDATA(HVOFF);
    }
    return *this;
}

WindowProtocol &WindowProtocol::clearDATA()
{
    DATA.clear();
    return *this;
}

WindowProtocol &WindowProtocol::setCRC(const quint16 crc)
{
    CRC=crc;
    return *this;
}

const QByteArray &WindowProtocol::genMSG()
{
    MSG.clear();
    MSG<<ADDR;
    if (WIN)
        MSG<<IntStr2QBArr0Pad(WIN,szWIN)<<COM;
    MSG<<DATA<<ETX;
    CRC=QString(QByteArray().append(XORofAllBytesInQByteArr(MSG)).toHex()).toUpper().toLocal8Bit().toHex().toInt(NULL,16);
    MSG<<CRC;
    MSG.prepend(STX);
    anIf(WindowProtocolDbgEn,
         anAck("Protocol Generated !");
         anInfo(MSG.toHex());
         anInfo(getMSGMean());
    );
    return MSG;
}

WindowProtocol &WindowProtocol::fromQByteArray(const QByteArray &aMsg)
{
    WindowProtocol * tmpReturn = new WindowProtocol();
    quint8 upPos=szSTX;
    quint8 downPos=aMsg.size();
    tmpReturn->setADDR(aMsg.mid(upPos,szADDR).toHex().toInt(NULL,16));
    upPos+=szADDR;
    tmpReturn->setCRC(aMsg.right(szCRC).toHex().toInt(NULL,16));
    downPos-=(szETX+szCRC);
    quint8 tmpInt = downPos - upPos;
    quint16 tmpWINCode=aMsg.mid(upPos,szWIN).toInt();
    if ((tmpInt>=4) && (WINCode2WINMean.contains(tmpWINCode)))
    {
        tmpReturn->setWIN(tmpWINCode);
        upPos+=szWIN;
        tmpReturn->setCOM(aMsg.mid(upPos,szCOM).toHex().toInt(NULL,16));
        upPos+=szCOM;
    }
    else
    {
        tmpReturn->setWIN(0).setCOM(0);
    }
    tmpInt = downPos - upPos;
    tmpReturn->setDATA(aMsg.mid(upPos,tmpInt)).genMSG();
    return *tmpReturn;
}

WindowProtocol &WindowProtocol::setCMDFlag(const bool isACMD)
{
    isCMD=isACMD;
    return *this;
}

quint8 WindowProtocol::getADDR() const
{
    return ADDR;
}

quint16 WindowProtocol::getWINCode() const
{
    return WIN;
}

quint8 WindowProtocol::getCOM() const
{
    return COM;
}

const QByteArray &WindowProtocol::getDATA() const
{
    return DATA;
}

quint16 WindowProtocol::getCRC() const
{
    return CRC;
}

const QByteArray &WindowProtocol::getMSG() const
{
    return MSG;
}

const QHash<QString , quint16> &WindowProtocol::WINMean2WINCode = * new QHash<QString, quint16>
({
     {"HVOnOffCh1", 11},
     {"HVOnOffCh2", 12},
     {"HVOnOffCh3", 13},
     {"HVOnOffCh4", 14},
     {"BaudRate", 108},
     {"Status", 205},
     {"ErrorCode", 206},
     {"ControllerModel", 319},
     {"ControllerSerialNumber", 323},
     {"RS485SerialAddress", 503},
     {"SerialTypeSelect", 504},
     {"ChannelSelection", 505},
     {"UnitPressure", 600},
     {"Protect", 602},
     {"FixedStep", 603},
     {"DeviceNumberCh1", 610},
     {"PowerMaxCh1", 612},
     {"VTargetCh1", 613},
     {"IProtectCh1", 614},
     {"SetPointCh1", 615},
     {"DeviceNumberCh2", 620},
     {"PowerMaxCh2", 622},
     {"VTargetCh2", 623},
     {"IProtectCh2", 624},
     {"SetPointCh2", 625},
     {"DeviceNumberCh3", 630},
     {"PowerMaxCh3", 632},
     {"VTargetCh3", 633},
     {"IProtectCh3", 634},
     {"SetPointCh3", 635},
     {"DeviceNumberCh4", 640},
     {"PowerMaxCh4", 642},
     {"VTargetCh4", 643},
     {"IProtectCh4", 644},
     {"SetPointCh4", 645},
     {"TempFAN", 800},
     {"TempHV1", 801},
     {"TempHV2", 802},
     {"InterlockStatus", 803},
     {"StatusSetPoint", 804},
     {"TempHV3", 808},
     {"TempHV4", 809},
     {"VMeasuredCh1", 810},
     {"IMeasuredCh1", 811},
     {"PMeasuredCh1", 812},
     {"VMeasuredCh2", 820},
     {"IMeasuredCh2", 821},
     {"PMeasuredCh2", 822},
     {"VMeasuredCh3", 830},
     {"IMeasuredCh3", 831},
     {"PMeasuredCh3", 832},
     {"VMeasuredCh4", 840},
     {"IMeasuredCh4", 841},
     {"PMeasuredCh4", 842}
});

const QHash<quint16, QString > &WindowProtocol::WINCode2WINMean = SwapKeyValOnOneToOneQHash(WindowProtocol::WINMean2WINCode);

const QByteArray WindowProtocol::HVON = QByteArray().append(0x31);
const QByteArray WindowProtocol::HVOFF = QByteArray().append(0x30);
