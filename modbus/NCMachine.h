#pragma once

#ifndef __NCMACHINE_H
#define __NCMACHINE_H

#pragma warning(disable : 4100)

#include <QObject>
#include <cbang/geom/Vector.h>
#include <cbang/json/JSON.h>
#include "modbusadapter.h"
#include "NCMachineDefines.h"
#include "utils/TaskThread.h"
#include "NCMachineProperties.h"
#include "utils/GCodeTool.h"
#include "gcode/RealtimeJsonMachine.h"

//#define BEGIN_TASK_GCODE 
//#define END_TASK 

//#define BEGIN_TASK_NORMAL m_currentTaskPriorityPrev = m_currentTaskPriority; m_currentTaskPriority = 2;

#define TIMEOUT_WAIT_COUNT -1
//#define TIMEOUT_WAIT_MILLSECONDS 50
#define TASK_WAIT_MILLSECONDS_LARGE 200
#define GCODE_TASK_PRIOPROTY 1

using DdfxEnum = ::BasePropertyObject::DdfxEnum;

struct GCodeTask
{
	GCodeTask(std::string jCode, int lineNumber, int priority)
		: jCode(jCode), lineNumber(lineNumber), priority(priority)
	{
	}

	std::string jCode;
	int lineNumber;
	int priority;

	std::function<void(int, GCodeTask*, NCMachine*)> postDoFunction;
	std::string postDoFunctionDesc;
};

struct G01Data
{
	bool isPausing = false;
	bool isRunning = false;
	QDateTime startTime;
	int step = 0;
	Vector3I from_p;
	Vector3I to_p;

	int start_z = 0;
	int to_z = 0;
	int now_z = 0;
	int dssj = 0;
	//GCode::MachineEnum::plane_t plane = GCode::MachineEnum::plane_t::XY;
	long allWorkSeconds = 0;	// 总的加工时间
	long lastWorkSeconds = 0;	// 前一个G01的加工时间
	long nowWorkSeconds = 0;	// 如果暂停，这个值保存已经工作了的时间
	int cNo = 0;
	int lastSV = 3;
	cb::Vector4I lastPos;	// pos for pause
	bool lastPosSet = false;
	bool newData = false;	// 是否是一次新的G01，用于刷新加工参数
	QList<Vector4I> lastPosList;	// 用于Pausing的时候保存Jog轨迹
	bool lastYb1 = false;	// 用于Pausing的时候保存Yb状态
	bool lastYb2 = false;	
	bool isSimulate = false;
	bool powerOffAfterDone = false;

	int dssysj = 0; // 定时剩余时间
	int jgcyl = 0; // 加工残余量

	cb::Vector4I startPos;	// pos before g01

	QString workName;
	QDateTime workStartDate;
	QDateTime workEndDate;
	QList<std::tuple<QString, QDateTime, QDateTime, int>> workDetails; // 记录每次加工的时间段
};

class ModbusTaskCache
{
public:
	void addTask(ModbusTask* task, int priority = 0) {
		assert(priority >= 0 && priority < TASKQUEUE_MAX_COUNT);
		m_modbusTasksCache[priority].push(task);
	}
	//ModbusTask* addTaskWriteFile(int subAddr, int nb, std::string writeData, int priority = 0) {
	//	ModbusTask* task = new ModbusTask(DEFAULT_MODBUS_SLAVE, MODBUS_FC_WRITE_FILE_RECORD, subAddr, nb, writeData);
	//	return addTask(task, priority);
	//}
	//ModbusTask* addTaskReadFile(int subAddr, int nb, int priority = 0) {
	//	ModbusTask* task = new ModbusTask(DEFAULT_MODBUS_SLAVE, MODBUS_FC_READ_FILE_RECORD, subAddr, nb);
	//	return addTask(task, priority);
	//}

	//ModbusTask* addTaskWrite(int startAddr, int numOfRegs, std::string writeData, int priority = 0) {
	//	ModbusTask* task = new ModbusTask(DEFAULT_MODBUS_SLAVE, DEFAULT_MODBUS_WRITE_FUNCTION, startAddr, numOfRegs, writeData);
	//	return addTask(task, priority);
	//}
	//ModbusTask* addTaskRead(int startAddr, int numOfRegs, int priority = 0) {
	//	ModbusTask* task = new ModbusTask(DEFAULT_MODBUS_SLAVE, DEFAULT_MODBUS_READ_FUNCTION, startAddr, numOfRegs);
	//	return addTask(task, priority);
	//}
	//ModbusTask* addTaskWait(int priority = 0, std::function<void(int, ModbusTask*, ModbusAdapter*)> function = NULL, std::string postDoFuncDesc = "") {
	//	ModbusTask* task = new ModbusTask(DEFAULT_MODBUS_SLAVE, MODBUS_FC_WAIT, -1, 0);
	//	task->setPostFunction(function, postDoFuncDesc);
	//	return addTask(task, priority);
	//}

	int getTaskCnt(int priority = 0) {
		assert(priority >= 0 && priority < TASKQUEUE_MAX_COUNT);
		return m_modbusTasksCache[priority].size();
	}
	void AddToModbusTask(ModbusAdapter* modbus, int priority)
	{
		while (m_modbusTasksCache[priority].size() > 0) {
			modbus->addTask(m_modbusTasksCache[priority].front(), priority);
			m_modbusTasksCache[priority].pop();
		}
	}
private:
	MultiThreadQueue<ModbusTask*> m_modbusTasksCache[TASKQUEUE_MAX_COUNT];
};

class NCMachine : public QObject
{
	Q_OBJECT;

public:
	explicit NCMachine(QObject* parent, ModbusAdapter* adapter);
	//explicit NCMachine(ModbusAdapter* adapter);
	~NCMachine();

	void ReadAllPosAndState();

	bool ConvertModbusData4Key(ModbusTask* task, uint16_t* readData);
	bool ConvertModbusData4Pos(ModbusTask* task, uint16_t* readData);
	bool ConvertModbusData4State(ModbusTask* task, uint16_t* readData);
	bool ConvertModbusData4JogDuanlu(ModbusTask* task, uint16_t* readData);

	//cb::Vector4I GetXYZUDirect();
	//cb::Vector3I GetXYZDirect();
	//cb::Vector2I GetXYDirect();
	//cb::Vector<3, uint16_t> GetNCTStateDirect();

	cb::Vector4I GetXYZU() { return m_pos; }

	//cb::Vector4I GetXYZUWithOffset() { return m_pos + m_posOffset; }
	cb::Vector3U GetState() { return m_state; }
	uint16_t GetNctState() { return m_state[2]; }
	uint16_t GetRLSTRslt() { return m_state[0]; }
	uint16_t GetRLSTPara() { return m_state[1]; }

	void SetState4Debug(uint16_t t1, uint16_t t2, uint16_t t3 ) { m_stateDebug[0] = t1; m_stateDebug[1] = t2; m_stateDebug[2] = t3;}

	/*
	手动移动标志。
Bit0：X+   Bit1：X-
Bit2：Y+   Bit3：Y-
Bit4：Z+   Bit5：Z-
Bit6~Bit13：保留位
Bit14：CMV（条件移动）
Bit15：ST  （取消短路保护） 

	*/
	bool GetScamanulFlag(int bitIndex) const;
	/* 
	输入信号（机床信号输入）
Data1（L16）
Bit0：XL+限位  Bit1：XL-限位
Bit2：YL+限位  Bit3：YL+限位 Bit4：ZL+限位  Bit5：ZL+限位 
Bit6、7：保留
Bit8：油温报警Bit9：油位报警
Bit10：火焰报警
Bit11~Bit15：保留

Data2（H16）
Bit0：X轴的零位信号
Bit1：Y轴的零位信号
Bit2：Z轴的零位信号
Bit3：U轴的零位信号
Bit4~6：保留
Bit7：伺服电机报警标志
Bit8~15：保留
	*/
	bool GetInputFlag(int bitIndex);
	QString GetDebugMsg();
	bool GetIsJogDuanlu() { return m_isJogDuanlu; }

	int GetTargetPosOne() const { return m_targetPosX; }
	uint16_t GetTargetType() const { return m_targetType; }

	static QString GetStateDesc(uint16_t state);
	static QString GetRLSTDesc(uint16_t rslt, uint16_t para);

	// Modbus
	ModbusAdapter* getModbus() { return m_modbus; }
	QList<ModbusTask*> executeCmdsInFile(std::string filename);
	QList<ModbusTask*> executeCmds(std::string s);
	QList<ModbusTask*> executeCmds(cb::JSON::ValuePtr json);

	QList<ModbusTask*> executeCmdsByAddress(int address, std::vector<uint16_t> vs);
	QList<ModbusTask*> executeCmdsByTncOrder(int tncOrder, std::vector<uint16_t> vs);

	ModbusTask* executeCmdWait(std::function<void(int, ModbusTask*, ModbusAdapter*)> func, std::string desc = "");

	//std::function<void(int, ModbusTask*, ModbusAdapter*)> waitUntilNctState(uint16_t state, int timeout = TIMEOUT_WAIT_COUNT);
	//std::function<void(int, ModbusTask*, ModbusAdapter*)> waitUntilRLST(uint16_t rslt, uint16_t para, int timeout = TIMEOUT_WAIT_COUNT);

	std::function<void()> doWhenWaitFail = NULL;
	std::function<int()> waitno(std::function<void()> doAfter = NULL);
	std::function<int()> waitUntilNctState(uint16_t state);
	std::function<int()> waitUntilRLST(uint16_t rslt, uint16_t para = 0);
	std::function<void(int, ModbusTask*, ModbusAdapter*)> convertWaitFunction(std::function<int()> func2, int timeout = TIMEOUT_WAIT_COUNT);

	// 设置机械坐标，目前只能清零
	void SetPosOne(int axis, int value);

	void GoApi();
	void GoJog();
	void LocSeq(int* axis, int* value, int size);
	void Loc(int* axis, int* value, int size);
	void Loc(int axis, int value);
	void Spk(int* axis, int* value, int size, int zgj = 1);
	void Spk(int axis, int value);
	void Beep(int n = 1, int ms = 50);
	bool CanBeJogDDMS();
	void SetSpeedGear(int v);

	void Touch(DdfxEnum ddfx);
	void ToLMT(DdfxEnum ddfx);
	void ToLMTSoft(DdfxEnum ddfx);
	void FromLMT(DdfxEnum ddfx);
	void ToZSig(DdfxEnum ddfx);

	void BacktoZeroSimple(int axis);
	void BacktoZero(DdfxEnum ddfx);
	void PrepareCent();

	// 0: none
	// 1: PrepareCent
	// 2: BacktoZero
	int GetNowOperating() const { return m_nowOperating; }
	void SetNowOperating(int nowOperating) { m_nowOperating = nowOperating; }

	void PowerOn();
	void PowerOff();

	G01Data& GetG01Data() { std::lock_guard<std::mutex> lock(m_mutex); return m_g01Data; }
	QDateTime GetDwellEndTime() { return m_dwellEndTime; }

	void SetInputKey(unsigned long long key);

signals:
	void keyPressed(unsigned long long);
	void positionChanged();

private:
	void ProcessState();
	bool HasError();

	void ProcessKey();
	
	void ProcessPadset();

	void ProcessPos();

	ModbusAdapter* m_modbus;

	// 机械坐标
	cb::Vector4I m_pos;
	cb::Vector4I m_axisPositionLastSend;
	int m_axisCsLast = -1;

	// 用户坐标偏移： 用户坐标=机械坐标+偏移
	//cb::Vector4I m_posOffset;

	// 指令执行结果，NCTState
	// 2: 运行状态
	// 0: RLST.u16Rslt
	// 1: RLST.u16Para 
	cb::Vector<3, uint16_t> m_state;
	// 端口输入信息，限位、回零等
	cb::Vector<2, uint16_t> m_inputFlag;

	// 手动移动标志
	uint16_t m_scamanulFlag;

	int m_isJogDuanlu = 0;

	cb::Vector3U m_stateDebug;
	unsigned long long m_key;
	unsigned long long m_keyPrev;
	int m_targetPosX;
	uint16_t m_targetType;

	std::atomic<int> m_nowOperating = 0;
#ifdef BACKZERO_METHOD2
	int m_isBackzeroing = 0;
	int m_isZeroAsix = -1;
#endif // BACKZERO_METHOD2

	std::function<void(int, ModbusTask*, ModbusAdapter*)> m_functionWaitUntilApirun;

	std::condition_variable m_priorityCondition;
	std::atomic<int> m_currentTaskPriority = -1;
	//std::atomic<int> m_currentTaskPriorityPrev = -1;
	std::atomic<int> m_currentTaskPriorityLockCnt = 0;

	int m_currentGCodeLine = 0;
	QStringList m_currentGCodes;
	std::string m_currentJCode;
	std::string m_currentTaskDesc;
	
	std::mutex m_mutex;

	QHash<int, QStringList> m_paramCs;
	QStringList m_paramCsNames;

	QDateTime m_dwellEndTime;
	G01Data m_g01Data;
	//QDateTime m_maxTime;
private:
	// JCode
	TaskThread<GCodeTask>* m_taskThread;
	int doTask(GCodeTask* task, TaskThread<GCodeTask>* taskThread);
	std::vector<std::tuple<std::function<int()>, std::string>> doTaskJson(cb::JSON::ValuePtr json, GCodeTask* task, TaskThread<GCodeTask>* taskThread);

	std::mutex m_priorityMutex;


public:
	void enterSetPriority(int priority);
	void exitSetPriority();
	GCodeTask* addTask(std::string jCode, int lineNumber, int priority);
	int getTaskCnt(int priority = 0) { return m_taskThread->getTasksCnt(priority); }
	void clearTasks(int priority = 0) { m_taskThread->clearTasks(priority); }

	void startTaskThread();
	void stopTaskThread() { m_taskThread->stopTaskThread(); };

	bool isTaskThreadPausing() { return m_taskThread->isTaskThreadPausing(); }

public:
	QString RunCCode(QString gcode);
	QString RunHCode(QString gcode);
	static QString ParseCCode(QString gcode);
	static QString ParseHCode(QString gcode);
	static QHash<QString, QString> ParseCCodeLine(QString cCode);

	bool RunGCodeSync(QString gcode);
	bool RunGCode(QString gcode, std::function<void()> functionDoFinished = NULL);
	
	void StopRun();
	void PauseRun();
	void ResumeRun();
	int WaitUntilRunFinished();

	bool serialize();
	bool deserialize();
private:
	QString PreprocessGCode(QString gcode);
	
	bool RunGCodeAsync(QString gcode, std::function<void()> functionDoFinished = NULL);
	//void RunJcode(cb::JSON::ValuePtr json, std::function<void(int, GCodeTask*, NCMachine*)> function = NULL);
	//void RunJcode(std::string action, std::function<void(int, GCodeTask*, NCMachine*)> function = NULL);
	//void RunGCode(cb::JSON::ValuePtr json, std::function<void(int, GCodeTask*, NCMachine*)> function = NULL);
	GCodeTool* m_gcodeTool;
	SmartPointer<RealtimeJsonMachine> m_realtimeJsonMachine;
	std::thread m_realtimeJsonMachineThread;
	std::atomic<bool> m_realtimeJsonMachineThreadDone;

	GCodeTool* m_gcodeTool2;
	SmartPointer<RealtimeJsonMachine> m_realtimeJsonMachine2;

	void SaveWorkTimeToDb();
public:
	// modbus队列中如果有任务，也算在运行中
	// || this->m_modbus->getTaskCnt(0) > 0: 这个不能加，可能某些时候会有其他命令，但这时GCode可以运行
	bool IsGCodeRunning() { return this->getTaskCnt(1) > 0 || this->getTaskCnt(0) > 0 || this->IsRealtimeJsonMachineRunning() || this->m_modbus->getTaskCnt(1) > 0; }
	bool IsRealtimeJsonMachineRunning() { return m_gcodeTool->IsRunning() || !m_realtimeJsonMachineThreadDone;  }
	cb::SmartPointer<ControllerImpl> GetController() { return m_gcodeTool->GetController(); }
	cb::SmartPointer<RealtimeJsonMachine> GetRealtimeJsonMachine() { return m_realtimeJsonMachine; }

private:
	ModbusTaskCache m_modbusTaskCache;
};


class NCMachineWrapper : public QObject {

	Q_OBJECT

public Q_SLOTS:
	void SetInputKey(NCMachine* o, unsigned long long key) { o->SetInputKey(key); }
	bool RunGCode(NCMachine* o, QString gcode) { return o->RunGCode(gcode); }
	bool RunGCodeSync(NCMachine* o, QString gcode) { return o->RunGCodeSync(gcode); }
	QVariantList GetState(NCMachine* o) { return ToQVariantList(o->GetState()); }
	QVariantList GetXYZU(NCMachine* o) { return ToQVariantList(o->GetXYZU()); }
	// https://linuxcnc.org/docs/html/gcode/overview.html
	// set("_   
	double GetControllerParam(NCMachine* o, QString param) { return o->GetController()->get(EUtils::QString2StdString(param)); }
	// GetControllerParam(5220)	# CURRENT_COORD_SYSTEM = 5220,
	double GetControllerParam(NCMachine* o, int addr) { return o->GetController()->get((address_t)addr); }

private:
	QVariantList ToQVariantList(const cb::Vector3U& list) {
		QVariantList varList;
		varList.append(QVariant(list.x()));
		varList.append(QVariant(list.y()));
		varList.append(QVariant(list.z()));
		return varList;
	}
	QVariantList ToQVariantList(const cb::Vector4I& list) {
		QVariantList varList;
		varList.append(QVariant(list.x()));
		varList.append(QVariant(list.y()));
		varList.append(QVariant(list.z()));
		varList.append(QVariant(list.data[3]));
		return varList;
	}
};

#endif