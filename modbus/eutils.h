#ifndef EUTILS_H
#define EUTILS_H

#include <QString>
#include <QMap>
#include <QTime>

//static const QString ModbusFunctionNames[]={"Read Coils (0x01)","Read Discrete Inputs (0x02)","Read Holding Registers (0x03)",
//                               "Read Input Registers (0x04)","Write Single Coil (0x05)","Write Single Register (0x06)",
//                               "Write Multiple Coils (0x0f)","Write Multiple Registers (0x10)","Report Server ID (0x11)"};
//static const int ModbusFunctionCodes[]={0x1,0x2,0x3,0x4,0x5,0x6,0xf,0x10,0x11};
static const QString ModbusModeStamp[]={"RTU","TCP",""};

class EUtils
{
private:
    EUtils();

public:

    static std::string ModbusDataTypeName(int fCode);

    static bool ModbusIsWriteFunction(int fCode);
    static bool ModbusIsWriteCoilsFunction(int fCode);
    static bool ModbusIsWriteRegistersFunction(int fCode);

    //static QString ModbusFunctionName(int index)
    //{
    //     return ModbusFunctionNames[index];
    //}

    //static int ModbusFunctionCode(int index)
    //{
    //        return ModbusFunctionCodes[index];
    //}

    static QString TxTimeStamp(int md)
    {
        return ModbusModeStamp[md] + "(Tx):" + (QTime::currentTime().toString("HH:mm:ss:zzz")) + ", ";
    }

    static QString RxTimeStamp(int md)
    {
        return ModbusModeStamp[md] + "(Rx):" + (QTime::currentTime().toString("HH:mm:ss:zzz")) + ", ";
    }

    static QString SysTimeStamp()
    {
        return ((QTime::currentTime().toString("HH:mm:ss:zzz")) + ":SYS:");
    }

    static QChar parity(QString p)
    {
        //the first char is what we need
        return p.at(0);
    }

    static enum {RTU = 0, TCP = 1, None = 0} ModbusMode;

    static enum {Bin = 2, UInt = 10, Hex = 16} NumberFormat;

    //static enum {ReadCoils = 0x1, ReadDisInputs = 0x2,
    //            ReadHoldRegs = 0x3, ReadInputRegs = 0x4,
    //            WriteSingleCoil = 0x5, WriteSingleReg = 0x6,
    //            WriteMultiCoils = 0xf, WriteMultiRegs = 0x10} FunctionCodes;

    static QString formatValue(int value,int frmt, bool is16Bit, bool isSigned);

    static QString libmodbus_strerror(int errnum);

   // template<typename ... Args>
   // static std::string string_format(const std::string& format, Args ... args);

    static std::string QString2StdString(QString str) { return str.toUtf8().constData(); }

    static int doUntilOk(std::function<int()> function, int& retryCnt);
    static void sleep(int milliseconds);
};

#endif // EUTILS_H
