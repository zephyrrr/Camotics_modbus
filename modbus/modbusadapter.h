#ifndef MODBUSADAPTER_H
#define MODBUSADAPTER_H

#pragma warning(disable : 4100)

#define DEFAULT_MODBUS_SLAVE 1
#define DEFAULT_MODBUS_READ_FUNCTION MODBUS_FC_READ_HOLDING_REGISTERS
#define DEFAULT_MODBUS_WRITE_FUNCTION MODBUS_FC_WRITE_MULTIPLE_REGISTERS
#define DEFAULT_MODBUS_FILE_ADDRESS 0

#define MODBUS_FC_WAIT 0x9990

#include <thread>
#include <queue>
#include <mutex>
#include <tuple>
#include <QObject>
#include "registersmodel.h"
#include "rawdatamodel.h"
#include <QTimer>
#include "eutils.h"
#include "utils/TaskThread.h"
#include "libmodbus/modbus.h"

#define TASK_TIMER_PRIORITY 2
#define TASK_DELAY_PRIORITY 1

class ModbusAdapter;

struct ModbusTask
{
    ModbusTask(int slave, int functionCode, int startAddr, int numOfRegs)
		: slave(slave), functionCode(functionCode), startAddr(startAddr), numOfRegs(numOfRegs), postDoFunction(NULL), preDoFunction(NULL)
    {
    }
    ModbusTask(int slave, int functionCode, int startAddr, int numOfRegs, std::string writeData)
        : slave(slave), functionCode(functionCode), startAddr(startAddr), numOfRegs(numOfRegs), writeData(writeData), postDoFunction(NULL), preDoFunction(NULL)
    {
    }
    int slave;
    int functionCode;
    int startAddr;
    int numOfRegs;
    std::string writeData;

    void setPostFunction(std::function<void(int, ModbusTask*, ModbusAdapter*)> function, std::string functionDesc) { postDoFunction = function; postDoFunctionDesc = functionDesc; }
    std::function<void(int, ModbusTask*, ModbusAdapter*)> postDoFunction;
    std::string postDoFunctionDesc;

    void setPreFunction(std::function<int(ModbusTask*, ModbusAdapter*)> function, std::string functionDesc) { preDoFunction = function; preDoFunctionDesc = functionDesc; }
    std::function<int(ModbusTask*, ModbusAdapter*)> preDoFunction;
    std::string preDoFunctionDesc;
};

class ModbusAdapter : public QObject
{
    Q_OBJECT;
public:
    explicit ModbusAdapter(QObject *parent, RegistersModel *regModel, RawDataModel *rawdataModel);
    ~ModbusAdapter();
    void busMonitorRequestData(uint8_t * data,int dataLen);
    void busMonitorResponseData(uint8_t * data,int dataLen);

    void modbusConnectRTU(QString port, int baud, QChar parity, int dataBits, int stopBits, int RTS, int responseTimeOut, int byteTimeout);
    void modbusConnectTCP(QString ip, int port, int timeOut=1);
    void modbusDisConnect();
     
    bool isConnected() const { return m_connected; }

    //void setSlave(int slave) { m_slave = slave; }
    //int getSlave() { return m_slave; }

    //void setFunctionCode(int functionCode) { m_functionCode = functionCode; }
    //int getFunctionCode() { return m_functionCode; }

    //void setStartAddr(int addr) { m_startAddr = addr; }
    //int getStartAddr() { return m_startAddr; }

    //void setNumOfRegs(int num) { m_numOfRegs = num; }
    //int getNumOfRegs() { return m_numOfRegs; }

    void resetCounters();
    void addItems(int startAddr, int numOfRegs);

    void setScanRate(int scanRate) { m_scanRate = scanRate; }

    void startPollTimer();
    void stopPollTimer();

    int getPacketsCount() const { return m_packets; }
    int getErrorsCount() const { return m_errors; }
    ulong getCommMSec() const { return m_msecComm;  }
    ulong getAllMSec() const { return m_timeStart.msecsTo(QDateTime::currentDateTime()); }

    RegistersModel* GetRegistersModel() { return m_regModel; }
    RawDataModel* GetRawDatasModel() { return m_rawModel; }

private:
    int modbusWriteDataRaw(int slave, int functionCode, int startAddr, int numOfRegs, uint16_t* writeData);
    int modbusReadDataRaw(int slave, int functionCode, int startAddr, int numOfRegs, uint16_t* readData = NULL);

public:
    uint16_t* GetReadedData16() { return readDataDest16; }
    uint16_t* GetWritedData16() { return writeDataDest16; }
    modbus_t* GetRawInterface() { return m_modbus; }

private:
    RegistersModel* m_regModel;
    RawDataModel* m_rawModel;
    QString stripIP(QString ip);
    bool m_connected;
    int m_ModBusMode;

    ModbusTask m_currentGuiTask;
    //int m_slave;
    //int m_functionCode;
    //int m_startAddr;
    //int m_numOfRegs;

    int m_scanRate = 200;
    QTimer *m_pollTimer;
    uint m_timerCount;
    uint m_packets;
    uint m_errors;
    ulong m_msecComm = 0;
    QDateTime m_timeStart;

    uint8_t *readDataDest;
    uint16_t *readDataDest16;
    //uint8_t* writeDataDest;
    uint16_t* writeDataDest16;

    modbus_t* m_modbus;

    bool m_useThread;
    
    std::vector<std::tuple<ModbusTask, int, bool>> m_tasksNormal;

private:
    TaskThread<ModbusTask>* m_taskThread;
    int doTask(ModbusTask* task, TaskThread<ModbusTask>*);
    std::string m_currentTaskDesc;
    std::mutex m_mutex;

    void fillWriteData(int functionCode, int numOfRegs);

public:
    TaskThread<ModbusTask>* getTaskThread() { return m_taskThread; }

public:
    ModbusTask* getTaskWriteFile(int subAddr, int nb, std::string writeData);
    ModbusTask* getTaskReadFile(int subAddr, int nb);

    ModbusTask* getTaskWrite(int startAddr, int numOfRegs, std::string writeData);
    ModbusTask* getTaskRead(int startAddr, int numOfRegs);
    ModbusTask* getTaskWait(std::function<void(int, ModbusTask*, ModbusAdapter*)> function = NULL, std::string postDoFuncDesc = "");
    
public:
    void addTask(ModbusTask* task, int priority = 0);
    void addTaskAsNormal(ModbusTask* task, int timeInterval, bool showInGui);
    int getTaskCnt(int priority = 0) { assert(priority >= 0 && priority < TASKQUEUE_MAX_COUNT); return m_taskThread->getTasksCnt(priority); }
    void clearTasks(int priority = 0) { assert(priority >= 0 && priority < TASKQUEUE_MAX_COUNT); m_taskThread->clearTasks(priority);  assert(m_taskThread->getTasksCnt(priority) == 0); }
    int getTaskLastRet(int priority = 0) { assert(priority >= 0 && priority < TASKQUEUE_MAX_COUNT);  return m_taskThread->LastRets[priority]; }
    void clearTaskLastRet(int priority = 0) { assert(priority >= 0 && priority < TASKQUEUE_MAX_COUNT);  m_taskThread->LastRets[priority] = -1; }
    std::string getCurrentTaskDesc() { std::lock_guard<std::mutex> lock(m_mutex); return m_currentTaskDesc; }

signals:
    void refreshView();

public slots:
    void modbusTransaction();

private slots:
    void updateState(int ret);
};

#endif // MODBUSADAPTER_H
