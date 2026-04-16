#include "modbuscommsettings.h"
//#include "QsLog/QsLog.h"

ModbusCommSettings::ModbusCommSettings(const QString &fileName, Format format , QObject *parent)
    : QSettings(fileName, format, parent)
{
    this->loadSettings();
}

void  ModbusCommSettings::loadSettings()
{
    load(this);
}

void  ModbusCommSettings::saveSettings()
{
    save(this);
}

QString  ModbusCommSettings::TCPPort()
{
    return m_TCPPort;
}

void ModbusCommSettings::setTCPPort(QString tcpPort)
{
    m_TCPPort = tcpPort;
}

void ModbusCommSettings::setSlaveIP(QString IP)
{
    m_slaveIP = IP;
}

QString  ModbusCommSettings::slaveIP()
{
    return m_slaveIP;
}

QString  ModbusCommSettings::serialDev()
{
    return m_serialDev;
}

QString  ModbusCommSettings::serialPort()
{
    return m_serialPort;
}

QString  ModbusCommSettings::serialPortName()
{
    QString serialPortName;
    int serialPortNo = m_serialPort.toInt();
#ifdef Q_OS_WIN32
    if (serialPortNo > 9)
        serialPortName = "\\\\.\\COM" + m_serialPort;
    else
        serialPortName = "COM" + m_serialPort;
#else
    serialPortName = m_serialDev;
    serialPortName += QString("%1").arg(m_serialPort.toInt() - 1);
#endif
    return serialPortName;
}

void ModbusCommSettings::setSerialPort(QString serialPort, QString serialDev)
{
    m_serialDev = serialDev;
    m_serialPort = serialPort;
}

QString  ModbusCommSettings::baud()
{
    return m_baud;
}

void ModbusCommSettings::setBaud(QString baud)
{
    m_baud = baud;
}

QString  ModbusCommSettings::dataBits()
{
    return m_dataBits;
}

void ModbusCommSettings::setDataBits(QString dataBits)
{
    m_dataBits = dataBits;
}

QString  ModbusCommSettings::stopBits()
{
    return m_stopBits;
}

void ModbusCommSettings::setStopBits(QString stopBits)
{
    m_stopBits = stopBits;
}

QString  ModbusCommSettings::parity()
{
    return m_parity;
}

void ModbusCommSettings::setParity(QString parity)
{
    m_parity = parity;
}

QString  ModbusCommSettings::RTS()
{
    return m_RTS;
}

void ModbusCommSettings::setRTS(QString RTS)
{
    m_RTS = RTS;
}


QString  ModbusCommSettings::responseTimeOut()
{
    return m_responseTimeOut;
}

void ModbusCommSettings::setResponseTimeOut(QString timeOut)
{
    m_responseTimeOut = timeOut;
}

QString  ModbusCommSettings::byteTimeOut()
{
    return m_byteTimeOut;
}

void ModbusCommSettings::setByteTimeOut(QString timeOut)
{
    m_byteTimeOut = timeOut;
}

int ModbusCommSettings::scanRate()
{
    return m_scanRate;
}

void ModbusCommSettings::setScanRate(int scanRate)
{
    m_scanRate = scanRate;
}

QString ModbusCommSettings::mode()
{
    if (!m_slaveIP.isEmpty())
        return "TCP";
	else if (!m_serialDev.isEmpty())
		return "RTU";
    else
		return "NONE";
}


//void ModbusCommSettings::loadSession(QString fName)
//{
//    //QLOG_INFO()<<  "Load session config from file " << fName;
//    QSettings m_save_session(fName, QSettings::IniFormat, this);
//    load(&m_save_session);
//}

//void ModbusCommSettings::saveSession(QString fName)
//{
//    //QLOG_INFO()<<  "Save session config to file " << fName;
//    QSettings m_save_session(fName, QSettings::IniFormat, this);
//    save(&m_save_session);
//}

void ModbusCommSettings::load(QSettings *s)
{

    if (s->value("TCP/TCPPort").isNull())
        m_TCPPort = "502";
    else
        m_TCPPort = s->value("TCP/TCPPort").toString();

    if (s->value("TCP/SlaveIP").isNull())
        m_slaveIP = "";
    else
        m_slaveIP = s->value("TCP/SlaveIP").toString();

    if (s->value("RTU/SerialDev").isNull())
        #ifdef Q_OS_WIN32
            m_serialDev = "";
        #else
            m_serialDev = "/dev/ttyS";
        #endif
    else
        #ifdef Q_OS_WIN32
            m_serialDev = "COM";
        #else
            m_serialDev = s->value("RTU/SerialDev").toString();
        #endif
    if (s->value("RTU/SerialPort").isNull())
    {
        m_serialPort = "1";
    }
    else {
        m_serialPort = s->value("RTU/SerialPort").toString();
    }

    if (s->value("RTU/Baud").isNull())
        m_baud = "115200";
    else
        m_baud = s->value("RTU/Baud").toString();

    if (s->value("RTU/DataBits").isNull())
        m_dataBits = "8";
    else
        m_dataBits = s->value("RTU/DataBits").toString();

    if (s->value("RTU/StopBits").isNull())
        m_stopBits = "1";
    else
        m_stopBits = s->value("RTU/StopBits").toString();

    if (s->value("RTU/Parity").isNull())
        m_parity = "None";
    else
        m_parity = s->value("RTU/Parity").toString();

    if (s->value("RTU/RTS").isNull())
        #ifdef Q_OS_WIN32
            m_RTS = "Disable";
        #else
            m_RTS = "None";
        #endif
    else
        m_RTS = s->value("RTU/RTS").toString();

    if (s->value("Var/ResponseTimeOut").isNull())
        m_responseTimeOut = "500";
    else
        m_responseTimeOut = s->value("Var/ResponseTimeOut").toString();

    if (s->value("Var/ByteTimeOut").isNull())
        m_byteTimeOut = "500";
    else
        m_byteTimeOut = s->value("Var/ByteTimeOut").toString();

    if (s->value("Session/ScanRate").isNull())
        m_scanRate = 50;
    else
        m_scanRate = s->value("Session/ScanRate").toInt();
}

void ModbusCommSettings::save(QSettings *s)
{
    s->setValue("TCP/TCPPort",m_TCPPort);
    s->setValue("TCP/SlaveIP",m_slaveIP);
    s->setValue("RTU/SerialDev",m_serialDev);
    s->setValue("RTU/SerialPort",m_serialPort);
    s->setValue("RTU/Baud",m_baud);
    s->setValue("RTU/DataBits",m_dataBits);
    s->setValue("RTU/Parity",m_parity);
    s->setValue("RTU/StopBits", m_stopBits);
    s->setValue("RTU/RTS",m_RTS);
    s->setValue("Var/ResponseTimeOut",m_responseTimeOut);
    s->setValue("Var/ByteTimeOut", m_byteTimeOut);
    s->setValue("Session/ScanRate",m_scanRate);
}
