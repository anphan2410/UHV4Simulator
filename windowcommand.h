#ifndef WINDOWCOMMAND_H
#define WINDOWCOMMAND_H

#include <QDebug>
#include <QtMath>
#include <QHash>
#include <math.h>
#include "commoncontrols.h"



///
/// \brief The WindowCommand class
///
class WindowCommand
{
    ///Define Types in the MESSAGE
    /// <STX> + <ADDR> + <WIN> + <COM> + <DATA> + <ETX> + <CRC>
    //STX = 0x02 ETX = 0x03
    typedef QByteArray TypMSG;
    typedef quint8 TypADDR; // Field Length = 1 char
    //unit address = 0x80 + device number(0 to 31)
    typedef QByteArray TypWIN; // Field Length = 3 chars
    //Window number '000' to '999'
    typedef quint8 TypCOM; // Field Length = 1 char
    //read = 0x30 , write = 0x31
    typedef QByteArray TypDATA; // Field Length = 1, 6, 10 chars
    //data to be sent
    typedef QByteArray TypCRC; // Field length = 2 chars
    //checksum


    ///Define Class Constants
    //Fixed Size in Byte of Message Components (need discussed)
    //Size of DATA is not constant
    static const quint8 SzADDR = 1;
    static const quint8 SzWIN = 3;
    static const quint8 SzCOM = 1;
    static const quint8 SzCRC = 2;

    //Fixed Values of Message Components
    static  const quint8 STX = 0x02;
    static  const quint8 ETX = 0x03;
    static  const TypADDR AddressBase = 0x80;
    static  const TypCOM COMRead = 0x30; //default
    static  const TypCOM COMWrite = 0x31;
    static  const quint8 CHANNEL1 = 0x31;
    static  const quint8 CHANNEL2 = 0x32;
    static  const quint8 CHANNEL3 = 0x33;
    static const quint8 CHANNEL4 = 0x34;

    static const TypDATA NumBase;
    static const TypDATA ProtectBase;

    //Fixed Values of DATA Components and Bases correspond with WIN
      /* WIN HV ON/OFF */

    static  const quint8 ChannelON = 0x31;
    static  const quint8 ChannelOFF = 0x30; //default

    static  const TypWIN HVBase;// = {0x30, 0x31}; //append CHANNEL1 - 4 for Channel 1 - 4

     /* WIN Baud rate */
    static  const quint8 BaudRate1200 = 0x31;
    static  const quint8 BaudRate2400 = 0x32;
    static  const quint8 BaudRate4800 = 0x33;
    static  const quint8 BaudRate9600 = 0x34;

     /* WIN Serial Type Select */
    static  const quint8 SerialRS232 = 0x30; //default
    static  const quint8 SerialRS485 = 0x31;

     /* WIN Channel Selection */
    /*static  const TypDATA Channel1;// = {0x30, 0x30, 0x30, 0x30, 0x30, 0x31};
    static  const TypDATA Channel2;// = {0x30, 0x30, 0x30, 0x30, 0x30, 0x32};
    static  const TypDATA Channel3;// = {0x30, 0x30, 0x30, 0x30, 0x30, 0x33};
    static  const TypDATA Channel4;// = {0x30, 0x30, 0x30, 0x30, 0x30, 0x34};*/

     /* WIN Unit Pressure */
    static  const quint8 PressureTorr = 0x30;
    static  const quint8 PressuremBar = 0x31; //default
    static  const quint8 PressurePa = 0x32;

     /* WIN Temperature */
    static  const TypWIN TemperatureBase;// = {0x38, 0x30}; //append FAN, CHANNELFAN1 - 4 for Channel 1 - 4
     //to append because this is an exception for channel 3 and 4

    static  const quint8 FAN = 0x30;
    static  const quint8 CHANNELTEMP1 = 0x31;
    static  const quint8 CHANNELTEMP2 = 0x32;
    static  const quint8 CHANNELTEMP3 = 0x38;
    static  const quint8 CHANNELTEMP4 = 0x39;

     /* WIN Measurement */
    static  const quint8 MeasuredBase = 0x38; //append CHANNEL1 - 4 then append MeasuredV / I / P
    static  const quint8 MeasuredV = 0x30;
    static  const quint8 MeasuredI = 0x31;
    static  const quint8 MeasuredP = 0x32;

    /*Hash Table to track Class Instance*/
    static QHash<const quint8, WindowCommand *> &WCObjSet;

    ///Declare Object Properties
    /// <STX> + <ADDR> + <WIN> + <COM> + <DATA> + <ETX> + <CRC>
    quint8 mWCNo; // Number of the Unique Pump for address
    quint8 mCHANNEL;
    TypMSG &mMSG;
    //Message Components
    TypADDR mADDR;
    TypWIN mWIN;
    TypCOM mCOM;
    TypDATA &mDATA;
    TypDATA &mDATAProtectSwitch;
    TypCRC mCRC;
    bool IsTemperature;
    bool IsHV;
    bool IsProtected; //ProtectSwitch has been called
    bool IsProtect;

public:
    explicit WindowCommand(const quint8 WCNum);
    ~WindowCommand();
    static WindowCommand &WC(const TypMSG &QBArr);
    static WindowCommand &WC(const quint8 WCNum);
    static bool IsWC(const quint8 WCNum);
    static quint8 CountWC();
    static bool DeleteWC(const quint8 WCNum);

    quint8 GetWCNo() const;
    void SetWCNo(const quint8 WCNum);

    //Get Message Components
    const TypMSG GetMSG() const;
    TypADDR GetADDR() const;
    TypWIN GetWIN() const;
    TypCOM GetCOM() const;
    const TypDATA GetDATA() const;
    TypCRC GetCRC() const;

    const TypCRC GenerateCRC(const QByteArray &data);
    const TypMSG GenerateMSG();

    //Get Message Components Meanings
    //QString GetWindow() const;
    //QString GetCommand() const;
    //const QString GetData() const;
    //const QString GetMessage() const;

    ///Functions
    //Complete Functions
    WindowCommand &Raw(QByteArray RawMSG);
    WindowCommand &SetBaudRate(const int BaudRate);
    WindowCommand &ReadModel();
    WindowCommand &SelectSerialType(QByteArray Serial);
    //WindowCommand &ReadInterlock();
    WindowCommand &UnitPressure(QByteArray Pressure);
    WindowCommand &UnitPressure();
    WindowCommand &HVSwitch(const int Channel, QByteArray State);
    WindowCommand &ReadBaudRate();
    WindowCommand &ProtectSwitch(const int Channel, QByteArray State);
    WindowCommand &ProtectRead();

    //Incomplete Functions
    WindowCommand &HVSwitch();
    WindowCommand &On();
    WindowCommand &Off();
    WindowCommand &SelectChannelError(const int Channel); //Win 505, view in Win 206, to be appended
    WindowCommand &ReadT(); //to be appended
    WindowCommand &ReadV(); //to be appended
    WindowCommand &ReadI(); //to be appended
    WindowCommand &ReadP(); //to be appended

     /* To append */
    WindowCommand &SetChannel(const int Channel);
    WindowCommand &SetChannel(QByteArray Channel);
    WindowCommand &Channel1();
    WindowCommand &Channel2();
    WindowCommand &Channel3();
    WindowCommand &Channel4();
    WindowCommand &Fan();

    WindowCommand &Read();
    WindowCommand &Write();

    //Translators
    int ReadTemperature();
    int ReadVoltage();
    double ReadCurrent();
    double ReadPressure();
    int *ReadProtect();

};

#endif // WINDOWCOMMAND_H
