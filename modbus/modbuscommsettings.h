#ifndef MODBUSCOMMSETTINGS_H
#define MODBUSCOMMSETTINGS_H

#include <QSettings>

class ModbusCommSettings : public QSettings
{
    Q_OBJECT;
public:
    explicit ModbusCommSettings(QObject *parent = 0)  : QSettings(parent) { }
    ModbusCommSettings(const QString &fileName, Format format = QSettings::IniFormat, QObject *parent = 0);
    //TCP
    QString TCPPort();
    void setTCPPort(QString tcpPort);
    void setSlaveIP(QString IP);
    QString slaveIP();
    //Serial
    QString serialDev();
    QString serialPort();
    QString serialPortName();
    void setSerialPort(QString serialPort, QString serialDev = "");
    QString baud();
    void setBaud(QString baud);
    QString dataBits();
    void setDataBits(QString dataBits);
    QString stopBits();
    void setStopBits(QString stopBits);
    QString parity();
    void setParity(QString parity);
    QString RTS();
    void setRTS(QString RTS);
    //Var
    QString  maxNoOfLines();
    void setMaxNoOfLines(QString maxNoOfLines);
    QString  responseTimeOut();
    void setResponseTimeOut(QString timeOut);
    QString  byteTimeOut();
    void setByteTimeOut(QString timeOut);
    void loadSettings();
    void saveSettings();
    
    //session
    int scanRate();
    void setScanRate(int scanRate);
    void loadSession(QString fName);
    void saveSession(QString fName);

private:
    //TCP
    QString m_TCPPort;
    QString m_slaveIP;
    //Serial
    QString m_serialDev;
    QString m_serialPort;
    QString m_baud;
    QString m_dataBits;
    QString m_stopBits;
    QString m_parity;
    QString m_RTS;

    QString m_responseTimeOut;
    QString m_byteTimeOut;
    void load(QSettings *s);
    void save(QSettings *s);
    //Log
    //int m_loggingLevel;
    //Session vars
    //int m_modbusMode;
    //int m_slaveID;
    int m_scanRate = 50;
    //int m_functionCode;
    //int m_startAddr;
    //int m_noOfRegs;
    //int m_base;

signals:

public slots:

};

#endif // MODBUSCOMMSETTINGS_H
