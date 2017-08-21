#include "windowcommand.h"

//Define the bases
QByteArray tmp = "00000";
const WindowCommand::TypDATA WindowCommand::NumBase = tmp;

QByteArray tmp4 = "01";
const WindowCommand::TypWIN WindowCommand::HVBase = tmp4;

QByteArray tmp10 = "80";
const WindowCommand::TypWIN WindowCommand::TemperatureBase = tmp10;

QByteArray tmp11 = "0000000000";
const WindowCommand::TypDATA WindowCommand::ProtectBase = tmp11;



WindowCommand::WindowCommand(const quint8 WCNum)
    :mWCNo(WCNum)
    ,mADDR(TypADDR(AddressBase + WCNum))
    ,mWIN("000")
    ,mCOM(COMRead)
    ,mDATA(* new TypDATA())
    ,mDATAProtectSwitch(* new TypDATA())
    ,mCRC("00")
    ,mMSG(*new TypMSG())
    ,IsTemperature(false)
    ,IsHV(false)
    ,mCHANNEL(5) //no Channel is set
    ,IsProtected(false)
    ,IsProtect(false)
{
    if (WindowCommand::WCObjSet.contains(WCNum))   //If There's An Object With That BPNum, Then Throw Exception
        throw std::invalid_argument(std::string("Pump #")+std::to_string(WCNum)+std::string(" Has Already Existed !!!"));
    else
        WindowCommand::WCObjSet.insert(WCNum,this);
}

WindowCommand::~WindowCommand()
{
    WindowCommand::WCObjSet.remove(mWCNo);
}

WindowCommand &WindowCommand::WC(const quint8 WCNum)
{
    if (WindowCommand::WCObjSet.contains(WCNum))
        return *WindowCommand::WCObjSet.value(WCNum);
    else
        throw std::invalid_argument(std::string("Pump #")+std::to_string(WCNum)+std::string(" Does Not Exist !!!"));
}

WindowCommand &WindowCommand::WC(const TypMSG &QBArr)
{
    // <STX> + <ADDR> + <WIN> + <COM> + <DATA> + <ETX> + <CRC>
    WindowCommand::TypADDR WCAdress = *QBArr.mid(sizeof(STX),SzADDR);

    quint8 tmp1 = sizeof(STX);
    WindowCommand &WCTmp = WindowCommand::WC(WCAdress - AddressBase);
    WCTmp.mADDR = WCAdress;
    tmp1 += SzADDR;
    WCTmp.mWIN = QBArr.mid(tmp1,SzWIN);
    tmp1 += SzWIN;
    WCTmp.mCOM = *QBArr.mid(tmp1,SzCOM);
    tmp1 += SzCOM;
    quint8 tmp2 = sizeof(QBArr);
    WCTmp.mCRC = QBArr.right(tmp2 - SzCRC);
    tmp2 -= tmp1 + SzCRC + sizeof(ETX); //data length
    WCTmp.mDATA = QBArr.mid(tmp1,tmp2);
    qDebug() << "<<STX: " << STX << " ADDR: " << WCTmp.mADDR << " WIN: " << WCTmp.mWIN.toHex() << " COM: " << WCTmp.mCOM << " DATA: " << WCTmp.mDATA.toHex() << " ETX: " << ETX << " CRC: " << WCTmp.mCRC << endl;
    WCTmp.mMSG.clear();
    WCTmp.mMSG << STX << WCTmp.mADDR << WCTmp.mWIN << WCTmp.mCOM << WCTmp.mDATA << ETX <<WCTmp.mCRC;
    return WCTmp;
}

bool WindowCommand::IsWC(const quint8 WCNum)
{
    return WindowCommand::WCObjSet.contains(WCNum);
}

quint8 WindowCommand::CountWC()
{
    return WindowCommand::WCObjSet.size();
}

bool WindowCommand::DeleteWC(const quint8 WCNum)
{
    if(WindowCommand::WCObjSet.contains(WCNum))
    {
        WindowCommand::WCObjSet.value(WCNum)->~WindowCommand();
        return true;
    }
    else
        return false;
}

quint8 WindowCommand::GetWCNo() const
{
    return mWCNo;
}

void WindowCommand::SetWCNo(const quint8 WCNum)
{
    if(WindowCommand::WCObjSet.contains(WCNum))
        throw std::invalid_argument(std::string("Pump #")+std::to_string(WCNum)+std::string("Has Already Existed !!!"));
    else
    {
        WindowCommand::WCObjSet.remove(mWCNo);
        mWCNo = WCNum;
        mADDR = AddressBase + WCNum;
        mWIN = "000";
        mCOM = COMRead;
        mDATA.clear();
        mCRC = "00";
        mMSG.clear();
        IsTemperature = false;
        IsHV = false;
        mCHANNEL = 5; //no Channel is set

    }
}

const WindowCommand::TypMSG WindowCommand::GetMSG() const
{
    return mMSG;
}

WindowCommand::TypADDR WindowCommand::GetADDR() const
{
    return mADDR;
}

WindowCommand::TypWIN WindowCommand::GetWIN() const
{
    return mWIN;
}

WindowCommand::TypCOM WindowCommand::GetCOM() const
{
    return mCOM;
}

const WindowCommand::TypDATA WindowCommand::GetDATA() const
{
    return mDATA;
}

WindowCommand::TypCRC WindowCommand::GetCRC() const
{
    return mCRC;
}

const WindowCommand::TypCRC WindowCommand::GenerateCRC(const QByteArray &data)
{
    // <STX> + <ADDR> + <WIN> + <COM> + <DATA> + <ETX>
    quint8 checksum = 0;
    QByteArray bytes[2];
    int len = data.size();
    //XOR all the bits AFTER STX
    for(int i = sizeof(STX); i < len; i++)
    {
        checksum = checksum ^ data[i];
    }
    //a check sum of a 2-digits hex
    //split it into two ASCII characters and convert to hex again
    WindowCommand::TypCRC CRC;
    CRC.append(QString::number((checksum /16),16).toUpper());
    CRC.append(QString::number((checksum %16),16).toUpper());


    return CRC;
}

const WindowCommand::TypMSG WindowCommand::GenerateMSG()
{
    // <STX> + <ADDR> + <WIN> + <COM> + <DATA> + <ETX>


    mMSG.clear();
    mMSG << STX << mADDR << mWIN << mCOM << mDATA << ETX;
    mCRC.clear();
    mCRC = WindowCommand::GenerateCRC(mMSG);

    //connect the checksum at the end of the data string
    mMSG << mCRC;
    qDebug() << ">>STX: " << STX << " ADDR: " << mADDR << " WIN: " << mWIN.toHex() << " COM: " << mCOM << " DATA: " << mDATA.toHex() << " ETX: " << ETX << " CRC: " << mCRC << endl;
    //reset the flags and values
    IsTemperature = false;
    IsHV = false;
    IsProtect = false;
    mWIN.clear();
    mWIN = "000";
    mDATA.clear();

    return mMSG;
}

/*QString WindowCommand::GetWindow() const
{

}

QString WindowCommand::GetCommand() const
{

}

const QString WindowCommand::GetData() const
{

}

const QString WindowCommand::GetMessage() const
{

}*/

//Complete Functions
WindowCommand &WindowCommand::Raw(QByteArray RawMSG)
{
    QByteArray data;
    data = data.fromHex(RawMSG);
    //<ADDR> + <WIN> + <COM> + <DATA>
    WindowCommand::TypADDR WCAddress = *RawMSG.left(SzADDR);

    quint8 tmp1 = 0;
    WindowCommand &WCTmp = WindowCommand::WC(WCAddress - AddressBase);
    WCTmp.mADDR = WCAddress;
    tmp1 += SzADDR;
    WCTmp.mWIN = RawMSG.mid(tmp1,SzWIN);
    tmp1 += SzWIN;
    WCTmp.mCOM = *RawMSG.mid(tmp1,SzCOM);
    tmp1 += SzCOM;
    WCTmp.mDATA = RawMSG.right(tmp1 + 1);
    //QByteArray data;
    return WCTmp;
}

WindowCommand &WindowCommand::SetBaudRate(const int BaudRate)
{
    mWIN = "108";
    mCOM = COMWrite;
    mDATA = NumBase;

    switch (BaudRate){
    case 1200:
    {
        mDATA << BaudRate1200;
        break;
    }
    case 2400:
    {
        mDATA << BaudRate2400;
        break;
    }
    case 4800:
    {
        mDATA << BaudRate4800;
        break;
    }
    case 9600:
    {
        mDATA << BaudRate9600;
        break;
    }
    }

    return *this;
}

WindowCommand &WindowCommand::ReadModel()
{
    mWIN = "319";
    mCOM = COMRead;

    return *this;
}

WindowCommand &WindowCommand::SelectSerialType(QByteArray Serial)
{
    mWIN = "504";
    mCOM = COMWrite;

    Serial.toUpper();
    if( Serial == "RS232")
    {
        mDATA << SerialRS232;
    }
    else if ( Serial == "RS485")
    {
        mDATA << SerialRS485;
    }


    return *this;
}

/*WindowCommand &WindowCommand::ReadInterlock()
{

}*/

WindowCommand &WindowCommand::UnitPressure(QByteArray Pressure)
{
    mWIN = "600";
    mCOM = COMWrite;
    mDATA = NumBase;

    Pressure = Pressure.toUpper();
    if( Pressure == "TORR")
    {
        mDATA << PressureTorr;
    }
    else if( Pressure == "MBAR")
    {
        mDATA << PressuremBar;
    }
    else if ( Pressure == "PA")
    {
        mDATA << PressurePa;
    }

    return *this;
}

WindowCommand &WindowCommand::UnitPressure()
{
    mWIN = "600";
    mCOM = COMRead;

    return *this;
}


WindowCommand &WindowCommand::ReadBaudRate()
{
    mWIN = "108";
    mCOM = COMRead;

    return *this;
}

WindowCommand &WindowCommand::ProtectSwitch(const int Channel, QByteArray State)
{
    mWIN.clear();
    mWIN = "602";
    mCOM = COMWrite;
    IsProtect = true;

    if(!IsProtected)
    {
        IsProtected = true;
        //initialized the DATA
        mDATAProtectSwitch = ProtectBase;
    }

    quint8 tmpState;
    State = State.toUpper();
    if (State == "ON")
    {
        tmpState = ChannelON;
    }
    else if (State == "OFF")
    {
        tmpState = ChannelOFF;
    }

    switch (Channel){
    case 1:
        mDATAProtectSwitch[9] = tmpState;
        break;
    case 2:
        mDATAProtectSwitch[8] = tmpState;
        break;
    case 3:
        mDATAProtectSwitch[7] = tmpState;
        break;
    case 4:
        mDATAProtectSwitch[6] = tmpState;
        break;
    }

    mDATA.clear();
    mDATA << mDATAProtectSwitch;

    return *this;
}

WindowCommand &WindowCommand::ProtectRead()
{
    mWIN = "602";
    mCOM = COMRead;
    mDATA.clear();

    return *this;
}


WindowCommand &WindowCommand::HVSwitch(const int Channel, QByteArray State)
{
    IsHV = true; //set the flag
    mWIN.clear();
    switch (Channel){
    case 1:
        mCHANNEL = CHANNEL1;
        break;
    case 2:
        mCHANNEL = CHANNEL2;
        break;
    case 3:
        mCHANNEL = CHANNEL3;
        break;
    case 4:
        mCHANNEL = CHANNEL4;
        break;
    }

    State = State.toUpper();
    if(State == "ON")
    {
        mDATA[0] = ChannelON;
        mCOM = COMWrite;
    }
    else if(State == "OFF")
    {
        mDATA[0] = ChannelOFF;
        mCOM = COMWrite;
    }
    else if(State =="READ")
    {
        mCOM = COMRead;
    }

    mWIN << HVBase << mCHANNEL;

    return *this;
}

WindowCommand &WindowCommand::HVSwitch()
{
    IsHV = true;
    mWIN = HVBase;
    if (mCHANNEL != 5) //already set Channel
        mWIN[2] = mCHANNEL;

    return *this;
}

WindowCommand &WindowCommand::On()
{
    mDATA.clear();
    mDATA[0] = ChannelON;
    mCOM = COMWrite;
    return *this;
}
WindowCommand &WindowCommand::Off()
{
    mDATA.clear();
    mDATA[0] = ChannelOFF;
    mCOM = COMWrite;
    return *this;
}

WindowCommand &WindowCommand::SelectChannelError(const int Channel) //Win 505, view in Win 206, to be appended
{
    mWIN.clear();
    mWIN = "505";
    mCOM = COMWrite;
    mDATA = NumBase;

    switch(Channel){
    case 1:
        mDATA << CHANNEL1;
        break;
    case 2:
        mDATA << CHANNEL2;
        break;
    case 3:
        mDATA << CHANNEL3;
        break;
    case 4:
        mDATA << CHANNEL4;
        break;
    }


    return *this;
}

WindowCommand &WindowCommand::ReadT() //to be appended
{
    IsTemperature = true; //set the flag
    //in case the channel is set before calling and fall into non-Temperature case
    //set the mCHANNEL to correct value
    mWIN = TemperatureBase;
    switch(mCHANNEL){
    case 0x30: //Fan case
        mWIN = TemperatureBase;
        mWIN[2] = mCHANNEL;
    case 0x31:
        mWIN = TemperatureBase;
        mCHANNEL = CHANNELTEMP1;
        mWIN[2] = mCHANNEL;
        break;
    case 0x32:
        mWIN = TemperatureBase;
        mCHANNEL = CHANNELTEMP2;
        mWIN[2] = mCHANNEL;
        break;
    case 0x33:
        mWIN = TemperatureBase;
        mCHANNEL = CHANNELTEMP3;
        mWIN[2] = mCHANNEL;
        break;
    case 0x34:
        mWIN = TemperatureBase;
        mCHANNEL = CHANNELTEMP4;
        mWIN[2] = mCHANNEL;
        break;
    case 5: //havent set Channel
        break;
    }
    return *this;
}

WindowCommand &WindowCommand::ReadV() //to be appended
{
    mWIN[0] = MeasuredBase;
    mWIN[1] = mCHANNEL; // = 5 if not set channel
    mWIN[2] = MeasuredV;
    mCOM = COMRead;
    mDATA.clear();

    return *this;
}

WindowCommand &WindowCommand::ReadI() //to be appended
{
    mWIN[0] = MeasuredBase;
    mWIN[1] = mCHANNEL; // = 5 if not set channel
    mWIN[2] = MeasuredI;
    mCOM = COMRead;
    mDATA.clear();

    return *this;
}

WindowCommand &WindowCommand::ReadP() //to be appended
{
    mWIN[0] = MeasuredBase;
    mWIN[1] = mCHANNEL; // = 5 if not set channel
    mWIN[2] = MeasuredP;
    mCOM = COMRead;
    mDATA.clear();

    return *this;
}

WindowCommand &WindowCommand::SetChannel(const int Channel)
{
    if(IsTemperature == true) //case Temperature
    {
        switch (Channel){
        case 1:
            mCHANNEL = CHANNELTEMP1;
            break;
        case 2:
            mCHANNEL = CHANNELTEMP2;
            break;
        case 3:
            mCHANNEL = CHANNELTEMP3;
            break;
        case 4:
            mCHANNEL = CHANNELTEMP4;
            break;
        }
    }
    else //case SWITCH and the rest
    {
        switch (Channel){
        case 1:
            mCHANNEL = CHANNEL1;
            break;
        case 2:
            mCHANNEL = CHANNEL2;
            break;
        case 3:
            mCHANNEL = CHANNEL3;
            break;
        case 4:
            mCHANNEL = CHANNEL4;
            break;
        }
    }
    if(IsHV == true || IsTemperature == true) // WIN X-X-Channel
    {
        mWIN[2] = mCHANNEL;
    }
    else //the rests WIN X-Channel-X
        mWIN[1] = mCHANNEL;

    return *this;
}

WindowCommand &WindowCommand::SetChannel(QByteArray Channel) //accepts Fan
{
    Channel = Channel.toUpper();
    if( Channel == "FAN")
    {
        mCHANNEL = 0x30;
        mWIN[2] = mCHANNEL;
        mCOM = COMRead;
    }


    return *this;
}

WindowCommand &WindowCommand::Channel1()
{
    if(IsTemperature == true)
        mCHANNEL = CHANNELTEMP1;
    else
        mCHANNEL = CHANNEL1;
    if(IsHV == true || IsTemperature == true) // WIN X-X-Channel, already called the function
    {
        mWIN[2] = mCHANNEL;
    }
    else //the rests WIN X-Channel-X,
        mWIN[1] = mCHANNEL;

    return *this;
}

WindowCommand &WindowCommand::Channel2()
{
    if(IsTemperature == true)
        mCHANNEL = CHANNELTEMP2;
    else
        mCHANNEL = CHANNEL2;
    if(IsHV == true || IsTemperature == true) // WIN X-X-Channel, already called the function
    {
        mWIN[2] = mCHANNEL;
    }
    else //the rests WIN X-Channel-X,
        mWIN[1] = mCHANNEL;

    return *this;
}

WindowCommand &WindowCommand::Channel3()
{
    if(IsTemperature == true)
        mCHANNEL = CHANNELTEMP3;
    else
        mCHANNEL = CHANNEL3;
    if(IsHV == true || IsTemperature == true) // WIN X-X-Channel, already called the function
    {
        mWIN[2] = mCHANNEL;
    }
    else //the rests WIN X-Channel-X,
        mWIN[1] = mCHANNEL;

    return *this;
}

WindowCommand &WindowCommand::Channel4()
{
    if(IsTemperature == true)
        mCHANNEL = CHANNELTEMP4;
    else
        mCHANNEL = CHANNEL4;
    if(IsHV == true || IsTemperature == true) // WIN X-X-Channel, already called the function
    {
        mWIN[2] = mCHANNEL;
    }
    else //the rests WIN X-Channel-X,
        mWIN[1] = mCHANNEL;

    return *this;
}

WindowCommand &WindowCommand::Fan()
{
    if(IsTemperature == true) //already called the function
    {
        mCHANNEL = FAN;
        mWIN[2] = mCHANNEL;

    }
    else
        throw std::invalid_argument(std::string("Cannot read from Fan!!"));
    return *this;
}

WindowCommand &WindowCommand::Read()
{
    mCOM = COMRead;
    return *this;
}

WindowCommand &WindowCommand::Write()
{
    mCOM = COMWrite;
    return *this;
}

int WindowCommand::ReadTemperature()
{
   int b, c ,d;
   b = mDATA[7] - '0';
   c = mDATA[8] - '0';
   d = mDATA[9] - '0';
   int result = b*100 + c*10 + d;

   return result;
}

int WindowCommand::ReadVoltage()
{
    int b, c, d, e;
    b = mDATA[6] - '0';
    c = mDATA[7] - '0';
    d = mDATA[8] - '0';
    e = mDATA[9] - '0';
    int result = b*1000 + c*100 + d*10 + e;

    return result;
}

double WindowCommand::ReadCurrent()
{
    //bE-cd
    int b;
    int c;
    int d;
    double result;
    b = mDATA[5] - '0';
    c = mDATA[8] - '0';
    d = mDATA[9] - '0';

    result = b*pow(10,-(c*10 + d));

    return result;
}

double WindowCommand::ReadPressure()
{
    //b.cE-de
    int b,c,d,e;
    double result;
    b = mDATA[3] - '0';
    c = mDATA[5] - '0';
    d = mDATA[8] - '0';
    e = mDATA[9] - '0';

    result = (b + c*0.1)*pow(10,-(d*10 + e));

    return result;
}

int *WindowCommand::ReadProtect()
{
    static int protect[4];
    for (int i= 0; i < 4; i++)
    {
        int j = 0;
        if (mDATAProtectSwitch[9-i] == '1')
        {
            protect[j] = i+1;
            j++;
        }
    }

    return protect;
}

QHash<const quint8, WindowCommand *> &WindowCommand::WCObjSet = * new QHash<const quint8, WindowCommand *>();


