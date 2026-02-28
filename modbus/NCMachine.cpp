#pragma warning(disable : 4100)

#include <thread>
#include <chrono>
#include <limits>
#include <cmath>
#include <iomanip> // For std::fixed and std::setprecision
#include <cbang/json/JSON.h>
#include <cbang/io/InputSource.h>
#include <cbang/Catch.h>
#include <cbang/log/Logger.h>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QThread>
#include "NCCommand.h"
#include "NCMachine.h"
#include "utils/TaskThread.cpp"
#include "utils/GCodeTool.h"
#include "utils/LineLogger.h"
#include "NCMachineParametersC.h"
#include "utils/gcodeutils.h"
#include "utils/dataform.h"
#include "BaseMainWindow.h"
#include <QException>
#include "utils/nfile.h"
#include "utils/formutils.h"
#include "utils/MathTool.h"
#include "forms/other/jiagongjiluform.h"

//template class TaskThread<cb::JSON::Value>;

NCMachine::NCMachine(QObject* parent, ModbusAdapter* adapter) :
	QObject(parent), m_modbus(adapter), m_taskThread(NULL),
	m_keyPrev(0), m_key(0), m_targetPosX(0), m_targetType(0)
{
	//Vector3D p = MathTool::TransformPoint(Vector3D(2.5, 4.5, 0), Vector3D(0, 0, 0), Vector3D(6, 2, 0));

	//m_maxTime = QDateTime::fromTime_t(1 << 31);

	m_functionWaitUntilApirun = convertWaitFunction(waitUntilNctState(NCT_STATE_API_RUN));

	for (int i = 0; i < 3; ++i)
		m_stateDebug[i] = 0;

	m_gcodeTool = new GCodeTool();
	m_realtimeJsonMachineThreadDone = true;
	m_realtimeJsonMachine = new RealtimeJsonMachine(this, GCode::Units::METRIC);

	m_gcodeTool2 = new GCodeTool();
	m_realtimeJsonMachine2 = new RealtimeJsonMachine(this, GCode::Units::METRIC);
}

NCMachine::~NCMachine()
{
	StopRun();

	if (m_taskThread) {
		m_taskThread->stopTaskThread();
		delete m_taskThread;
	}
	if (m_gcodeTool) {
		//m_gcodeTool->serialize();
		delete m_gcodeTool;
	}
	if (m_gcodeTool2) {
		delete m_gcodeTool2;
	}
}

void NCMachine::ReadAllPosAndState()
{
	assert(TMBS_MAP0_ID_XPOS_LEN == 2);
	assert(TMBS_MAP0_ID_NCTSTATE_LEN + TMBS_MAP0_ID_RSLT_LEN == 3);

	std::function<void(int, ModbusTask*, ModbusAdapter*)> function1 = [this](int ret, ModbusTask* task, ModbusAdapter* adapter) {
		if (ret == -1)
			return;
		uint16_t* readData = adapter->GetReadedData16();
		ConvertModbusData4Pos(task, readData);

		uint16_t* readData2 = &readData[TMBS_MAP0_ID_XPOS_LEN * 4];
		ConvertModbusData4State(task, readData2);
	};
	ModbusTask* task = m_modbus->getTaskRead(TMBS_MAP0_ID_XPOS, TMBS_MAP0_ID_XPOS_LEN * 4 + TMBS_MAP0_ID_RSLT_LEN + TMBS_MAP0_ID_NCTSTATE_LEN);
	task->setPostFunction(function1, "Read Pos and State");
	m_modbusTaskCache.addTask(task, m_currentTaskPriority);
	if (m_currentTaskPriority <= 1)
		LOG_INFO(8, "NCMachine-Modbus: addTaskRead(" << m_currentTaskPriority << ", " << m_modbus->getTaskCnt(m_currentTaskPriority) << "):" << TMBS_MAP0_ID_XPOS << ", " << (TMBS_MAP0_ID_XPOS_LEN * 4 + TMBS_MAP0_ID_RSLT_LEN + TMBS_MAP0_ID_NCTSTATE_LEN));
}

bool NCMachine::ConvertModbusData4Key(ModbusTask* task, uint16_t* readData)
{
	assert(task->startAddr == TMBS_MAP0_ID_KEY);
	assert(task->functionCode == MODBUS_FC_READ_HOLDING_REGISTERS);

	assert(TMBS_MAP0_ID_KEY_LEN == 4);
	assert(TMBS_MAP0_ID_HOPR_LEN == 3);

	if (readData == NULL)
		return false;

	if (task->numOfRegs >= TMBS_MAP0_ID_KEY_LEN)
	{
		m_key = static_cast<long long>(readData[0]) + static_cast<long long>(readData[1] << 16) + (static_cast<long long>(readData[2]) << 32) + (static_cast<long long>(readData[3]) << 48);
		//LOG_INFO(0, EUtils::QString2StdString(QStringLiteral("Key：%1").arg(m_key)));

		// key down
		if (m_key != KNLK_NONE && m_key != KNLK_INVALID) {
			if (m_key != m_keyPrev) {
				m_keyPrev = m_key;
				ProcessKey();
			}
		}
		else {
			// key up
			if (m_keyPrev != KNLK_NONE && m_keyPrev != KNLK_INVALID)
			{
				m_keyPrev = m_key;
				ProcessKey();
			}
		}
	}

	if (task->numOfRegs >= TMBS_MAP0_ID_KEY_LEN + TMBS_MAP0_ID_HOPR_LEN)
	{
		m_targetPosX = readData[4] + (readData[5] << 16);
		m_targetType = readData[6];

		ProcessPadset();
	}

	return true;
}

bool NCMachine::ConvertModbusData4Pos(ModbusTask* task, uint16_t* readData)
{
	assert(task->startAddr == TMBS_MAP0_ID_XPOS);
	assert(task->functionCode == MODBUS_FC_READ_HOLDING_REGISTERS);

	assert(TMBS_MAP0_ID_XPOS_LEN == 2);

	if (readData == NULL)
		return false;

	if (task->numOfRegs >= TMBS_MAP0_ID_XPOS_LEN * 4) {
		int x = readData[0] + (readData[1] << 16);
		int y = readData[2] + (readData[3] << 16);
		int z = readData[4] + (readData[5] << 16);
		int u = readData[6] + (readData[7] << 16);
		m_pos = cb::Vector4I(x, y, z, u);
	}
	else if (task->numOfRegs >= TMBS_MAP0_ID_XPOS_LEN * 3) {
		int x = readData[0] + (readData[1] << 16);
		int y = readData[2] + (readData[3] << 16);
		int z = readData[4] + (readData[5] << 16);
		//int u = readData[6] + (readData[7] << 16);
		m_pos = cb::Vector4I(x, y, z, 0);
	}

	auto unit = PropertyObjects::getInstance()->propertyObjectShouKongHe->isgz() ? GCode::Units::METRIC : GCode::Units::IMPERIAL;
	GetController()->setAxisAbsolutePosition('X', m_pos.x() / 1000.0, unit);
	GetController()->setAxisAbsolutePosition('Y', m_pos.y() / 1000.0, unit);
	GetController()->setAxisAbsolutePosition('Z', m_pos.z() / 1000.0, unit);
	GetController()->setAxisAbsolutePosition('U', m_pos.data[3] / 1000.0, unit);

	ProcessPos();

	return true;
}

bool NCMachine::ConvertModbusData4State(ModbusTask* task, uint16_t* readData)
{
	assert(task->startAddr <= TMBS_MAP0_ID_RSLT);
	assert(task->functionCode == MODBUS_FC_READ_HOLDING_REGISTERS);

	assert(TMBS_MAP0_ID_RSLT_LEN == 2);
	assert(TMBS_MAP0_ID_NCTSTATE_LEN == 1);
	assert(TMBS_MAP0_ID_INPUT_LEN == 2);

	if (readData == NULL) {
		m_state = cb::Vector<3, uint16_t>(NCT_STATE_INVALID, NCT_STATE_INVALID, NCT_STATE_INVALID);
		m_scamanulFlag = 0;
		m_inputFlag = cb::Vector<2, uint16_t>(0, 0);
		return false;
	}
	if (task->numOfRegs >= TMBS_MAP0_ID_RSLT_LEN + TMBS_MAP0_ID_NCTSTATE_LEN) {
		m_state = cb::Vector<3, uint16_t>(readData[0], readData[1], readData[2]);
		for (int i = 0; i < 3; ++i) {
			if (m_stateDebug[i] != 0) {
				m_state[i] = m_stateDebug[i];
				m_stateDebug[i] = 0;
			}
		}

		ProcessState();
	}
	if (task->numOfRegs >= TMBS_MAP0_ID_RSLT_LEN + TMBS_MAP0_ID_NCTSTATE_LEN + TMBS_MAP0_ID_SCAMANUL_LEN) {
		m_scamanulFlag = readData[3];
	}

	if (task->numOfRegs >= TMBS_MAP0_ID_RSLT_LEN + TMBS_MAP0_ID_NCTSTATE_LEN + TMBS_MAP0_ID_SCAMANUL_LEN + TMBS_MAP0_ID_INPUT_LEN) {
		m_inputFlag = cb::Vector<2, uint16_t>(readData[4], readData[5]);
		//m_input = cb::Vector<2, uint16_t>(1, 1);
	}

	return true;
}
bool NCMachine::ConvertModbusData4JogDuanlu(ModbusTask* task, uint16_t* readData)
{
	if (readData == NULL)
		return false;

	// 如果不是JOG状态，不Beep
	if (m_state[2] != NCT_STATE_JOG_RUN) {
		if (m_isJogDuanlu > 0) {
			m_isJogDuanlu = 0;
			if (PropertyObjects::getInstance()->propertyObjectFengMingQi->mjfs() == FmqPropertyObject::MjfsEnum::TYPE2) {
				enterSetPriority(0);
				Beep(0, 0);
				exitSetPriority();
			}
		}
		return false;
	}
	assert(task->startAddr == TMBS_MAP0_ID_JOG_DUANLU);
	assert(task->functionCode == MODBUS_FC_READ_HOLDING_REGISTERS);

	assert(TMBS_MAP0_ID_JOG_DUANLU_LEN == 1);

	// not in ST
	if (readData[0] > 32767 && !this->GetScamanulFlag(15)) {
		m_isJogDuanlu = 1;
	}
	else {
		m_isJogDuanlu = 0;
	}

	if (m_isJogDuanlu > 0 && m_key != KNLK_ST) {
		if (PropertyObjects::getInstance()->propertyObjectFengMingQi->mjfs() != FmqPropertyObject::MjfsEnum::TYPE2) {
			enterSetPriority(0);
			Beep(0, -1);
			exitSetPriority();
			//m_isJogDuanlu = 0;
		}
	}
	else {
		if (PropertyObjects::getInstance()->propertyObjectFengMingQi->mjfs() == FmqPropertyObject::MjfsEnum::TYPE2) {
			enterSetPriority(0);
			Beep(0, 0);
			exitSetPriority();
		}
	}
	return true;
}
//cb::Vector4I NCMachine::GetXYZUDirect()
//{
//	uint16_t* readData = new uint16_t[TMBS_MAP0_ID_XPOS_LEN * 4];
//
//	m_modbus->modbusReadDataRaw(m_modbus->getSlave(), MODBUS_FC_READ_HOLDING_REGISTERS, TMBS_MAP0_ID_XPOS, TMBS_MAP0_ID_XPOS_LEN * 4, readData);
//	int x = readData[0] + (readData[1] << 16);
//	int y = readData[2] + (readData[3] << 16);
//	int z = readData[4] + (readData[5] << 16);
//	int u = readData[6] + (readData[7] << 16);
//	delete[] readData;
//
//	cb::Vector4I pos = cb::Vector4I(x, y, z, u);
//	return pos;
//}
//
//cb::Vector3I NCMachine::GetXYZDirect()
//{
//	uint16_t* readData = new uint16_t[TMBS_MAP0_ID_XPOS_LEN * 3];
//
//	m_modbus->modbusReadDataRaw(m_modbus->getSlave(), MODBUS_FC_READ_HOLDING_REGISTERS, TMBS_MAP0_ID_XPOS, TMBS_MAP0_ID_XPOS_LEN * 3, readData);
//	int x = readData[0] + (readData[1] << 16);
//	int y = readData[2] + (readData[3] << 16);
//	int z = readData[4] + (readData[5] << 16);
//	delete[] readData;
//
//	cb::Vector3I pos = cb::Vector3I(x, y, z);
//	return pos;
//}
//
//cb::Vector2I NCMachine::GetXYDirect()
//{
//	uint16_t* readData = new uint16_t[TMBS_MAP0_ID_XPOS_LEN * 2];
//
//	m_modbus->modbusReadDataRaw(m_modbus->getSlave(), MODBUS_FC_READ_HOLDING_REGISTERS, TMBS_MAP0_ID_XPOS, TMBS_MAP0_ID_XPOS_LEN * 2, readData);
//	int x = readData[0] + (readData[1] << 16);
//	int y = readData[2] + (readData[3] << 16);
//	delete[] readData;
//
//	cb::Vector2I pos = cb::Vector2I(x, y);
//	return pos;
//}
//
//cb::Vector<3, uint16_t> NCMachine::GetNCTStateDirect()
//{
//	assert(TMBS_MAP0_ID_NCTSTATE_LEN + TMBS_MAP0_ID_NCTSTATE_LEN == 3);
//
//	uint16_t* readData = new uint16_t[TMBS_MAP0_ID_NCTSTATE_LEN + TMBS_MAP0_ID_NCTSTATE_LEN];
//	m_modbus->modbusReadDataRaw(m_modbus->getSlave(), MODBUS_FC_READ_HOLDING_REGISTERS, TMBS_MAP0_ID_NCTSTATE, TMBS_MAP0_ID_NCTSTATE_LEN + TMBS_MAP0_ID_NCTSTATE_LEN, readData);
//	cb::Vector<3, uint16_t> ret = cb::Vector<3, uint16_t>(readData[0], readData[1], readData[2]);
//	delete[] readData;
//	return ret;
//}

bool NCMachine::GetScamanulFlag(int bitIndex) const
{
	if (bitIndex < 0 || bitIndex >= 16)
		return false;
	return (m_scamanulFlag >> bitIndex) & 1;
}

bool NCMachine::GetInputFlag(int bitIndex)
{
	//if (bitIndex == 9)
	//	return true;
	if (bitIndex < 0 || bitIndex >= 32)
		return false;
#ifdef _DEBUG999
	m_inputFlag[0] = 0x00;
	m_inputFlag[1] = 0x00;
	//m_inputFlag[0] = 0x10;
#endif
	if (bitIndex < 16) {
		return (m_inputFlag[0] >> bitIndex) & 1;
	}
	else {
		return (m_inputFlag[1] >> (bitIndex - 16)) & 1;
	}
	return false;
}

QString NCMachine::GetDebugMsg()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return QString("Speed: %1\n Modbus Task: %2,%3,%4 \nCurrent: %5; GCode Task: %6,%7 \nCurrent: %8,%9,%10")
		.arg(PropertyObjects::getInstance()->propertyObjectJog->xV())
		.arg(m_modbus->getTaskCnt(0)).arg(m_modbus->getTaskCnt(1)).arg(m_modbus->getTaskCnt(2))
		.arg(QString::fromStdString(m_modbus->getCurrentTaskDesc()))
		.arg(m_taskThread->getTasksCnt(0)).arg(m_taskThread->getTasksCnt(1))
		.arg(m_currentGCodeLine).arg(QString::fromStdString(m_currentJCode))
		.arg(QString::fromStdString(m_currentTaskDesc));
}

QString NCMachine::GetStateDesc(uint16_t state)
{
	switch (state)
	{
	case NCT_STATE_API_RUN:
		return QStringLiteral("无移动状态（API）");
	case NCT_STATE_JOG_RUN:
		return QStringLiteral("JOG运行");
	case NCT_STATE_JOG_EXIT:
		return QStringLiteral("JOG退出");
	case NCT_STATE_LOC_RUN:
		return QStringLiteral("LOC运行");
	case NCT_STATE_CMV_RUN:
		return QStringLiteral("CMV运行");
	case NCT_STATE_CMV_EXIT:
		return QStringLiteral("CMV退出");
	case NCT_STATE_CMV_FINISHED:
		return QStringLiteral("CMV完成");
	case NCT_STATE_SPK_RUN:
		return QStringLiteral("SPK运行");
	case NCT_STATE_SPK_EXTOC:
		return QStringLiteral("SPK退出前回中心");
	case NCT_STATE_SPK_EXBAK:
		return QStringLiteral("SPK退出前回退（先回中心再回退）");
	case NCT_STATE_SPK_PMBAK:
		return QStringLiteral("SPK抬刀前回退");
	case NCT_STATE_SPK_PMOV:
		return QStringLiteral("SPK抬刀");
	case NCT_STATE_SPK_TOSPK:
		return QStringLiteral("SPK抬刀结束去加工");
	}
	return QStringLiteral("未定义状态：%1").arg(state);
}

QString NCMachine::GetRLSTDesc(uint16_t rslt, uint16_t para)
{
	switch (rslt)
	{
	case 0:
		return "";
		//return QStringLiteral("0,%1").arg(para);
		break;
	case 2 + 256:
		//if (para == (1 << 15))
			return QStringLiteral("JOG手动退出");
		break;
	case 1 + 512:
		//if (para == 0)
			return QStringLiteral("LOC正常完成退出");
		break;
	case 2 + 512:
		//if (para == (1 << 15))
			return QStringLiteral("LOC手动退出");
		break;
	case 3 + 512:
		//if (para == 0xFFFF)
			return QStringLiteral("LOC驱动器报警退出：%1").arg(para);
		break;
	case 4 + 512:
		return QStringLiteral("LOC触发限位退出：%1").arg(para);
		break;
	case 5 + 512:
		//if (para == 0)
			return QStringLiteral("LOC短路退出");
		break;
	case 61 + 512:
		//if (para == 0)
			return QStringLiteral("LOC参数非法");
		break;
	case 62 + 512:
		//if (para == 0)
			return QStringLiteral("LOC校验错误");
		break;
	case 63 + 512:
		//if (para == 0)
			return QStringLiteral("LOC无效功能");
		break;
	case 1 + 768:
		//if (para == 0)
			return QStringLiteral("CMV正常完成退出");
		break;
	case 2 + 768:
		//if (para == (1 << 15))
			return QStringLiteral("CMV手动退出");
		break;
	case 3 + 768:
		//if (para == (0xFFFF))
			return QStringLiteral("CMV驱动器报警退出：%1").arg(para);
		break;
	case 4 + 768:
		return QStringLiteral("CMV触发限位退出：%1").arg(para);
		break;
	case 5 + 768:
		//if (para == 0)
			return QStringLiteral("CMV短路退出");
		break;
	case 53 + 768:
		//if (para == 0)
			return QStringLiteral("CMV轴编码错误");
		break;
	case 54 + 768:
		//if (para == 0)
			return QStringLiteral("CMV操作码错误");
		break;
	case 51 + 768:
		//if (para == 0)
			return QStringLiteral("CMV无效功能");
		break;
	case 1 + 1024:
		//if (para == 0)
			return QStringLiteral("SPK正常完成退出");
		break;
	case 2 + 1024:
		//if (para == (1 << 15))
			return QStringLiteral("SPK手动退出");
		break;
	case 3 + 1024:
		//if (para == 0xFFFF)
			return QStringLiteral("SPK驱动器报警退出");
		break;
	case 4 + 1024:
		return QStringLiteral("SPK触发限位退出");
		break;
	case 5 + 1024:
		//if (para == 0)
			return QStringLiteral("SPK短路退出");
		break;
	case 6 + 1024:
	{
		QString s;
		if (para & 0x1)
			s += QStringLiteral("油温");
		if (para & 0x2)
			s += QStringLiteral("油位");
		if (para & 0x4)
			s += QStringLiteral("火焰");
		return QStringLiteral("SPK%1报警退出").arg(s);
	}
		break;
	case 53 + 1024:
		//if (para == 0)
			return QStringLiteral("SPK轴编码错误");
		break;
	case 54 + 1024:
		//if (para == 0)
			return QStringLiteral("SPK操作码错误");
		break;
	case 55 + 1024:
		if (para == 0)
			return QStringLiteral("SPK无效功能");
		break;
	default:
		break;
	}
	return QStringLiteral("未定义：%1,%2").arg(rslt).arg(para);
}


QList<ModbusTask*> NCMachine::executeCmdsInFile(std::string filename)
{
	try {
		cb::JSON::ValuePtr json = cb::JSON::Reader::parse(cb::InputSource::open(filename));
		return executeCmds(json);
	}
	catch (const std::exception& e) {
		LOG_WARNING("NCMachine: exception: " << e.what() << ", executeCmdsInFile" << filename);
	}
	catch (...) {
		LOG_ERROR("NCMachine: " << "Unknown error" << ", executeCmdsInFile" << filename);
	}
	return QList<ModbusTask*>();
}

QList<ModbusTask*> NCMachine::executeCmds(std::string s)
{
	try {
		cb::JSON::ValuePtr json = cb::JSON::Reader::parse(s);
		return executeCmds(json);
	}
	catch (const std::exception& e) {
		LOG_WARNING("NCMachine: exception: " << e.what() << ", executeCmds" << s);
	}
	catch (...) {
		LOG_ERROR("NCMachine: exception: " << "Unknown error" << ", executeCmds" << s);
	}
	return QList<ModbusTask*>();
}

QList<ModbusTask*> NCMachine::executeCmds(cb::JSON::ValuePtr json)
{
	QList<ModbusTask*> tasks;
	try {
		if (json->hasList("cmds")) {
			cb::JSON::Value& cmds = json->getList("cmds");
			for (unsigned i = 0; i < cmds.size(); i++) {
				auto task2 = executeCmds(cmds.get(i));
				tasks.append(task2);
			}
		}
		else {
			std::string action = json->getAsString("action");
			ModbusTask* task = NULL;
			if (action == "write") {
				uint quantity = json->getS32("quantity");
				uint address = json->getS32("address");
				std::string data = json->getAsString("data");

				std::function<void(int, ModbusTask*, ModbusAdapter*)> function1 = [this](int ret, ModbusTask* task, ModbusAdapter* adapter) {
					if (ret == -1)
					{
						// 暂时还是去掉，因为某些写命令重要性不大
						//QMetaObject::invokeMethod(this, [this]() {
						//	this->StopRun();
						//}, Qt::QueuedConnection);
					}
				};
				task = m_modbus->getTaskWrite(address, quantity, data);
				m_modbusTaskCache.addTask(task, m_currentTaskPriority);
				if (m_currentTaskPriority <= 1)
					LOG_INFO(8, "NCMachine-Modbus: addTaskWrite(" << m_currentTaskPriority << ", " << m_modbus->getTaskCnt(m_currentTaskPriority) << "): " << address << ", " << quantity);
			}
			else if (action == "read") {
				int quantity = json->getS32("quantity");
				uint address = json->getS32("address");
				task = m_modbus->getTaskRead(address, quantity);
				m_modbusTaskCache.addTask(task, m_currentTaskPriority);
				if (m_currentTaskPriority <= 1)
					LOG_INFO(8, "NCMachine-Modbus: addTaskRead(" << m_currentTaskPriority << ", " << m_modbus->getTaskCnt(m_currentTaskPriority) << "):" << address << ", " << quantity);
			} else if (action == "write_file") {
				uint quantity = json->getS32("quantity");
				uint address = json->getS32("address");
				std::string data = json->getAsString("data");

				task = new ModbusTask(DEFAULT_MODBUS_SLAVE, MODBUS_FC_WRITE_FILE_RECORD, address, quantity, data);
				m_modbusTaskCache.addTask(task, m_currentTaskPriority);
				if (m_currentTaskPriority <= 1)
					LOG_INFO(8, "NCMachine-Modbus: addTaskWriteFile(" << m_currentTaskPriority << ", " << m_modbus->getTaskCnt(m_currentTaskPriority) << "): " << address << ", " << quantity);
			}
			else if (action == "read_file") {
				int quantity = json->getS32("quantity");
				uint address = json->getS32("address");
				task = new ModbusTask(DEFAULT_MODBUS_SLAVE, MODBUS_FC_READ_FILE_RECORD, address, quantity);
				m_modbusTaskCache.addTask(task, m_currentTaskPriority);
				if (m_currentTaskPriority <= 1)
					LOG_INFO(8, "NCMachine-Modbus: addTaskReadFile(" << m_currentTaskPriority << ", " << m_modbus->getTaskCnt(m_currentTaskPriority) << "):" << address << ", " << quantity);
			}
			if (task) {
				tasks.append(task);
			}
		}
	}
	catch (const std::exception& e) {
		LOG_WARNING("NCMachine: exception: " << e.what() << ", executeCmds" << json->toString());
	}
	catch (...) {
		LOG_ERROR("NCMachine: exception: " << "Unknown error" << ", executeCmds" << json->toString());
	}
	return tasks;
}

QList<ModbusTask*> NCMachine::executeCmdsByAddress(int address, std::vector<uint16_t> v)
{
	std::string s;
	switch (address)
	{
	case TMBS_MAP0_ID_XPOS:
		s = NCCommand::GetWriteString(TMBS_MAP0_ID_XPOS, TMBS_MAP0_ID_XPOS_LEN * POS_AXIS_LEN, v);
		break;
	case TMBS_MAP0_ID_XOFST:
		s = NCCommand::GetWriteString(TMBS_MAP0_ID_XOFST, TMBS_MAP0_ID_XOFST_LEN * POS_PAD_AXIS_LEN, v);
		break;
	case TMBS_MAP0_ID_MCUP:
		s = NCCommand::GetWriteString(TMBS_MAP0_ID_MCUP, TMBS_MAP0_ID_MCUP_LEN, v);
		break;
	case TMBS_MAP0_ID_MCUSW:
		s = NCCommand::GetWriteString(TMBS_MAP0_ID_MCUSW, TMBS_MAP0_ID_MCUSW_LEN, v);
		break;
	case TMBS_MAP0_ID_HCTRL:
		s = NCCommand::GetWriteString(TMBS_MAP0_ID_HCTRL, TMBS_MAP0_ID_HCTRL_LEN + TMBS_MAP0_ID_HSPD_LEN, v);
		break;
	case TMBS_MAP0_ID_XCPSPOPT:
		s = NCCommand::GetWriteString(TMBS_MAP0_ID_XCPSPOPT, TMBS_MAP0_ID_XCPSPOPT_LEN * LJBC_AXIS_LEN, v);
		break;
	case TMBS_MAP0_ID_XGAP:
		s = NCCommand::GetWriteString(TMBS_MAP0_ID_XGAP, TMBS_MAP0_ID_XGAP_LEN * LJBC_AXIS_LEN, v);
		break;
	case TMBS_MAP0_ID_XGAPMAXPT:
		s = NCCommand::GetWriteString(TMBS_MAP0_ID_XGAPMAXPT, TMBS_MAP0_ID_XGAPMAXPT_LEN * LJBC_AXIS_LEN, v);
		break;
	case TMBS_MAP0_ID_CPSPOPTCFG:
		s = NCCommand::GetWriteString(TMBS_MAP0_ID_CPSPOPTCFG, TMBS_MAP0_ID_CPSPOPTCFG_LEN, v);
		break;
	case TMBS_MAP0_ID_CPSPOPTCFG_DISABLE:
		s = NCCommand::GetWriteString(TMBS_MAP0_ID_CPSPOPTCFG_DISABLE, TMBS_MAP0_ID_CPSPOPTCFG_LEN, v);
		break;
	case TMBS_MAP0_ID_BJGJSJ:
		s = NCCommand::GetWriteString(TMBS_MAP0_ID_BJGJSJ, TMBS_MAP0_ID_BJGJSJ_LEN, v);
		break;
	case TMBS_MAP0_ID_DJSNKZQ:
		s = NCCommand::GetWriteString(TMBS_MAP0_ID_DJSNKZQ, TMBS_MAP0_ID_DJSNKZQ_LEN, v);
		break;
	case TMBS_MAP0_ID_MCUCTRL:
		s = NCCommand::GetWriteString(TMBS_MAP0_ID_MCUCTRL, TMBS_MAP0_ID_MCUCTRL_LEN, v);
		break;
	case TMBS_MAP0_ID_FMQ:
		s = NCCommand::GetWriteString(TMBS_MAP0_ID_FMQ, TMBS_MAP0_ID_FMQ_LEN, v);
		break;
	default:
		s = NCCommand::GetWriteString(address, v.size(), v);
		break;
		//assert(false);
	}
	return this->executeCmds(s);
}

QList<ModbusTask*> NCMachine::executeCmdsByTncOrder(int tncOrder, std::vector<uint16_t> v)
{
	QList<ModbusTask*> tasks;
	// if run in task queue, but the state is current state
	/*if (true || m_stateDirty || m_state[2] != NCT_STATE_API_RUN) {
		GoApi();
	}*/
	switch (tncOrder)
	{
	case TNC_ORDER0_JOG:
	{
		std::vector<uint16_t> v1(v.begin(), v.begin() + 10);
		//auto s1 = NCMachine::UIntsToString(v1);
		//std::string s0 = R"({
		//    "action": "write",
		//    "address" : 8,
		//    "quantity" : 10,
		//    "data" : ")";
		//std::ostringstream stringStream;
		//stringStream << s0 << s1 << "\"}";

		////std::string s = EUtils::string_format(s0, s2);
		//std::string s = stringStream.str();

		auto s1 = NCCommand::GetWriteString(TMBS_MAP0_ID_NCTCTR, TMBS_MAP0_ID_NCTCTR_LEN + TMBS_MAP0_ID_JOGCFG_LEN + TMBS_MAP0_ID_XJOGSPD_LEN * 4, v1);
		tasks.append(this->executeCmds(s1));
		auto v2 = std::vector<uint16_t>{ v[10] };
		// unsinged, -1 become a large value
		if (v[10] <= 1) {
			auto s2 = NCCommand::GetWriteString(TMBS_MAP0_ID_ORD, 1, v2);
			tasks.append(this->executeCmds(s2));
		}
	}
	break;
	case TNC_ORDER0_LOC:
	{
		//auto s2 = NCMachine::UIntsToString(v);
	//std::string s0 = R"({
	//    "action": "write",
	//    "address" : 15,
	//    "quantity" : 14,
	//    "data" : ")";
	//std::ostringstream stringStream;
	//stringStream << s0 << s2 << "\"}";

	////std::string s = EUtils::string_format(s0, s2);
	//std::string s = stringStream.str();

		auto s = NCCommand::GetWriteString(TMBS_MAP0_ID_ORD, 6 + 2 * LOC_AXIS_LEN, v);
		tasks.append(this->executeCmds(s));
	}
	break;
	case TNC_ORDER0_TOUCH:
	{
		auto s1 = NCCommand::GetWriteString(TMBS_MAP0_ID_CMVCFG, TMBS_MAP0_ID_CMVCFG_LEN, { v[0], v[1], v[2], v[3] });
		auto s2 = NCCommand::GetWriteString(TMBS_MAP0_ID_ORD, 4, { v[4], v[5], v[6], v[7] });

		tasks.append(this->executeCmds(s1));
		tasks.append(this->executeCmds(s2));
	}
	break;
	case TNC_ORDER0_TOLMT:
	{
		auto s1 = NCCommand::GetWriteString(TMBS_MAP0_ID_CMVCFG, TMBS_MAP0_ID_CMVCFG_LEN, { v[0], v[1], v[2], v[3] });
		auto s2 = NCCommand::GetWriteString(TMBS_MAP0_ID_ORD, 4, { v[4], v[5], v[6], v[7] });

		tasks.append(this->executeCmds(s1));
		tasks.append(this->executeCmds(s2));
	}
	break;
	case TNC_ORDER0_FRMLMT:
	{
		auto s1 = NCCommand::GetWriteString(TMBS_MAP0_ID_CMVCFG, TMBS_MAP0_ID_CMVCFG_LEN, { v[0], v[1], v[2], v[3] });
		auto s2 = NCCommand::GetWriteString(TMBS_MAP0_ID_ORD, 4, { v[4], v[5], v[6], v[7] });

		tasks.append(this->executeCmds(s1));
		tasks.append(this->executeCmds(s2));
	}
	break;
	case TNC_ORDER0_TOZSIG:
	{
		auto s1 = NCCommand::GetWriteString(TMBS_MAP0_ID_CMVCFG, TMBS_MAP0_ID_CMVCFG_LEN, { v[0], v[1], v[2], v[3] });
		auto s2 = NCCommand::GetWriteString(TMBS_MAP0_ID_ORD, 4, { v[4], v[5], v[6], v[7] });

		tasks.append(this->executeCmds(s1));
		tasks.append(this->executeCmds(s2));
	}
	break;
	case TNC_ORDER0_SPK:
	{
		auto s = NCCommand::GetWriteString(TMBS_MAP0_ID_ORD, 4 + 2 * SPK_AXIS_LEN, v);
		tasks.append(this->executeCmds(s));
	}
	break;
	default:
		assert(false);
	}
	return tasks;
}

ModbusTask* NCMachine::executeCmdWait(std::function<void(int, ModbusTask*, ModbusAdapter*)> func, std::string desc)
{
	ReadAllPosAndState();
	ModbusTask* task = m_modbus->getTaskWait(func, desc);
	m_modbusTaskCache.addTask(task, m_currentTaskPriority);
	if (m_currentTaskPriority <= 1)
		LOG_INFO(8, "NCMachine-Modbus: addTaskWait(" << m_currentTaskPriority << ", " << m_modbus->getTaskCnt(m_currentTaskPriority) << "): " << desc << ", " << func.target_type().name());
	return task;
}

/*
* NCTSTATE 状态	定义编码	状态说明
NCT_STATE_API_RUN	0	无移动状态（API）
NCT_STATE_JOG_RUN	0 + 256	JOG运行
NCT_STATE_JOG_EXIT	1 + 256	JOG退出，退出后返回到NCT_STATE_API_RUN
		
NCT_STATE_LOC_RUN	0 + 512	LOC运行
		
NCT_STATE_CMV_RUN	0 + 768	CMV运行
NCT_STATE_CMV_EXIT	1 + 768	CMV退出状态，退出后返回到NCT_STATE_API_RUN
NCT_STATE_CMV_FINISHED	2 + 768	CMV完成，退出后返回到NCT_STATE_API_RUN

*/
std::function<int()> NCMachine::waitno(std::function<void()> doAfter)
{
	std::function<int()> func = [doAfter]() {
		if (doAfter != NULL) {
			doAfter();
		}
		return 1;
	};
	return func;
}

std::function<int()> NCMachine::waitUntilNctState(uint16_t state)
{
	std::function<int()> func = [this, state]() {
		//if (m_g01Data.isRunning && m_g01Data.isPausing)
		//	return -2;

		if (m_state[2] == state) {
			return 1;
		}
		else {
			// 检测API状态的时候，只能检测m_state[2], 因为m_state[0] = 0是中间态
			if (state == 0) {
				return -2;
			}

			if ((m_state[0] & 0xFF00) == (state & 0xFF00)) {		// 同一个类型的指令执行结果数据
				// 正常
				uint16_t para = 0;
				uint16_t rslt = state;
				if ((m_state[0] >= rslt && m_state[0] <= rslt + 2) && (m_state[1] == para || para == 0)) {
					LOG_WARNING("NCMachine: Function waitUntilNctState(" << state << ") return 1, but not by nctState, it's by rslt. Now state is " << m_state[0] << ", " << m_state[1]);
					return 1;		// 完成任务
				}
				else {
					LOG_WARNING("NCMachine: Function waitUntilNctState(" << state << ") return -9. Now state is " << m_state[0] << ", " << m_state[1]);
					//if (doAfterFail != NULL) {
					//	doAfterFail();
					//}
					QString error = QStringLiteral("ERROR:%1").arg(NCMachine::GetRLSTDesc(m_state[0], m_state[1])); // .arg(gcode.replace("\n", ""));
					LineLogger::instance().append(error);

					enterSetPriority(0);
					Beep();
					exitSetPriority();

					if (doWhenWaitFail != NULL) {
						doWhenWaitFail();
						doWhenWaitFail = NULL;
					}

					return -9;		// 清空Modbus任务队列
				}
			}
			else 
			{
				return -2;
			}

			//return -2;
		}
	};
	return func;
}
std::function<void(int, ModbusTask*, ModbusAdapter*)> NCMachine::convertWaitFunction(std::function<int()> func2, int timeout)
{
	std::function<void(int, ModbusTask*, ModbusAdapter*)> func = [func2, timeout](int ret, ModbusTask* task, ModbusAdapter* adapter) {
		int r = func2();
		if (task != NULL) {
			if (r == 1) {
				task->startAddr = 1;
			}
			else if (r == -9) {
				task->startAddr = -9;
			}
			else {
				task->startAddr = -2;
				task->numOfRegs++;
				if (timeout != -1 && task->numOfRegs > timeout) {
					task->startAddr = -9;
				}
			}
		}
		else {
			int cnt = 0;
			while (true)
			{
				r = func2();
				if (r == 1 || r == -9) {
					break;
				}
				else {
					std::this_thread::sleep_for(std::chrono::milliseconds(TASK_WAIT_MILLSECONDS));
					cnt++;
				}
				if (timeout != -1 && cnt >= timeout) {
					break;
				}
			}
		}
		};
	return func;
}

//std::function<void(int, ModbusTask*, ModbusAdapter*)> NCMachine::waitUntilNctState(uint16_t state, int timeout)
//{
//	std::function<void(int, ModbusTask*, ModbusAdapter*)> func = [this, state, timeout](int ret, ModbusTask* task, ModbusAdapter* adapter) {
//		if (task != NULL) {
//			if (m_state[2] == state) {
//				task->startAddr = 1;
//			}
//			else {
//				task->startAddr = -2;
//				task->numOfRegs++;
//				if (timeout != -1 && task->numOfRegs > timeout) {
//					task->startAddr = -9;
//				}
//			}
//		}
//		else {
//			int cnt = 0;
//			while (true)
//			{
//				if (m_state[2] == state) {
//					break;
//				}
//				else {
//					EUtils::sleep(TIMEOUT_WAIT_MILLSECONDS);
//					cnt++;
//				}
//				if (timeout != -1 && cnt >= timeout) {
//					break;
//				}
//			}
//		}
//	};
//	return func;
//}

/*
RLST.u16Rslt 编码	RLST.u16Para 	结果说明	运行模式
( 2u )   + ( 0 )	( 1<<15 )	手动退出	JOG

( 1u )   + ( 512 )	( 0u )	正常完成退出	LOC
( 2u )   + ( 512 )	( 1<<15 )	手动退出	LOC
( 3u )   + ( 512 )	驱动器报警编号	驱动器报警退出	LOC
( 4u )   + ( 512 )	限位状态数据	触发限位退出	LOC
( 5u )   + ( 512 )	( 1<<15 )	短路退出	LOC
( 61u )  + ( 512 )	( 0u )	参数非法	LOC
( 62u )  + ( 512 )	( 0u )	校验错误	LOC
( 51u )  + ( 512 )	( 0u )	无效功能	LOC

( 1u )   + ( 768 )	( 0u )	正常完成退出	CMV
( 2u )   + ( 768 )	( 1<<15 )	手动退出	CMV
( 3u )   + ( 768 )	驱动器报警编号	驱动器报警退出	CMV
( 4u )   + ( 768 )	限位状态数据	触发限位退出	CMV
( 5u )   + ( 768 )	( 1<<15 )	短路退出	CMV
( 53u )  + ( 768 )	( 0u )	轴编码错误	CMV
( 54u )  + ( 768 )	( 0u )	操作码错误	CMV
( 51u )  + ( 768 )	( 0u )	无效功能	CMV

( 1u )  + ( 1024 )	( 0u )	正常完成退出	SPK
( 2u )  + ( 1024 )	( 1<<15 )	手动退出	SPK
( 3u )  + ( 1024 )	0xFFFF	驱动器报警退出	SPK
( 4u )  + ( 1024 )	D0~15：XL+;XL-;YL+;YL-
ZL+;Zl-…
置1：限位触发	触发限位退出	SPK
( 5u )  + ( 1024 )	( 0u )  	短路退出	SPK
( 53u ) + ( 1024 )	( 0u )	轴编码错误	SPK
( 54u ) + ( 1024 )	( 0u )	操作码错误	SPK
( 51u ) + ( 1024 )	( 0u )	无效功能	SPK

*/
std::function<int()> NCMachine::waitUntilRLST(uint16_t rslt, uint16_t para)
{
	std::function<int()> func = [this, rslt, para]() {
		if (m_g01Data.isRunning && m_g01Data.isPausing && rslt == NCT_STATE_SPK_EXTOC)
			return -2;
		// 调试时候用
		//if (m_state[0] == 768 + 5)
		//	m_state[0] = 768 + 1;

		if ((m_state[0] & 0xFF00) == (rslt & 0xFF00)) {		// 同一个类型的指令执行结果数据
			// 正常完成退出 or 手动退出 // 
			if ((m_state[0] >= rslt && m_state[0] <= rslt + 1) && (m_state[1] == para || para == 0)) {
				return 1;		// 完成任务
			}
			else {
				//// 油位报警
				//if (m_state[0] == 6 + 1024 && m_state[1] == 2) {
				//	//auto func = convertWaitFunction([this]() {
				//	//	if (this->GetInputFlag(9))
				//	//		return -2;
				//	//	else
				//	//		return 1;
				//	//	});
				//	//auto desc = "wait for YOUWEI_FLAG";
				//	//ModbusTask* task = m_modbus->getTaskWait(func, desc);
				//	//m_modbus->addTask(task, m_currentTaskPriority);

				//	enterSetPriority(1);

				//	executeCmdWait(convertWaitFunction([this]() {
				//		if (this->GetInputFlag(9))
				//			return -2;
				//		else
				//			return 1;
				//		}), "wait for YOUWEI_FLAG");
				//	SPKPropertyObject* spkP = PropertyObjects::getInstance()->propertyObjectSPK;
				//	spkP->setzgj(3);
				//	auto v = spkP->GetValues();
				//	this->executeCmdsByTncOrder(TNC_ORDER0_SPK, v);

				//	executeCmdWait(convertWaitFunction(waitUntilNctState(NCT_STATE_SPK_RUN)), "wait for SPK_RUN");

				//	executeCmdWait(convertWaitFunction(waitUntilRLST(NCT_STATE_SPK_EXTOC, 0)), "wait for SPK_EXTOC");

				//	executeCmdWait(m_functionWaitUntilApirun, "wait for API_RUN");


				//	exitSetPriority();
				//	return 1;
				//}

				LOG_WARNING("NCMachine: Function waitUntilRLST(" << rslt << ", " << para << ") return -9. Now state is " << m_state[0] << ", " << m_state[1]);
				QString error = QStringLiteral("ERROR:%1").arg(NCMachine::GetRLSTDesc(m_state[0], m_state[1])); // .arg(gcode.replace("\n", ""));
				LineLogger::instance().append(error);

				enterSetPriority(0);
				Beep();
				exitSetPriority();

				if (doWhenWaitFail != NULL) {
					doWhenWaitFail();
					doWhenWaitFail = NULL;
				}

				return -9;		// 清空Modbus任务队列
			}
		}
		else {
			return -2;
		}
	};
	return func;
}

//std::function<void(int, ModbusTask*, ModbusAdapter*)> NCMachine::waitUntilRLST(uint16_t rslt, uint16_t para, int timeout)
//{
//	std::function<void(int, ModbusTask*, ModbusAdapter*)> func = [this, rslt, para, timeout](int ret, ModbusTask* task, ModbusAdapter* adapter) {
//		if (task != NULL) {
//			if ((m_state[0] & 0xFF00) == (rslt & 0xFF00)) {		// 同一个类型的指令执行结果数据
//				if (m_state[0] <= rslt + 5 && m_state[1] == para) {
//					task->startAddr = 1;		// 完成任务
//				}
//				else {
//					task->startAddr = -9;		// 清空Modbus任务队列
//				}
//			}
//			else {
//				task->startAddr = -2;
//				task->numOfRegs++;
//				if (timeout != -1 && task->numOfRegs > timeout) {
//					task->startAddr = -9;		// 超时，清空Modbus任务队列
//				}
//			}
//		}
//		else
//		{
//			int cnt = 0;
//			while (true)
//			{
//				if ((m_state[0] & 0xFF00) == (rslt & 0xFF00) && m_state[1] == para) {
//					break;
//				}
//				else {
//					EUtils::sleep(TIMEOUT_WAIT_MILLSECONDS);
//					cnt++;
//				}
//				if (timeout != -1 && cnt >= timeout) {
//					return;
//				}
//			}
//		}
//	};
//	return func;
//}



void NCMachine::SetPosOne(int axis, int value)
{
	std::string s;
	std::vector<uint16_t> v;
	v.push_back(value & 0xFFFF);
	v.push_back(value >> 16);

	switch (axis) {
	case TMBS_MAP0_ID_XPOS:
		s = NCCommand::GetWriteString(TMBS_MAP0_ID_XPOS, TMBS_MAP0_ID_XPOS_LEN, v);
		break;
	case TMBS_MAP0_ID_YPOS:
		s = NCCommand::GetWriteString(TMBS_MAP0_ID_YPOS, TMBS_MAP0_ID_YPOS_LEN, v);
		break;
	case TMBS_MAP0_ID_ZPOS:
		s = NCCommand::GetWriteString(TMBS_MAP0_ID_ZPOS, TMBS_MAP0_ID_ZPOS_LEN, v);
		break;
	case TMBS_MAP0_ID_UPOS:
		s = NCCommand::GetWriteString(TMBS_MAP0_ID_UPOS, TMBS_MAP0_ID_UPOS_LEN, v);
		break;
	default:
		assert(false);
		return;
	}
	this->executeCmds(s);
}

// 多个坐标轴挨个进行移动，n个命令
/***************
* 直接多个命令Loc(TMBS_MAP0_ID_XPOS, m_g01Data.lastPos.x());
			Loc(TMBS_MAP0_ID_YPOS, m_g01Data.lastPos.y());
			Loc(TMBS_MAP0_ID_ZPOS, m_g01Data.lastPos.z());
  这样会出错，因为发送第二个Loc的时候，x坐标是原来的老坐标，会使得x又回到原来位置
* 多个Loc不能连续调用
****************/
void NCMachine::LocSeq(int* axis, int* value, int size)
{
	for (int i = 0; i < size; ++i)
	{
		if (m_pos[i] != value[i]) {
			// i=1时，虽然设置了2个轴，但是其实只是移动1个轴，另外一个在上一步就移动好了
			Loc(axis, value, i + 1);
		}
	}
}

// 多个坐标轴同时进行移动，一个命令
void NCMachine::Loc(int* axis, int* value, int size)
{
	if (size == 1)
		LOG_INFO(8, "NCMachine: Loc " << axis[0] << "(" << value[0] << ")");
	else if (size == 2)
		LOG_INFO(8, "NCMachine: Loc " << axis[0] << "(" << value[0] << ")," << axis[1] << "(" << value[1] << ")");
	else if (size == 3)
		LOG_INFO(8, "NCMachine: Loc " << axis[0] << "(" << value[0] << ")," << axis[1] << "(" << value[1] << ")," << axis[2] << "(" << value[2] << ")");
	else if (size == 4)
		LOG_INFO(8, "NCMachine: Loc " << axis[0] << "(" << value[0] << ")," << axis[1] << "(" << value[1] << ")," << axis[2] << "(" << value[2] << ")," << axis[3] << "(" << value[3] << ")");
	else {
		LOG_WARNING("NCMachine: Loc with more than 4 axis!");
		return;
	}

	LocPropertyObject* locP = PropertyObjects::getInstance()->propertyObjectLoc;
	locP->setx(m_pos[0]);
	locP->sety(m_pos[1]);
	locP->setz(m_pos[2]);
	locP->setu(m_pos[3]);

	bool newPos = false;
	for (int i = 0; i < size; ++i)
	{
		switch (axis[i])
		{
		case TMBS_MAP0_ID_XPOS:
			if (locP->x() != value[i]) {
				locP->setx(value[i]);
				newPos = true;
			}
			break;
		case TMBS_MAP0_ID_YPOS:
			if (locP->y() != value[i]) {
				locP->sety(value[i]);
				newPos = true;
			}
			break;
		case TMBS_MAP0_ID_ZPOS:
			if (locP->z() != value[i]) {
				locP->setz(value[i]);
				newPos = true;
			}
			break;
		case TMBS_MAP0_ID_UPOS:
			if (locP->u() != value[i]) {
				locP->setu(value[i]);
				newPos = true;
			}
			break;
		}
	}

	if (newPos) {
		executeCmdWait(m_functionWaitUntilApirun, "wait for API_RUN");

		auto v = locP->GetValues();
		this->executeCmdsByTncOrder(TNC_ORDER0_LOC, v);

		executeCmdWait(convertWaitFunction(waitUntilNctState(NCT_STATE_LOC_RUN)), "wait for LOC_RUN");
		executeCmdWait(convertWaitFunction(waitUntilRLST(NCT_STATE_LOC_EXIT)), "wait for LOC_EXIT");
		executeCmdWait(m_functionWaitUntilApirun, "wait for API_RUN");
	}
}
void NCMachine::Loc(int axis, int value)
{
	int axis2[1] = { axis };
	int value2[1] = { value };
	Loc(axis2, value2, 1);
}

void NCMachine::Spk(int* axis, int* value, int size, int zgj)
{
	if (size == 0) {
		LOG_WARNING("NCMachine: Spk with 0 axis!");
		return;
	}
	else if (size == 1)
		LOG_INFO(8, "NCMachine: Spk " << zgj << ", " << axis[0] << "(" << value[0] << ")");
	else if (size == 2)
		LOG_INFO(8, "NCMachine: Spk " << zgj << ", " << axis[0] << "(" << value[0] << ")," << axis[1] << "(" << value[1] << ")");
	else if (size == 3)
		LOG_INFO(8, "NCMachine: Spk " << zgj << ", " << axis[0] << "(" << value[0] << ")," << axis[1] << "(" << value[1] << ")," << axis[2] << "(" << value[2] << ")");
	else if (size == 4)
		LOG_INFO(8, "NCMachine: Spk " << zgj << ", " << axis[0] << "(" << value[0] << ")," << axis[1] << "(" << value[1] << ")," << axis[2] << "(" << value[2] << ")," << axis[3] << "(" << value[3] << ")");
	else {
		LOG_WARNING("NCMachine: Spk with more than 4 axis!");
		return;
	}

	//if (size >= 2) {
	//	THROW(EUtils::QString2StdString(QStringLiteral("目前不支持多于1个坐标轴加工")));
	//}

	SPKPropertyObject* spkP = PropertyObjects::getInstance()->propertyObjectSPK;
	spkP->setx(m_pos[0]);
	spkP->sety(m_pos[1]);
	spkP->setz(m_pos[2]);
	spkP->setu(m_pos[3]);
	spkP->setzgj(zgj);

	for (int i = 0; i < size; ++i)
	{
		switch (axis[i])
		{
		case TMBS_MAP0_ID_XPOS:
			if (spkP->x() != value[i]) {
				spkP->setx(value[i]);
			}
			break;
		case TMBS_MAP0_ID_YPOS:
			if (spkP->y() != value[i]) {
				spkP->sety(value[i]);
			}
			break;
		case TMBS_MAP0_ID_ZPOS:
			if (spkP->z() != value[i]) {
				spkP->setz(value[i]);
			}
			break;
		case TMBS_MAP0_ID_UPOS:
			if (spkP->u() != value[i]) {
				spkP->setu(value[i]);
			}
			break;
		}
	}

	executeCmdWait(m_functionWaitUntilApirun, "wait for API_RUN");
	executeCmdWait(convertWaitFunction([this]() {
		if (this->GetInputFlag(9))
			return -2;
		else
			return 1;
		}), "wait for YOUWEI_FLAG");

	auto v = spkP->GetValues();
	this->executeCmdsByTncOrder(TNC_ORDER0_SPK, v);

	executeCmdWait(convertWaitFunction(waitUntilNctState(NCT_STATE_SPK_RUN)), "wait for SPK_RUN");

	executeCmdWait(convertWaitFunction(waitUntilRLST(NCT_STATE_SPK_EXTOC, 0)), "wait for SPK_EXTOC");

	executeCmdWait(m_functionWaitUntilApirun, "wait for API_RUN");
}

void NCMachine::Spk(int axis, int value)
{
	int axis2[1] = { axis };
	int value2[1] = { value };
	Spk(axis2, value2, 1);
}

void NCMachine::Beep(int n, int ms)
{
	DataForm* dataForm = DataForms::getInstance()->getDataForm("xitongshezhi5");
	if (dataForm->getValue("FMQKG") == "0")
		return;

	if (ms == 0) {
		PropertyObjects::getInstance()->propertyObjectFengMingQi->setmjfs(FmqPropertyObject::MjfsEnum::TYPE_OFF);
		PropertyObjects::getInstance()->propertyObjectFengMingQi->ExecuteCmds(this);
	}
	else if (ms == -1) {
		PropertyObjects::getInstance()->propertyObjectFengMingQi->setmjfs(FmqPropertyObject::MjfsEnum::TYPE2);
		PropertyObjects::getInstance()->propertyObjectFengMingQi->ExecuteCmds(this);
	}
	else {
		PropertyObjects::getInstance()->propertyObjectFengMingQi->setmjfs(FmqPropertyObject::MjfsEnum::TYPE1);
		PropertyObjects::getInstance()->propertyObjectFengMingQi->setdata2(ms);
		for (int i = 0; i < n; ++i) {
			PropertyObjects::getInstance()->propertyObjectFengMingQi->ExecuteCmds(this);
			std::function<void()> doFuncWait = []() {
				EUtils::sleep(80);
				};
			executeCmdWait(convertWaitFunction(waitno(doFuncWait)), "wait 80ms for beep");
		}
	}
}

void NCMachine::Touch(DdfxEnum ddfx)
{
	LOG_INFO(8, "NCMachine: Touch " << ddfx);

	auto pCmvTouch = PropertyObjects::getInstance()->propertyObjectCmvTouch;
	pCmvTouch->setddfx(ddfx);

	executeCmdWait(m_functionWaitUntilApirun, "wait for API_RUN");

	pCmvTouch->ExecuteCmds(this);

	//executeCmdWait(convertWaitFunction(waitUntilNctState(NCT_STATE_CMV_RUN)), "wait for CMV_RUN");
	executeCmdWait(convertWaitFunction(waitUntilRLST(NCT_STATE_CMV_EXIT)), "wait for CMV_EXIT in Touch");
	executeCmdWait(m_functionWaitUntilApirun, "wait for API_RUN");
}

// 按照系统设置的坐标来移动，用于"限位移动"
void NCMachine::ToLMTSoft(DdfxEnum ddfx)
{
	LOG_INFO(8, "NCMachine: ToLMTSoft " << ddfx);

	DataForm* dataForm = DataForms::getInstance()->getDataForm("xitongshezhi");
	if (ddfx == DdfxEnum::X_PLUS) {
		QString s = dataForm->getValue("JXYD_X+");
		Loc(TMBS_MAP0_ID_XPOS, s.toInt());
	}
	else if (ddfx == DdfxEnum::X_MINUS) {
		QString s = dataForm->getValue("JXYD_X-");
		Loc(TMBS_MAP0_ID_XPOS, s.toInt());
	}
	else if (ddfx == DdfxEnum::Y_PLUS) {
		QString s = dataForm->getValue("JXYD_Y+");
		Loc(TMBS_MAP0_ID_YPOS, s.toInt());
	}
	else if (ddfx == DdfxEnum::Y_MINUS) {
		QString s = dataForm->getValue("JXYD_Y-");
		Loc(TMBS_MAP0_ID_YPOS, s.toInt());
	}
	else if (ddfx == DdfxEnum::Z_PLUS) {
		QString s = dataForm->getValue("JXYD_Z+");
		Loc(TMBS_MAP0_ID_ZPOS, s.toInt());
	}
	else if (ddfx == DdfxEnum::Z_MINUS) {
		QString s = dataForm->getValue("JXYD_Z-");
		Loc(TMBS_MAP0_ID_ZPOS, s.toInt());
	}
	else {
		assert(false);
	}

	
}
void NCMachine::ToLMT(DdfxEnum ddfx)
{
	LOG_INFO(8, "NCMachine: ToLMT " << ddfx);

	auto pCmvTouch = PropertyObjects::getInstance()->propertyObjectCmvToLMT;
	pCmvTouch->setddfx(ddfx);

	executeCmdWait(m_functionWaitUntilApirun, "wait for API_RUN");
	pCmvTouch->ExecuteCmds(this);
	
	//executeCmdWait(convertWaitFunction(waitUntilNctState(NCT_STATE_CMV_RUN)), "wait for CMV_RUN");
	executeCmdWait(convertWaitFunction(waitUntilRLST(NCT_STATE_CMV_EXIT)), "wait for CMV_EXIT in ToLMT");
	executeCmdWait(m_functionWaitUntilApirun, "wait for API_RUN");
}

void NCMachine::FromLMT(DdfxEnum ddfx)
{
	LOG_INFO(8, "NCMachine: FromLMT " << ddfx);

	auto pCmvTouch = PropertyObjects::getInstance()->propertyObjectCmvFRMLMT;
	pCmvTouch->setddfx(ddfx);
	
	executeCmdWait(m_functionWaitUntilApirun, "wait for API_RUN");
	pCmvTouch->ExecuteCmds(this);

	// 如果限位，不会进入CMV_RUN状态, comment it
	//executeCmdWait(convertWaitFunction(waitUntilNctState(NCT_STATE_CMV_RUN)), "wait for CMV_RUN");
	executeCmdWait(convertWaitFunction(waitUntilRLST(NCT_STATE_CMV_EXIT)), "wait for CMV_EXIT in FromLMT");
	executeCmdWait(m_functionWaitUntilApirun, "wait for API_RUN");
}

void NCMachine::ToZSig(DdfxEnum ddfx)
{
	LOG_INFO(8, "NCMachine: ToZSig " << ddfx);

	auto pCmvTouch = PropertyObjects::getInstance()->propertyObjectCmvToZSig;
	pCmvTouch->setddfx(ddfx);

	executeCmdWait(m_functionWaitUntilApirun, "wait for API_RUN");
	pCmvTouch->ExecuteCmds(this);

	//executeCmdWait(convertWaitFunction(waitUntilNctState(NCT_STATE_CMV_RUN)), "wait for CMV_RUN");
	executeCmdWait(convertWaitFunction(waitUntilRLST(NCT_STATE_CMV_EXIT)), "wait for CMV_EXIT in ToZSig");
	executeCmdWait(m_functionWaitUntilApirun, "wait for API_RUN");
}

void NCMachine::PowerOn()
{
	LOG_INFO(5, "EDM: POWER ON");

	enterSetPriority(0);

	PropertyObjects::getInstance()->propertyObjectFanddianIO->ExecuteCmds(this);
	EUtils::sleep(2000);
	PropertyObjects::getInstance()->propertyObjectDianJiShiNeng->ExecuteCmds(this);

	PropertyObjects::getInstance()->propertyObjectShouKongHe->ExecuteCmds(this);

	this->GoJog();
	//PropertyObjects::getInstance()->propertyObjectJog->ExecuteCmds(this);

	exitSetPriority();

	ProcessPos();
}

void NCMachine::PowerOff()
{
	LOG_INFO(5, "EDM: POWER OFF");

	enterSetPriority(0);
	this->executeCmds(NCCommand::Shutdown);
	//this->GoApi();
	exitSetPriority();

	// for task done
	EUtils::sleep(2000);
}

void NCMachine::BacktoZeroSimple(int axis)
{
	this->SetPosOne(TMBS_MAP0_ID_XPOS + axis, 0);

	GetController()->setAxisAbsolutePosition(GCodeTool::axis[axis], 0, GCode::Units::METRIC);
}

void NCMachine::BacktoZero(DdfxEnum ddfx)
{
	// int axis, bool axisPositive
	int axis = 0;
	bool zeroAsix = false;
	bool enableLjbc = false;

	LOG_INFO(5, "EDM: BACK TO ZERO");

	// For Test
	//enterSetPriority(1);
	//GoApi();
	//Touch(DdfxEnum::X_PLUS);
	//executeCmdWait(convertWaitFunction(waitUntilRLST(NCT_STATE_CMV_EXIT)), "wait for CMV_EXIT");
	//exitSetPriority();
	//return;

	std::function<void()> doFunc1 = [this]() {
		m_nowOperating = 2;
		};
	executeCmdWait(convertWaitFunction(waitno(doFunc1)), "start back_to_zero");

#ifdef BACKZERO_METHOD2
	m_isBackzeroing = 1;
	if (zeroAsix) {
		m_isZeroAsix = axis;
	}
	else
	{
		m_isZeroAsix = -1;
	}
#else
	GoApi();

	switch (ddfx)
	{
	case DdfxEnum::X_PLUS:
		ToLMT(DdfxEnum::X_PLUS);
		FromLMT(DdfxEnum::X_MINUS);
		ToZSig(DdfxEnum::X_MINUS);
		axis = 0;
		break;
	case DdfxEnum::X_MINUS:
		ToLMT(DdfxEnum::X_MINUS);
		FromLMT(DdfxEnum::X_PLUS);
		ToZSig(DdfxEnum::X_PLUS);
		axis = 0;
		break;
	case DdfxEnum::Y_PLUS:
		ToLMT(DdfxEnum::Y_PLUS);
		FromLMT(DdfxEnum::Y_MINUS);
		ToZSig(DdfxEnum::Y_MINUS);
		axis = 1;
		break;
	case DdfxEnum::Y_MINUS:
		ToLMT(DdfxEnum::Y_MINUS);
		FromLMT(DdfxEnum::Y_PLUS);
		ToZSig(DdfxEnum::Y_PLUS);
		axis = 1;
		break;
	case DdfxEnum::Z_PLUS:
		ToLMT(DdfxEnum::Z_PLUS);
		FromLMT(DdfxEnum::Z_MINUS);
		ToZSig(DdfxEnum::Z_MINUS);
		axis = 2;
		break;
	case DdfxEnum::Z_MINUS:
		ToLMT(DdfxEnum::Z_MINUS);
		FromLMT(DdfxEnum::Z_PLUS);
		ToZSig(DdfxEnum::Z_PLUS);
		axis = 2;
		break;
	default:
		assert(false);
		break;
	}

	if (zeroAsix)
	{
		this->SetPosOne(TMBS_MAP0_ID_XPOS + axis, 0);
		//std::function<void(int, ModbusTask*, ModbusAdapter*)> function = [this, axis](int ret, ModbusTask* task, ModbusAdapter* adapter) {
		//	GetController()->setAxisAbsolutePosition(GCodeTool::axis[axis], 0, GCode::Units::METRIC);
		//	task->startAddr = 1;
		//};
		std::function<void()> doFuncZeroAxisInController = [this, axis]() {
			GetController()->setAxisAbsolutePosition(GCodeTool::axis[axis], 0, GCode::Units::METRIC);
			};
		executeCmdWait(convertWaitFunction(waitno(doFuncZeroAxisInController)), "doFuncZeroAxisInController");
		//executeCmdWait(function, "wait for doFuncZeroAxisInController");
		//this->BacktoZeroSimple(axis);


		// 螺距补偿使能
		if (enableLjbc) {
			PropertyObjects::getInstance()->propertyObjectZLJBC[axis]->setbcsx(true);
			std::vector<uint16_t> v;
			uint16_t uint1 = 0;
			uint1 |= axis == 0 ? 0x1 : 0x0;
			uint1 |= axis == 1 ? 0x2 : 0x0;
			uint1 |= axis == 2 ? 0x4 : 0x0;
			v.push_back(uint1);
			if (uint1 > 0) {
				this->executeCmdsByAddress(TMBS_MAP0_ID_CPSPOPTCFG, v);
			}
			//ZLJBCPropertyObject::ExecuteCmds(this, PropertyObjects::getInstance()->propertyObjectZLJBC);
		}
	}
#endif

	std::function<void()> doFunc2 = [this]() {
		m_nowOperating = 0;
		};
	executeCmdWait(convertWaitFunction(waitno(doFunc2)), "end back_to_zero");
}

void NCMachine::PrepareCent()
{
	if (IsGCodeRunning() || m_nowOperating > 0)
		return;

	enterSetPriority(1);
	
	PropertyObjects::getInstance()->propertyObjectFengMingQi->setmjfs(FmqPropertyObject::MjfsEnum::TYPE3);
	PropertyObjects::getInstance()->propertyObjectFengMingQi->setdata3(25);
	PropertyObjects::getInstance()->propertyObjectFengMingQi->setdata4(20);
	PropertyObjects::getInstance()->propertyObjectFengMingQi->ExecuteCmds(this);

	PropertyObjects::getInstance()->propertyObjectFengMingQi->ExecuteCmds(this);

	GoApi();
	exitSetPriority();

	m_nowOperating = 1;
}

void NCMachine::GoApi()
{
	// goApi run in modbus task queue, but the state is current state
	// but in queue is not ok also because we should add command to queue now.
	//std::function<void()> doFunc1 = [this]() {
	//	if (m_state[2] != NCT_STATE_API_RUN)
	//	{
	//		this->executeCmds(NCCommand::DisableAll);
	//		executeCmdWait(m_functionWaitUntilApirun, "wait for API_RUN");
	//	}
	//};
	//executeCmdWait(convertWaitFunction(waitno(doFunc1)), "NCMachine: GoApi");

	LOG_INFO(8, "NCMachine: GoApi");
	ReadAllPosAndState();
	QList<ModbusTask*> tasks = this->executeCmds(NCCommand::DisableAll);
	//ModbusTask* task = m_modbus->addTaskWrite(TMBS_MAP0_ID_EXIT, TMBS_MAP0_ID_EXIT_LEN, NCCommand::UIntsToString({ 0xFF }), m_currentTaskPriority);
	for (ModbusTask* task : tasks)
	{
		task->setPreFunction([this](ModbusTask* task, ModbusAdapter* adapter) {
			return m_state[2] == NCT_STATE_API_RUN ? 0 : 1;
		}, "Check API State");
	}
	executeCmdWait(m_functionWaitUntilApirun, "wait for API_RUN");
}

void NCMachine::GoJog()
{
	LOG_INFO(8, "NCMachine: GoJog");
	ReadAllPosAndState();
	QList<ModbusTask*> tasks = PropertyObjects::getInstance()->propertyObjectJog->ExecuteCmds(this);
	for (ModbusTask* task : tasks)
	{
		task->setPreFunction([this](ModbusTask* task, ModbusAdapter* adapter) {
			return m_state[2] == NCT_STATE_JOG_RUN ? 0 : 1;
		}, "Check Jog State");
	}
	executeCmdWait(convertWaitFunction(waitUntilNctState(NCT_STATE_JOG_RUN)), "wait for JOG_RUN");
}

void NCMachine::ProcessState()
{
	if (HasError())
	{
		//m_modbus->clearTask();
		enterSetPriority(1);
		GoApi();
		GoJog();
		exitSetPriority();
		m_nowOperating = 0;
#ifdef BACKZERO_METHOD2
		m_isBackzeroing = 0;
		m_isZeroAsix = -1;
#endif
	}

#ifdef BACKZERO_METHOD2
	if (m_isBackzeroing > 0) {
		if (this->GetNctState() == NCT_STATE_API_RUN) {
			if (m_isBackzeroing == 1) {
				PropertyObjects::getInstance()->propertyObjectCmvToLMT->ExecuteCmds(this);
				m_isBackzeroing = 2;
			}
			else if (m_isBackzeroing == 2) {
				PropertyObjects::getInstance()->propertyObjectCmvFRMLMT->ExecuteCmds(this);
				m_isBackzeroing = 3;
			}
			else if (m_isBackzeroing == 3) {
				PropertyObjects::getInstance()->propertyObjectCmvToZSig->ExecuteCmds(this);
				m_isBackzeroing = 4;
			}
			else if (m_isBackzeroing == 4) {
				if (m_isZeroAsix >= 0) {
					this->SetPosOne(TMBS_MAP0_ID_XPOS + m_isZeroAsix, 0);
				}
				m_isBackzeroing = 0;
			}

		}
		else if (this->GetNctState() == NCT_STATE_CMV_RUN) {
		}
		else {
		}
	}
#endif // BACKZERO_METHOD2
}

bool NCMachine::HasError()
{
	// 0,1: rlst
	if (m_state[0] == 0 && m_state[1] == 0)
		return false;

	// 2: state
	switch (m_state[2])
	{
	case NCT_STATE_API_RUN:
		break;
	case NCT_STATE_JOG_RUN:
	case NCT_STATE_JOG_EXIT:
		break;
	case NCT_STATE_LOC_RUN:
		return !(m_state[0] == (1 + 512) && m_state[1] == 0);
	case NCT_STATE_CMV_RUN:
	case NCT_STATE_CMV_EXIT:
	case NCT_STATE_CMV_FINISHED:
		return !(m_state[0] == (1 + 768) && m_state[1] == 0);
	case NCT_STATE_SPK_RUN:
	case NCT_STATE_SPK_EXTOC:
	case NCT_STATE_SPK_EXBAK:
	case NCT_STATE_SPK_PMBAK:
	case NCT_STATE_SPK_PMOV:
	case NCT_STATE_SPK_TOSPK:
		break;
	}

	return false;
}

void NCMachine::SetInputKey(unsigned long long key)
{
	m_key = key;
	ProcessKey();
}

void NCMachine::ProcessKey()
{
	//if (m_key == KNLK_NONE)
	//	return;
	LOG_INFO(8, "NCMachine: ProcessKey " << std::hex << "0x" << m_key << std::dec << ", " << m_state[2] << ", " << m_nowOperating);
	if (m_nowOperating == 1) {
		if (m_state[2] == NCT_STATE_API_RUN) {
			QString gcode = "";
			switch (m_key)
			{
			case KNLK_NONE:
				break;
			case KNLK_XSUB:
			{
				QString inBack = DataForms::getInstance()->getDataForm("DuanMianForm", SystemSettings::instance().GetProjectDir())->getValue("inBack");
				//PropertyObjects::getInstance()->propertyObjectCmvTouch->setddfx(DdfxEnum::X_MINUS);
				//PropertyObjects::getInstance()->propertyObjectCmvTouch->ExecuteCmds(this);
				gcode = GCodeUtils::Duidao("X", -1, 0, 0, inBack.toDouble());
			}
				break;
			case KNLK_XADD:
			{
				QString inBack = DataForms::getInstance()->getDataForm("DuanMianForm", SystemSettings::instance().GetProjectDir())->getValue("inBack");
				gcode = GCodeUtils::Duidao("X", 1, 0, 0, inBack.toDouble());
			}
				break;
			case KNLK_YSUB:
			{
				QString inBack = DataForms::getInstance()->getDataForm("DuanMianForm", SystemSettings::instance().GetProjectDir())->getValue("inBack");
				gcode = GCodeUtils::Duidao("Y", -1, 0, 0, inBack.toDouble());
			}
				break;
			case KNLK_YADD:
			{
				QString inBack = DataForms::getInstance()->getDataForm("DuanMianForm", SystemSettings::instance().GetProjectDir())->getValue("inBack");
				gcode = GCodeUtils::Duidao("Y", 1, 0, 0, inBack.toDouble());
			}
				break;
			case KNLK_ZSUB:
			{
				QString inBack = DataForms::getInstance()->getDataForm("DuanMianForm", SystemSettings::instance().GetProjectDir())->getValue("inBack");
				gcode = GCodeUtils::Duidao("Z", -1, 0, 0, inBack.toDouble());
			}
				break;
			case KNLK_ZADD:
			{
				QString inBack = DataForms::getInstance()->getDataForm("DuanMianForm", SystemSettings::instance().GetProjectDir())->getValue("inBack");
				gcode = GCodeUtils::Duidao("Z", 1, 0, 0, inBack.toDouble());
			}
				break;
			// 此时摁其他键，停止对刀
			default:
				enterSetPriority(0);
				GoJog();
				exitSetPriority();
				m_nowOperating = 0;
				break;
			}
			if (!gcode.isEmpty()) {
				m_nowOperating = 0;
				this->RunGCode(gcode);
			}
		}
		else {
			// 对刀中，只能按停止键停止对刀
			switch (m_key)
			{
			case KNLK_STOP:
				QMetaObject::invokeMethod(this, [this]() {
					this->StopRun();
					}, Qt::QueuedConnection);
				m_nowOperating = 0;
			}
		}
	}
	else {
		switch (m_key)
		{
		case KNLK_XSUB:
			if (CanBeJogDDMS() && PropertyObjects::getInstance()->propertyObjectJog->ms() == JogPropertyObject::MsEnum::DDMS) {
				enterSetPriority(0);
				PropertyObjects::getInstance()->propertyObjectDianDong->setbjjgz(DDYDPropertyObject::AxisEnum::X);
				PropertyObjects::getInstance()->propertyObjectDianDong->setbjmcs(-abs(PropertyObjects::getInstance()->propertyObjectDianDong->bjmcs()));
				PropertyObjects::getInstance()->propertyObjectDianDong->ExecuteCmds(this);
				exitSetPriority();
			}
			//QMetaObject::invokeMethod(this, [this]() {
			//	FormUtils::MessageBoxInfo("a");
			//	}, Qt::QueuedConnection);
			
			break;
		case KNLK_XADD:
			if (CanBeJogDDMS() && PropertyObjects::getInstance()->propertyObjectJog->ms() == JogPropertyObject::MsEnum::DDMS) {
				enterSetPriority(0);
				PropertyObjects::getInstance()->propertyObjectDianDong->setbjjgz(DDYDPropertyObject::AxisEnum::X);
				PropertyObjects::getInstance()->propertyObjectDianDong->setbjmcs(abs(PropertyObjects::getInstance()->propertyObjectDianDong->bjmcs()));
				PropertyObjects::getInstance()->propertyObjectDianDong->ExecuteCmds(this);
				exitSetPriority();
			}
			break;
		case KNLK_YSUB:
			if (CanBeJogDDMS() && PropertyObjects::getInstance()->propertyObjectJog->ms() == JogPropertyObject::MsEnum::DDMS) {
				enterSetPriority(0);
				PropertyObjects::getInstance()->propertyObjectDianDong->setbjjgz(DDYDPropertyObject::AxisEnum::Y);
				PropertyObjects::getInstance()->propertyObjectDianDong->setbjmcs(-abs(PropertyObjects::getInstance()->propertyObjectDianDong->bjmcs()));
				PropertyObjects::getInstance()->propertyObjectDianDong->ExecuteCmds(this);
				exitSetPriority();
			}
			break;
		case KNLK_YADD:
			if (CanBeJogDDMS() && PropertyObjects::getInstance()->propertyObjectJog->ms() == JogPropertyObject::MsEnum::DDMS) {
				enterSetPriority(0);
				PropertyObjects::getInstance()->propertyObjectDianDong->setbjjgz(DDYDPropertyObject::AxisEnum::Y);
				PropertyObjects::getInstance()->propertyObjectDianDong->setbjmcs(abs(PropertyObjects::getInstance()->propertyObjectDianDong->bjmcs()));
				PropertyObjects::getInstance()->propertyObjectDianDong->ExecuteCmds(this);
				exitSetPriority();
			}
			break;
		case KNLK_ZSUB:
			if (CanBeJogDDMS() && PropertyObjects::getInstance()->propertyObjectJog->ms() == JogPropertyObject::MsEnum::DDMS) {
				enterSetPriority(0);
				PropertyObjects::getInstance()->propertyObjectDianDong->setbjjgz(DDYDPropertyObject::AxisEnum::Z);
				PropertyObjects::getInstance()->propertyObjectDianDong->setbjmcs(-abs(PropertyObjects::getInstance()->propertyObjectDianDong->bjmcs()));
				PropertyObjects::getInstance()->propertyObjectDianDong->ExecuteCmds(this);
				exitSetPriority();
			}
			break;
		case KNLK_ZADD:
			if (CanBeJogDDMS() && PropertyObjects::getInstance()->propertyObjectJog->ms() == JogPropertyObject::MsEnum::DDMS) {
				enterSetPriority(0);
				PropertyObjects::getInstance()->propertyObjectDianDong->setbjjgz(DDYDPropertyObject::AxisEnum::Z);
				PropertyObjects::getInstance()->propertyObjectDianDong->setbjmcs(abs(PropertyObjects::getInstance()->propertyObjectDianDong->bjmcs()));
				PropertyObjects::getInstance()->propertyObjectDianDong->ExecuteCmds(this);
				exitSetPriority();
			}
			break;
		case KNLK_CENT:
			PrepareCent();
			break;
		case KNLK_ST:

			break;
		case KNLK_STEP:
			enterSetPriority(0);
			if (CanBeJogDDMS()) {
				if (PropertyObjects::getInstance()->propertyObjectJog->ms() != JogPropertyObject::MsEnum::DDMS) {
					PropertyObjects::getInstance()->propertyObjectJog->setms(JogPropertyObject::MsEnum::DDMS);
					PropertyObjects::getInstance()->propertyObjectShouKongHe->setddsdkz(SKHKZPropertyObject::Enum3::DD);
				}
				else {
					PropertyObjects::getInstance()->propertyObjectJog->setms(JogPropertyObject::MsEnum::SDMS);
					PropertyObjects::getInstance()->propertyObjectShouKongHe->setddsdkz(SKHKZPropertyObject::Enum3::SD);
				}
				//PropertyObjects::getInstance()->propertyObjectJog->ExecuteCmds(this);
				PropertyObjects::getInstance()->propertyObjectJog->ExecuteCmdSetMs(this);
				PropertyObjects::getInstance()->propertyObjectShouKongHe->ExecuteCmds(this);
				Beep();
			}
			else {
				Beep(2);
			}
			
			exitSetPriority();
			break;
		case KNLK_SPEEDD:
		{
			enterSetPriority(0);
			auto v = PropertyObjects::getInstance()->propertyObjectShouKongHe->sdxskz();
			if (v != SKHKZPropertyObject::Enum2::X1)
			{
				SetSpeedGear((int)v - 1);
				Beep();
			}
			else {
				Beep(2);
			}
			exitSetPriority();
		}
		break;
		case KNLK_SPEEDU:
		{
			enterSetPriority(0);
			auto v = PropertyObjects::getInstance()->propertyObjectShouKongHe->sdxskz();
			if (v != SKHKZPropertyObject::Enum2::X100)
			{
				SetSpeedGear((int)v + 1);
				Beep();
			}
			else {
				Beep(2);
			}
			exitSetPriority();
		}
		break;
		case KNLK_PUMP1:
			enterSetPriority(0);
			PropertyObjects::getInstance()->propertyObjectFuzhuIO->setyb1(!PropertyObjects::getInstance()->propertyObjectFuzhuIO->yb1());
			PropertyObjects::getInstance()->propertyObjectFuzhuIO->ExecuteCmds(this);
			Beep();
			exitSetPriority();
			break;
		case KNLK_PUMP2:
			enterSetPriority(0);
			PropertyObjects::getInstance()->propertyObjectFuzhuIO->setyb2(!PropertyObjects::getInstance()->propertyObjectFuzhuIO->yb2());
			PropertyObjects::getInstance()->propertyObjectFuzhuIO->ExecuteCmds(this);
			Beep();
			exitSetPriority();
			break;
		case KNLK_START:
			break;
		case KNLK_STOP:

			//QMetaObject::invokeMethod(this, [this]() {
			//	this->StopRun();
			//	}, Qt::QueuedConnection);

			//m_isBackzeroing = 0;
			break;
		case KNLK_ENT:
			break;
		default:
			break;
		}
	}

	emit keyPressed(m_key);
}

bool NCMachine::CanBeJogDDMS()
{
	return m_state[2] == NCT_STATE_JOG_RUN || (m_state[2] == NCT_STATE_SPK_RUN && PropertyObjects::getInstance()->propertyObjectSPK->zgj() == 2);
}

void NCMachine::SetSpeedGear(int v)
{
	if (v < 0 || v >= 4)
		return;

	int jog_v[4] = { 100, 10, 2, 1 };

	PropertyObjects::getInstance()->propertyObjectShouKongHe->setsdxskz((SKHKZPropertyObject::Enum2)v);
	PropertyObjects::getInstance()->propertyObjectShouKongHe->ExecuteCmds(this);

	DataForm* dataForm = DataForms::getInstance()->getDataForm("xitongshezhi2");
	PropertyObjects::getInstance()->propertyObjectJog->setxV(dataForm->getValue("SPEED_JOG_V_X").toInt() / jog_v[v]);
	PropertyObjects::getInstance()->propertyObjectJog->setyV(dataForm->getValue("SPEED_JOG_V_Y").toInt() / jog_v[v]);
	PropertyObjects::getInstance()->propertyObjectJog->setzV(dataForm->getValue("SPEED_JOG_V_Z").toInt() / jog_v[v]);
	PropertyObjects::getInstance()->propertyObjectJog->setuV(dataForm->getValue("SPEED_JOG_V_U").toInt() / jog_v[v]);
	PropertyObjects::getInstance()->propertyObjectJog->setrun(false);
	PropertyObjects::getInstance()->propertyObjectJog->ExecuteCmds(this);
	PropertyObjects::getInstance()->propertyObjectJog->setrun(true);

	PropertyObjects::getInstance()->propertyObjectDianDong->setbjmcs(SPEED_DIANDONG_V / jog_v[v]);
	//PropertyObjects::getInstance()->propertyObjectDianDong->ExecuteCmds(this);
}

void NCMachine::ProcessPadset()
{
	if (m_targetType <= 0x00FF)
	{
		switch (m_targetType & 0x00F0)
		{
		case 0x00:
			break;
		case 0x10:
		{
			int axis = m_targetType & 0x000F;
			if (axis >= 0 && axis < 4) {
				RunGCode(QString("G92 %1%2").arg(GCodeTool::axis[axis]).arg(m_targetPosX / 1000.0, 0, 'f', 3));
				//m_posOffset[axis] = m_targetPosX - m_pos[axis];
			}
			emit keyPressed(m_key);
		}
		break;
		case 0x20:
		{
			int axis = m_targetType & 0x000F;
			if (axis >= 0 && axis < 4) {
				RunGCode(QString("G90 G00 %1%2").arg(GCodeTool::axis[axis]).arg(m_targetPosX / 1000.0, 0, 'f', 3));
			}
			/*this->GoApi();
			this->Loc(TMBS_MAP0_ID_XPOS + axis, m_targetPosX - m_posOffset[axis]);
			this->GoJog();*/
			emit keyPressed(m_key);
		}
		break;
		}
	}
}

void NCMachine::ProcessPos()
{
	bool changed = false;
	std::vector<uint16_t> v;
	for (int i = 0; i < POS_PAD_AXIS_LEN; ++i)
	{
		//int value = m_pos.data[i] + m_posOffset.data[i];
		int value = int(GetController()->getAxisPosition(GCodeTool::axis[i]) * 1000);

		v.push_back(value & 0xFFFF);
		v.push_back(value >> 16);
		if (m_axisPositionLastSend[i] != value) {
			changed = true;
			m_axisPositionLastSend[i] = value;
		}
	}
	int csNow = GetController()->get(CURRENT_COORD_SYSTEM) - 1;
	if (csNow != m_axisCsLast) {
		m_axisCsLast = csNow;
		changed = true;
	}

	if (changed) {
		enterSetPriority(2);
		this->executeCmdsByAddress(TMBS_MAP0_ID_XOFST, v);
		exitSetPriority();
		
		emit positionChanged();
	}

	if (m_g01Data.isRunning && m_g01Data.isPausing && m_g01Data.lastPosSet) {
		// 当位置不再变化时，增加路径点
		if (!changed) {
			if (m_g01Data.lastPosList.count() == 0 || m_g01Data.lastPosList.last() != m_pos) {
				m_g01Data.lastPosList.append(m_pos);
				LOG_INFO(8, "NCMachine: add to lastPosList:  " << m_pos);
			}
		}
	}
}

std::vector<std::tuple<std::function<int()>, std::string>> NCMachine::doTaskJson(cb::JSON::ValuePtr json, GCodeTask* task, TaskThread<GCodeTask>* taskThread)
{
	//char* szTemp = (char*)1;
	//strcpy_s(szTemp, 1000, "A");

	std::vector<std::tuple<std::function<int()>, std::string>> checkFunctions;
	checkFunctions.push_back(std::tuple<std::function<int()>, std::string>([this, task]() {
		//EUtils::sleep(10000); // just for test
		int cnt = m_modbus->getTaskCnt(task->priority);
		return cnt == 0 ? 1 : -2;
		}, "wait for modbus-thread finish all tasks"));

	if (m_g01Data.isRunning) {
		m_g01Data.isRunning = false;
		m_g01Data.lastWorkSeconds = m_g01Data.nowWorkSeconds + m_g01Data.startTime.secsTo(QDateTime::currentDateTime());
		//m_g01Data.allWorkSeconds += m_g01Data.lastWorkSeconds;
		m_g01Data.allWorkSeconds += m_g01Data.startTime.secsTo(QDateTime::currentDateTime());
		m_g01Data.nowWorkSeconds = 0;

		if (m_g01Data.workDetails.count() > 0) {
			std::tuple<QString, QDateTime, QDateTime, int>& lastItem = m_g01Data.workDetails[m_g01Data.workDetails.count() - 1];
			std::get<2>(lastItem) = QDateTime::currentDateTime();
			std::get<3>(lastItem) = m_g01Data.lastWorkSeconds;
		}

		// re-calculate allWorkSeconds. allWorkSeconds已经包含了暂停前的时间，lastWorkSeconds也包含了暂停前的时间。
		int allWorkSeconds = 0;
		for (int i = 0; i < m_g01Data.workDetails.length(); i++) {
			allWorkSeconds += std::get<3>(m_g01Data.workDetails[i]);
		}
		m_g01Data.allWorkSeconds = allWorkSeconds;
	}

	std::string action = json->getString("type");
	if (action == "api")
	{
		enterSetPriority(task->priority);
		GoApi();
		exitSetPriority();
		// already do in GoApi. but GoApi() is wait in modbus task thread.
		checkFunctions.push_back(std::tuple<std::function<int()>, std::string>(waitUntilNctState(NCT_STATE_API_RUN), "wait for API_RUN"));
	}
	else if (action == "jog")
	{
		enterSetPriority(task->priority);
		GoApi();
		GoJog();
		exitSetPriority();
		checkFunctions.push_back(std::tuple<std::function<int()>, std::string>(waitUntilNctState(NCT_STATE_JOG_RUN), "wait for JOG_RUN"));
	}
	else if (action == "units") {
		//{"type": "units", "value" : "METRIC"},
	}
	else if (action == "source") {
		//{"type": "source", "value" : "textbox.nc"},
	}
	else if (action == "line") {
		m_currentGCodeLine = json->getS32("value");
	}
	else if (action == "C_command") {
	}
	else if (action == "move") {
		enterSetPriority(task->priority);
		GoApi();
		if (json->hasBoolean("rapid") && json->getBoolean("rapid")) {
			cb::JSON::ValuePtr to = json->get("to");
			std::vector<int> axis;
			std::vector<int> value;
			if (to->hasNumber("x")) {
				axis.push_back(TMBS_MAP0_ID_XPOS);
				value.push_back(int(to->getNumber("x") * 1000));
			}
			if (to->hasNumber("y")) {
				axis.push_back(TMBS_MAP0_ID_YPOS);
				value.push_back(int(to->getNumber("y") * 1000));
			}
			if (to->hasNumber("z")) {
				axis.push_back(TMBS_MAP0_ID_ZPOS);
				value.push_back(int(to->getNumber("z") * 1000));
			}
			if (to->hasNumber("u")) {
				axis.push_back(TMBS_MAP0_ID_UPOS);
				value.push_back(int(to->getNumber("u") * 1000));
			}

			auto funcCheckPos = [axis, value, this]() {
				for (uint i = 0; i < axis.size(); ++i) {
					if (axis[i] == TMBS_MAP0_ID_XPOS && this->m_pos[0] != value[i])
						return -1;
					if (axis[i] == TMBS_MAP0_ID_YPOS && this->m_pos[1] != value[i])
						return -1;
					if (axis[i] == TMBS_MAP0_ID_ZPOS && this->m_pos[2] != value[i])
						return -1;
					if (axis[i] == TMBS_MAP0_ID_UPOS && this->m_pos[3] != value[i])
						return -1;
				}
				return 1;
				};
			if (funcCheckPos() == -1) {
				Loc(axis.data(), value.data(), axis.size());

				// if location is reached, the state won't change
				//checkFunctions.push_back([this]() {
				//	return this->m_state[2] == NCT_STATE_LOC_RUN ? 1 : -1;
				//	});

				LOG_INFO(5, "EDM: move rapid to " << json->getDict("to"));

				// 检测是否短路等
				checkFunctions.push_back(std::tuple<std::function<int()>, std::string>(waitUntilRLST(NCT_STATE_LOC_EXIT), "wait for LOC_EXIT"));
				checkFunctions.push_back(std::tuple<std::function<int()>, std::string>(funcCheckPos, "wait for LOC_POS_OK"));
			}
			
		}
		else {
			cb::JSON::ValuePtr to = json->get("to");
			std::vector<int> axis;
			std::vector<int> value;
			if (to->hasNumber("x")) {
				int v = int(to->getNumber("x") * 1000);
				if (v != m_pos.x()) {
					axis.push_back(TMBS_MAP0_ID_XPOS);
					value.push_back(v);
				}
			}
			if (to->hasNumber("y")) {
				int v = int(to->getNumber("y") * 1000);
				if (v != m_pos.y()) {
					axis.push_back(TMBS_MAP0_ID_YPOS);
					value.push_back(v);
				}
			}
			if (to->hasNumber("z")) {
				int v = int(to->getNumber("z") * 1000);
				if (v != m_pos.z()) {
					axis.push_back(TMBS_MAP0_ID_ZPOS);
					value.push_back(v);
				}
			}
			if (to->hasNumber("u")) {
				int v = int(to->getNumber("u") * 1000);
				if (v != m_pos.data[3]) {
					axis.push_back(TMBS_MAP0_ID_UPOS);
					value.push_back(v);
				}
			}

			if (value.size() > 0) {
				std::function<void()> doFuncWait = [this, axis, value]() {
					std::lock_guard<std::mutex> lock(m_mutex);

					m_g01Data.isRunning = true;
					m_g01Data.isPausing = false;
					m_g01Data.startTime = QDateTime::currentDateTime();
					m_g01Data.startPos = m_pos;

					//m_g01Data.cNo = cIndex;
					m_g01Data.newData = true;

					QDateTime now = QDateTime::currentDateTime();
					m_g01Data.workDetails.append(std::tuple<QString, QDateTime, QDateTime, int>("C" + QString::number(m_g01Data.cNo).rightJustified(3, '0'), now, now, 0));

					if (value.size() == 1) {
						switch (axis[0])
						{
						case TMBS_MAP0_ID_ZPOS:
							//m_g01Data.plane = GCode::MachineEnum::plane_t::XY;
							m_g01Data.to_p = Vector3I(m_pos.x(), m_pos.y(), value[0]);
							m_g01Data.from_p = Vector3I(m_pos.x(), m_pos.y(), m_pos.z());
							m_g01Data.start_z = m_pos.z();
							m_g01Data.to_z = value[0];
							break;
						case TMBS_MAP0_ID_YPOS:
							//m_g01Data.plane = GCode::MachineEnum::plane_t::XZ;
							m_g01Data.to_p = Vector3I(m_pos.x(), value[0], m_pos.z());
							m_g01Data.from_p = Vector3I(m_pos.x(), m_pos.y(), m_pos.z());
							m_g01Data.start_z = m_pos.y();
							m_g01Data.to_z = value[0];
							break;
						case TMBS_MAP0_ID_XPOS:
							//m_g01Data.plane = GCode::MachineEnum::plane_t::YZ;
							m_g01Data.to_p = Vector3I(value[0], m_pos.y(), m_pos.z());
							m_g01Data.from_p = Vector3I(m_pos.x(), m_pos.y(), m_pos.z());
							m_g01Data.start_z = m_pos.x();
							m_g01Data.to_z = value[0];
							break;
						case TMBS_MAP0_ID_UPOS:
							m_g01Data.to_p = Vector3I(m_pos.x(), m_pos.y(), m_pos.z());
							m_g01Data.from_p = Vector3I(m_pos.x(), m_pos.y(), m_pos.z());
							m_g01Data.start_z = m_pos.data[3];
							m_g01Data.to_z = value[0];
							break;
						default:
							THROW("invalid axis");
							break;
						}
					}
					else if (value.size() == 2 || value.size() == 3 || value.size() == 4)
					{
						int x = m_pos.x(), y = m_pos.y(), z = m_pos.z();
						for (size_t i = 0; i < value.size(); ++i)
						{
							switch (axis[i])
							{
							case TMBS_MAP0_ID_XPOS:
								x = value[i];
								break;
							case TMBS_MAP0_ID_YPOS:
								y = value[i];
								break;
							case TMBS_MAP0_ID_ZPOS:
								z = value[i];
								break;
							}
						}
						m_g01Data.to_p = Vector3I(x, y, z);
						m_g01Data.from_p = Vector3I(m_pos.x(), m_pos.y(), m_pos.z());

						m_g01Data.start_z = 0;
						Vector3I p = MathTool::TransformPoint(m_g01Data.to_p, m_g01Data.from_p, m_g01Data.to_p);
						m_g01Data.to_z = p.z();
					}
					else {
						THROW(EUtils::QString2StdString(QStringLiteral("目前不支持多于4个坐标轴加工")));
					}

					m_g01Data.now_z = m_g01Data.start_z;
					m_g01Data.step = PropertyObjects::getInstance()->propertyObjectYd->ydbj();
					m_g01Data.dssj = PropertyObjects::getInstance()->propertyObjectFdwc->fddssj();
					LOG_INFO(8, "NCMachine: g01Data center set to " << m_g01Data.from_p.x() << "," << m_g01Data.from_p.y());
					};
				executeCmdWait(convertWaitFunction(waitno(doFuncWait)), "set init g01Data");

				if (!m_g01Data.isSimulate) {
					if (m_g01Data.cNo == SPK_CNO_SPECIAL) {
						//PropertyObjects::getInstance()->propertyObjectYd->settcqhtjl(0);
						/*DataForm* dataForm3 = DataForms::getInstance()->getDataForm("xitongshezhi3");
						PropertyObjects::getInstance()->propertyObjectYd->settcqhtjl(dataForm3->getValue("FDDWHTJL").toInt());*/
						PropertyObjects::getInstance()->propertyObjectYd->settcqhtjl(SystemSettings::instance().GetValue("FDDWHTJL").toInt());
					}
					//else {
					//	DataForm* dataForm3 = DataForms::getInstance()->getDataForm("xitongshezhi3");
					//	PropertyObjects::getInstance()->propertyObjectYd->settcqhtjl(dataForm3->getValue("TCQHTJL").toInt());
					//}
					NCMachineParametersC::ExecuteCmds(this);

					Spk(axis.data(), value.data(), axis.size(), m_g01Data.cNo == SPK_CNO_SPECIAL ? 2 : 1);

					//checkFunctions.push_back(std::tuple<std::function<int()>, std::string>(waitUntilNctState(NCT_STATE_SPK_RUN), "wait for SPK_RUN"));
					checkFunctions.push_back(std::tuple<std::function<int()>, std::string>(waitUntilRLST(NCT_STATE_SPK_EXTOC, 0), "wait for SPK_EXTOC"));

					std::function<void(int, GCodeTask*, NCMachine*)> func1 = [this](int, GCodeTask*, NCMachine*) {
						PropertyObjects::getInstance()->propertyObjectFdjg->ExecuteCmdsSetSv(this, 3, 0);
						// initialize fdwcms
						PropertyObjects::getInstance()->propertyObjectFdwc->setfdwcms(0);

						//DataForm* dataForm3 = DataForms::getInstance()->getDataForm("xitongshezhi3");
						//int v = dataForm3->getValue("TCQHTJL").toInt();
						PropertyObjects::getInstance()->propertyObjectYd->settcqhtjl(DataForms::getInstance()->getValue("TCQHTJL").toInt());
						};
					task->postDoFunction = func1;
					task->postDoFunctionDesc = "SET_SV=3";
					LOG_INFO(5, "EDM: move to " << json->getDict("to"));
				}
			}
		}
		exitSetPriority();
	}
	else if (action == "dwell") {
		int seconds = json->getNumber("seconds");
		this->m_dwellEndTime = QDateTime::currentDateTime().addSecs(seconds);
		for (int i = 0; i < seconds; ++i)
		{
			if (!taskThread->isTaskToContinue())
				break;
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}
	else if (action == "custom")
	{
		enterSetPriority(task->priority);
		
		cb::JSON::Value& parameters = json->getDict("value");
		std::string action2 = parameters.getString("type");

		if (action2 == "touch") {
			GoApi();

			char axis = parameters.getS16("axis");
			double pos = parameters.getNumber("pos");

			int savedShouKongHesdxskz = PropertyObjects::getInstance()->propertyObjectShouKongHe->sdxskz();
			std::function<void(int, GCodeTask*, NCMachine*)> func1 = [this, savedShouKongHesdxskz](int, GCodeTask*, NCMachine*) {
				enterSetPriority(0);
				this->SetSpeedGear(savedShouKongHesdxskz);
				exitSetPriority();
				};
			task->postDoFunction = func1;
			task->postDoFunctionDesc = "SET_JOG_SPEED=BACKUP";

			this->SetSpeedGear((int)SKHKZPropertyObject::Enum2::X10);

			switch (axis) {
			case 'X':
				if (pos > 0)
					Touch(DdfxEnum::X_PLUS);
				else
					Touch(DdfxEnum::X_MINUS);
				break;
			case 'Y':
				if (pos > 0)
					Touch(DdfxEnum::Y_PLUS);
				else
					Touch(DdfxEnum::Y_MINUS);
				break;
			case 'Z':
				if (pos > 0)
					Touch(DdfxEnum::Z_PLUS);
				else
					Touch(DdfxEnum::Z_MINUS);
				break;
			case 'U':
				if (pos > 0)
					Touch(DdfxEnum::U_PLUS);
				else
					Touch(DdfxEnum::U_MINUS);
				break;
			default:
				LOG_WARNING("invalid axis for touch");
				break;
			}

			//checkFunctions.push_back([this]() {
			//	return this->m_state[2] == NCT_STATE_CMV_RUN ? 1 : -1;
			//});
			checkFunctions.push_back(std::tuple<std::function<int()>, std::string>(waitUntilRLST(NCT_STATE_CMV_EXIT), "wait for CMV_EXIT"));
			//checkFunctions.push_back(waitUntilNctState(NCT_STATE_API_RUN));
		}
		else if (action2 == "tolimit") {
			GoApi();

			char axis = parameters.getS16("axis");
			double pos = parameters.getNumber("pos");
			switch (axis) {
			case 'X':
				if (pos > 0)
					ToLMT(DdfxEnum::X_PLUS);
				else
					ToLMT(DdfxEnum::X_MINUS);
				break;
			case 'Y':
				if (pos > 0)
					ToLMT(DdfxEnum::Y_PLUS);
				else
					ToLMT(DdfxEnum::Y_MINUS);
				break;
			case 'Z':
				if (pos > 0)
					ToLMT(DdfxEnum::Z_PLUS);
				else
					ToLMT(DdfxEnum::Z_MINUS);
				break;
			case 'U':
				if (pos > 0)
					ToLMT(DdfxEnum::U_PLUS);
				else
					ToLMT(DdfxEnum::U_MINUS);
				break;
			default:
				LOG_WARNING("invalid axis for tolimit");
				break;
			}

			checkFunctions.push_back(std::tuple<std::function<int()>, std::string>(waitUntilRLST(NCT_STATE_CMV_EXIT), "wait for CMV_EXIT"));
		}
		else if (action2 == "fromlimit"){
			GoApi();

			char axis = parameters.getS16("axis");
			double pos = parameters.getNumber("pos");
			switch (axis) {
			case 'X':
				if (pos > 0)
					FromLMT(DdfxEnum::X_PLUS);
				else
					FromLMT(DdfxEnum::X_MINUS);
				break;
			case 'Y':
				if (pos > 0)
					FromLMT(DdfxEnum::Y_PLUS);
				else
					FromLMT(DdfxEnum::Y_MINUS);
				break;
			case 'Z':
				if (pos > 0)
					FromLMT(DdfxEnum::Z_PLUS);
				else
					FromLMT(DdfxEnum::Z_MINUS);
				break;
			case 'U':
				if (pos > 0)
					FromLMT(DdfxEnum::U_PLUS);
				else
					FromLMT(DdfxEnum::U_MINUS);
				break;
			default:
				LOG_WARNING("invalid axis for fromlimit");
				break;
			}

			checkFunctions.push_back(std::tuple<std::function<int()>, std::string>(waitUntilRLST(NCT_STATE_CMV_EXIT), "wait for CMV_EXIT"));
		}
		else if (action2 == "tozsig") {
			GoApi();

			char axis = parameters.getS16("axis");
			double pos = parameters.getNumber("pos");
			switch (axis) {
			case 'X':
				if (pos > 0)
					ToZSig(DdfxEnum::X_PLUS);
				else
					ToZSig(DdfxEnum::X_MINUS);
				break;
			case 'Y':
				if (pos > 0)
					ToZSig(DdfxEnum::Y_PLUS);
				else
					ToZSig(DdfxEnum::Y_MINUS);
				break;
			case 'Z':
				if (pos > 0)
					ToZSig(DdfxEnum::Z_PLUS);
				else
					ToZSig(DdfxEnum::Z_MINUS);
				break;
			case 'U':
				if (pos > 0)
					ToZSig(DdfxEnum::U_PLUS);
				else
					ToZSig(DdfxEnum::U_MINUS);
				break;
			default:
				LOG_WARNING("invalid axis for tozsig");
				break;
			}

			checkFunctions.push_back(std::tuple<std::function<int()>, std::string>(waitUntilRLST(NCT_STATE_CMV_EXIT), "wait for CMV_EXIT"));
		}
		else if (action2 == "backtozero") {
			char axis = parameters.getS16("axis");
			double pos = parameters.getNumber("pos");

			//int selectedAxisI = 0;
			switch (axis) {
			case 'X':
				if (pos > 0)
					BacktoZero(DdfxEnum::X_PLUS);
				else
					BacktoZero(DdfxEnum::X_MINUS);
				//selectedAxisI = 0;
				break;
			case 'Y':
				if (pos > 0)
					BacktoZero(DdfxEnum::Y_PLUS);
				else
					BacktoZero(DdfxEnum::Y_MINUS);
				//selectedAxisI = 1;
				break;
			case 'Z':
				if (pos > 0)
					BacktoZero(DdfxEnum::Z_PLUS);
				else
					BacktoZero(DdfxEnum::Z_MINUS);
				//selectedAxisI = 2;
				break;
			default:
				LOG_WARNING("invalid axis for backtozero");
				break;
			}
			// 必须放在最前面
			checkFunctions.insert(checkFunctions.begin(), std::tuple<std::function<int()>, std::string>([this]() {
				return this->m_nowOperating == 2 ? 1 : -2;
				}, "wait for nowOperating == 2"));
			checkFunctions.push_back(std::tuple<std::function<int()>, std::string>([this]() {
				return this->m_nowOperating == 0 ? 1 : -2;
				}, "wait for nowOperating == 0"));
		}
		else if (action2 == "zeroaxis") {
			GoApi();

			int axis = parameters.getS32("axis");
			
			for (int i = 0; i < ZEROAXIS_AXIS_LEN; ++i) {
				if (axis & GCodeTool::axisInt[i]) {
					SetPosOne(TMBS_MAP0_ID_XPOS + i, 0);
					std::function<void()> doFuncZeroAxisInController = [this, i]() {
						GetController()->setAxisAbsolutePosition(GCodeTool::axis[i], 0, GCode::Units::METRIC);
						};
					executeCmdWait(convertWaitFunction(waitno(doFuncZeroAxisInController)), "doFuncZeroAxisInController");
				}
			}
			// 统一螺距补偿使能
			for (int i = 0; i < LJBC_AXIS_LEN; ++i) {
				if (axis & GCodeTool::axisInt[i]) {
					PropertyObjects::getInstance()->propertyObjectZLJBC[i]->setbcsx(true);
				}
			}
			ZLJBCPropertyObject::ExecuteCmds2(this, PropertyObjects::getInstance()->propertyObjectZLJBC);
		}
		else if (action2 == "open_yb") {
			if (!m_g01Data.isSimulate) {
				PropertyObjects::getInstance()->propertyObjectFuzhuIO->setyb1(true);
				if (DataForms::getInstance()->getDataForm("xitongshezhi5")->getValue("ZDKQYB2") != "0") {
					PropertyObjects::getInstance()->propertyObjectFuzhuIO->setyb2(true);
				}
				PropertyObjects::getInstance()->propertyObjectFuzhuIO->ExecuteCmds(this);
			}
		}
		else if (action2 == "close_yb") {
			if (!m_g01Data.isSimulate) {
				PropertyObjects::getInstance()->propertyObjectFuzhuIO->setyb1(false);
				if (DataForms::getInstance()->getDataForm("xitongshezhi5")->getValue("ZDKQYB2") != "0") {
					PropertyObjects::getInstance()->propertyObjectFuzhuIO->setyb2(false);
				}
				PropertyObjects::getInstance()->propertyObjectFuzhuIO->ExecuteCmds(this);
			}
		}
		else if (action2 == "close_jcgz") {
			PropertyObjects::getInstance()->propertyObjectLoc->setjcdl(false);
		}
		else if (action2 == "open_jcgz") {
			PropertyObjects::getInstance()->propertyObjectLoc->setjcdl(true);
		}
		else if (action2 == "move_backto") {
			int v = parameters.getS32("v");
			if (v == -1) {
				DataForm* dataForm3 = DataForms::getInstance()->getDataForm("xitongshezhi3");
				v = dataForm3->getValue("TCQHTJL").toInt();
			}
			PropertyObjects::getInstance()->propertyObjectYd->settcqhtjl(v);
		}
		else if (action2 == "dsjg") {
			double seconds = parameters.getNumber("seconds");
			PropertyObjects::getInstance()->propertyObjectFdwc->setfddssj(1000 * seconds);
			if (parameters.getS32("isT") == 1)
				PropertyObjects::getInstance()->propertyObjectFdwc->setfdwcms(3);
			else
				PropertyObjects::getInstance()->propertyObjectFdwc->setfdwcms(2);
			PropertyObjects::getInstance()->propertyObjectFdwc->ExecuteCmds(this);
		}
		else if (action2 == "setc") {
			// set c values
			auto controller = GetController();
			int cIndex = static_cast<int>(controller->getVar('C'));
			if (cIndex >= 0 && cIndex < 1000) {
				m_g01Data.cNo = cIndex;

				// first set with global c
				NCMachineParametersC::instance().SetByCIndex(cIndex);

				// set with c in current file
				if (m_paramCs.contains(cIndex)) {
					QStringList parts = m_paramCs[cIndex];
					for (int i = 0; i < m_paramCsNames.length() && i < parts.length(); ++i)
					{
						if (parts[i].isEmpty())
							continue;
						NCMachineParametersC::SetCurrentByName(m_paramCsNames[i], parts[i]);
					}
				}

				//std::map<std::string, int> csmap = NCMachineParametersC::instance().namesMap2Index;
				//for (const auto& s : NCMachineParametersC::instance().names) {
				//	if (csmap.find(s) != csmap.end()) {
				//		std::string s2 = std::string(1, tolower(NCMachineParametersC::instance().namesChar[csmap[s]]));
				//		if (parameters.has(s2)) {
				//			NCMachineParametersC::SetCurrentByName(s, parameters.getAsString(s2));
				//		}
				//	}
				//}

				// todo: throw exception here will not interrupt running
				// done: catch exception and return -9 in doTask
				if (std::stoi(NCMachineParametersC::GetCurrentByName("SV")) > 180)
				{
					THROW(EUtils::QString2StdString(QStringLiteral("SV 必须小于 180！")));
				}
				// G01时候设置
				//NCMachineParametersC::ExecuteCmds(this);
			}
			else if (cIndex >= 9001 && cIndex < 9100)
			{
				bool setOk = true;
				
				auto& s = NCMachineParametersC::instance().names[cIndex - 9001];
				// 1. 必须用全局变量，否则get不到（局部变量用lookupReference）
				// 2. 必须用小写，controller内部自动转换为小写
				std::string pName = "_" + String::toLower(s);
				int eValue = static_cast<int>(round((controller->get(pName))));
				
				if (s == "PL") {
					if (eValue > 0)
						NCMachineParametersC::SetCurrentByName(s, "+");
					else
						NCMachineParametersC::SetCurrentByName(s, "-");
				}
				else if (s == "STEP") {
					double eValue2 = controller->get(pName);
					std::ostringstream oss;
					oss << std::fixed << std::setprecision(3) << eValue2;

					setOk = NCMachineParametersC::SetCurrentByName(s, oss.str());
					if (!setOk) {
						THROW(EUtils::QString2StdString(QStringLiteral("C命令参数%1=%2不满足要求！").arg(QString::fromStdString(s)).arg(QString::number(eValue))));
					}
				}
				else {
					QString sValue = QString::number(eValue).rightJustified(NCMachineParametersC::namesLen[cIndex - 9001], '0');
					//setOk = NCMachineParametersC::SetCurrentByName(s, std::to_string(eValue));
					setOk = NCMachineParametersC::SetCurrentByName(QString::fromStdString(s), sValue);
					if (!setOk) {
						THROW(EUtils::QString2StdString(QStringLiteral("C命令参数%1=%2不满足要求！").arg(QString::fromStdString(s)).arg(QString::number(eValue))));
					}
				}
				controller->clear(pName);
				// G01时候设置
				//if (setOk) {
				//	NCMachineParametersC::ExecuteCmds(this);
				//}
			}
			else {
				LOG_ERROR("EDM: Undefined C Index: " << cIndex);
			}
		}
		else if (action2 == "setc2") {
			std::string n = parameters.getAsString("n");
			double v = parameters.getNumber("v");
			if (n == "LEJL") {
				//if (PropertyObjects::getInstance()->propertyObjectYd->dmjtdmskgl() == 1) {
				//	PropertyObjects::getInstance()->propertyObjectYd->settdqmshtjl((int)(v * 1000));
				//	PropertyObjects::getInstance()->propertyObjectYd->ExecuteCmds(this);
				//}
				SystemSettings::instance().SetValue("LEJL", QString::number(v));
			}
			else if (n == "LEJS") {
				//if (PropertyObjects::getInstance()->propertyObjectYd->dmjtdmskgl() == 1) {
				//	PropertyObjects::getInstance()->propertyObjectYd->settdqmshtsd((int)(v));
				//	PropertyObjects::getInstance()->propertyObjectYd->ExecuteCmds(this);
				//}
				SystemSettings::instance().SetValue("LEJS", QString::number(v));
			}
			else if (n == "MDIV") {
				//SystemSettings::instance().SetValue("MDIV", QString::number(v));
				PropertyObjects::getInstance()->propertyObjectYd->setsdjcxfs((int)v);
				//PropertyObjects::getInstance()->propertyObjectYd->ExecuteCmds(this);
				
			}
			else if (n == "AOD") {
				//SystemSettings::instance().SetValue("AOD", QString::number(v));
				PropertyObjects::getInstance()->propertyObjectReg84->setv((int)v);
			}
			else {
				NCMachineParametersC& cInst = NCMachineParametersC::instance();
				for (const std::string& si : cInst.names) {
					if (n == si) {
						cInst.SetCurrentByName(n, parameters.getAsString("v"));
						// G01时候统一发送
						//if (b) {
						//	NCMachineParametersC::ExecuteCmds(this);
						//}
						break;
					}
				}
			}
		}
		else if (action2 == "message_yn") {
			tr("to_clear_machine_axis");
			GetController()->clear("message_ret");
			std::string n = parameters.getAsString("n");
			if (n[0] == '_') {

			}
			else {
				QMetaObject::invokeMethod(this, [this, n]() {
					bool b = FormUtils::MessageBoxYesNo(QApplication::translate("NCMachine", n.c_str()));
					GetController()->set("message_ret", b ? 1 : 2);
					}, Qt::QueuedConnection);
			}
			std::function<int()> func = [this]() {
				double v = GetController()->get("message_ret");
				return v != 0 ? 1 : -2;
				};
			checkFunctions.push_back(std::tuple<std::function<int()>, std::string>(func, "wait for message_ret"));
		}
		else if (action2 == "ajc") {
			char axis = parameters.getS16("axis");
			double pos = parameters.getNumber("pos");
			int lastAjc = PropertyObjects::getInstance()->propertyObjectAJC->ajc();
			int destAjc = -1;
			switch (axis) {
			case 'X':
				if (pos > 0)
					destAjc = 0;
				else
					destAjc = 1;
				break;
			case 'Y':
				if (pos > 0)
					destAjc = 2;
				else
					destAjc = 3;
				break;
			case 'Z':
				if (pos > 0)
					destAjc = 4;
				else
					destAjc = 5;
				break;
			default:
				destAjc = 9;
			}
			if (destAjc != lastAjc) {
				PropertyObjects::getInstance()->propertyObjectAJC->setajc(destAjc);
				PropertyObjects::getInstance()->propertyObjectAJC->ExecuteCmds(this);
			}
		}
		else if (action2 == "G83") {
			char axis = parameters.getS16("axis");
			switch (axis) {
			case 'T':
			{
				int idx = parameters.getS32("h");
				std::string hName = "_h" + std::to_string(idx);
				double v = m_g01Data.allWorkSeconds;
				GetController()->set(hName, v);
			}
				break;
			case 'A':
			{
				int idx = parameters.getS32("h");
				std::string hName = "_h" + std::to_string(idx);
				double v = m_g01Data.lastWorkSeconds;
				GetController()->set(hName, v);
			}
				break;
			default:
				assert("invalid axis for G83");
				break;
			}
		}
		else if (action2 == "beep") {
			int ms = parameters.getS32("ms");
			if (ms == 0) {
				Beep(0, -1);
			}
			else if (ms > 0 && ms < 1000 * 60) {
				Beep(1, ms);
			}
			else {
				Beep(0, 0);	// OFF
			}
		}
		else if (action2 == "light") {
			int v = parameters.getS32("v");
			if (v == 0) {
				PropertyObjects::getInstance()->propertyObjectFuzhuIO->setout1(false);
				PropertyObjects::getInstance()->propertyObjectFuzhuIO->setout2(false);
			} else if (v == 1) {	// 绿
				PropertyObjects::getInstance()->propertyObjectFuzhuIO->setout1(false);
				PropertyObjects::getInstance()->propertyObjectFuzhuIO->setout2(true);
			}
			else if (v == 2) {	// 红
				PropertyObjects::getInstance()->propertyObjectFuzhuIO->setout1(true);
				PropertyObjects::getInstance()->propertyObjectFuzhuIO->setout2(false);
			}
			else {
				PropertyObjects::getInstance()->propertyObjectFuzhuIO->setout1(false);
				PropertyObjects::getInstance()->propertyObjectFuzhuIO->setout2(false);
			}
			PropertyObjects::getInstance()->propertyObjectFuzhuIO->ExecuteCmds(this);
		}
		else if (action2 == "jogspeed") {
			int v = parameters.getS32("v");
			this->SetSpeedGear(v);
		}
		else if (action2 == "readreg") {
			std::string s = parameters.getAsString("s");
			int addr = std::stoi(s);
			int cnt = (int)GetController()->get("_reg_cnt");
			GetController()->clear("_reg_cnt");
			if (cnt == 0)
				cnt = 1;
			std::function<void(int, ModbusTask*, ModbusAdapter*)> function1 = [this](int ret, ModbusTask* task, ModbusAdapter* adapter) {
				if (ret == -1)
					return;
				uint16_t* readData = adapter->GetReadedData16();

				for (int i = 0; i < task->numOfRegs; ++i) {
					GetController()->set("_reg_" + std::to_string(task->startAddr + i), readData[i]);
				}
				};
			ModbusTask* task = m_modbus->getTaskRead(addr, cnt);
			task->setPostFunction(function1, "readreg");
			m_modbusTaskCache.addTask(task, m_currentTaskPriority);
		}
		else if (action2 == "writereg") {
/** Examples:
#<_reg_cnt> = 2
#<_reg_82> = 987
#<_reg_83> = 789
do_writereg 82 
***/
			std::string s = parameters.getAsString("s");
			int addr = std::stoi(s);
			int cnt = (int)GetController()->get("_reg_cnt");
			GetController()->clear("_reg_cnt");
			if (cnt == 0)
				cnt = 1;
			
			std::vector<uint16_t> vs;
			for (int i = 0; i < cnt; ++i) {
				vs.push_back((int)GetController()->get("_reg_" + std::to_string(addr + i)));
			}
			std::string data = NCCommand::UIntsToString(vs);
			ModbusTask* task = m_modbus->getTaskWrite(addr, cnt, data);
			m_modbusTaskCache.addTask(task, m_currentTaskPriority);
		}
		else if (action2 == "message") {
			std::string s = parameters.getAsString("s");
			QMetaObject::invokeMethod(this, [this, s]() {
				FormUtils::MessageBoxInfo(tr(s.c_str()));
				}, Qt::QueuedConnection);
		}
		else {
			LOG_ERROR("EDM: Undefined gcode custom action: " << action2);
		}

		exitSetPriority();
	}
	else if (action == "pause")
	{
		if (json->has("optional")) {

		}
		else if (json->has("pallet-change")) {

		}
		PauseRun();
	}
	else {
		LOG_ERROR("EDM: Undefined gcode action: " << action);
	}

	return checkFunctions;
}

void NCMachine::enterSetPriority(int priority)
{
	//if (m_currentTaskPriority == priority) {
	//	m_currentTaskPriorityLockCnt++;
	//	return;
	//}
	//m_priorityMutex.lock();
	//assert(m_currentTaskPriority == -1);
	//assert(priority >= 0 && priority < TASKQUEUE_MAX_COUNT);
	////m_currentTaskPriorityPrev.store(m_currentTaskPriority); 
	//m_currentTaskPriority = priority;

	// lock and unlock from different thread will cause exception
	// lock multiple times in same thread will cause exception
	std::unique_lock<std::mutex> lock(m_priorityMutex);
	if (m_currentTaskPriority == priority) {
		m_currentTaskPriorityLockCnt++;
		return;
	}
	m_priorityCondition.wait(lock, [this]() { return m_currentTaskPriority == -1; });
	assert(m_currentTaskPriority == -1);
	assert(priority >= 0 && priority < TASKQUEUE_MAX_COUNT);
	m_currentTaskPriority = priority;
	m_currentTaskPriorityLockCnt = 1;
	assert(m_modbusTaskCache.getTaskCnt(m_currentTaskPriority) == 0);
}

void NCMachine::exitSetPriority()
{
	//if (m_currentTaskPriority == -1)
	//	return;
	////if (m_currentTaskPriorityLockCnt > 0) {
	////	m_currentTaskPriorityLockCnt--;
	////	return;
	////}
	////m_currentTaskPriority.store(m_currentTaskPriorityPrev);
	//m_priorityMutex.unlock();

	std::lock_guard<std::mutex> lock(m_priorityMutex);
	if (m_currentTaskPriorityLockCnt > 1) {
		m_currentTaskPriorityLockCnt--;
		return;
	}
	assert(m_currentTaskPriority != -1);
	m_modbusTaskCache.AddToModbusTask(m_modbus, m_currentTaskPriority);

	m_currentTaskPriority = -1;
	m_currentTaskPriorityLockCnt = 0;
	m_priorityCondition.notify_one();
}

int NCMachine::doTask(GCodeTask* task, TaskThread<GCodeTask>* taskThread)
{
	m_modbus->clearTaskLastRet(task->priority);

	//LOG_INFO(5, "EDM: GCode Runing, Line(after parse): " << task->lineNumber << ", Code: " << task->code);

	std::string jCode = task->jCode;
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_currentGCodeLine = task->lineNumber;

		jCode.erase(std::remove(jCode.begin(), jCode.end(), '\n'), jCode.end());
		jCode.erase(std::remove(jCode.begin(), jCode.end(), '\r'), jCode.end());

		m_currentJCode = jCode;
	}
	std::string currentGCode = m_currentGCodeLine > 0 && m_currentGCodeLine <= m_currentGCodes.length() ? EUtils::QString2StdString(m_currentGCodes[m_currentGCodeLine - 1]) : "";
	LOG_INFO(5, "EDM: GCode Runing, Line: " << m_currentGCodeLine << ", GCode: " << currentGCode << ", JCode: " << m_currentJCode);

	std::vector<std::tuple<std::function<int()>, std::string>> checkFunctions;
	try
	{
		//cb::JSON::ValuePtr json = m_gcodeTool->ParseGCode(gcode, true);
		std::string stringCode = jCode;
		cb::JSON::ValuePtr json = cb::JSON::Reader::parse(stringCode);
		if (!json.isNull()) {
			if (json->isList()) {
				cb::JSON::Value& cmds = json->getList();
				for (unsigned i = 0; i < cmds.size(); i++) {
					auto checkFunctionsI = doTaskJson(cmds.get(i), task, taskThread);
					checkFunctions.insert(std::end(checkFunctions), std::begin(checkFunctionsI), std::end(checkFunctionsI));
				}
			}
			else {
				checkFunctions = doTaskJson(json, task, taskThread);
			}
		}
	}
	catch (const std::exception& e){
		exitSetPriority();

		//int ret2 = -9;
		LOG_ERROR("EDM: " << e.what() << ", doTask " << task->jCode);
		// already append in LOG_ERROR
		//LineLogger::instance().append("ERROR:" + e.getMessage());

		if (task->postDoFunction) {
			task->postDoFunction(-9, task, this);
			LOG_INFO(8, "NCMachine: GCodeTask postDoFunction run(" << -9 << "), " << task->postDoFunctionDesc);
		}

		QMetaObject::invokeMethod(this, [this]() {
			this->StopRun();
			// task is deleted in StopRun()
			}, Qt::QueuedConnection);
	}

	// ret:
	// 1(>=1): do ok
	// -1:  do fail
	// -2: continue do
	// -9: fail, clear all 
	// -8: task abort
	for (auto it = checkFunctions.begin(); it != checkFunctions.end(); it++) {
		// 等到ReadState完成，不然可能还是上一个状态
		std::this_thread::sleep_for(std::chrono::milliseconds(TASK_WAIT_MILLSECONDS));

		std::function<int()> itFunc = std::get<0>(*it);
		std::string itFuncDesc = std::get<1>(*it);
		//int cnt = 0;
		int ret2 = 1;

#ifdef _DEBUG
		if (m_currentTaskDesc != itFuncDesc) {
			std::lock_guard<std::mutex> lock(m_mutex);
			m_currentTaskDesc.assign(itFuncDesc);
		}
#endif

		while (true)
		{
			if (m_modbus->getTaskLastRet(task->priority) == -9) {
				m_modbus->clearTaskLastRet(task->priority);
				ret2 = -9;
			}
			else
			{
				if (!taskThread->isTaskToContinue()) {
					ret2 = -8;
				}
				else {
					int ret = itFunc();

					if (ret > 0) {
						ret2 = ret;
						LOG_INFO(8, "NCMachine: GCodeTask wait function run succeed: " << itFuncDesc);
						break;
					}
					else {
						if (ret == -9) {
							ret2 = -9;
						}
						else
						{
							ret2 = -1;
							std::this_thread::sleep_for(std::chrono::milliseconds(TASK_WAIT_MILLSECONDS));
							//cnt++;
							// 某些操作耗时很长
							//if (cnt >= TIMEOUT_WAIT_COUNT)
							//	ret2 = -9;
						}
					}
				}
				if (ret2 > 0)
					break;
			}

			if (ret2 == -9 || ret2 == -8)
			{
				if (ret2 == -9)
				{
					LOG_WARNING("NCMachine: GCodeTask wait function return -9: " << itFuncDesc << ". Now state is " << m_state[0] << ", " << m_state[1]);
					//// 执行错误：
					//QString error = QStringLiteral("ERROR:%1").arg(NCMachine::GetRLSTDesc(m_state[0], m_state[1])); // .arg(gcode.replace("\n", ""));
					//LineLogger::instance().append(error);

					if (task->postDoFunction) {
						task->postDoFunction(-9, task, this);
						LOG_INFO(8, "NCMachine: GCodeTask postDoFunction run(" << -9 << "), " << task->postDoFunctionDesc);
					}

					QMetaObject::invokeMethod(this, [this]() {
						this->StopRun();
						// task is deleted in StopRun()
					}, Qt::QueuedConnection);
				}
				// ret == -8 意味着是主动取消任务
				else if (ret2 == -8) {
					if (task->postDoFunction) {
						task->postDoFunction(-8, task, this);
						LOG_INFO(8, "NCMachine: GCodeTask postDoFunction run(" << -8 << "), " << task->postDoFunctionDesc);
					}
				}
				return ret2;
			}
		}
	}

	if (task->postDoFunction) {
		task->postDoFunction(1, task, this);
		LOG_INFO(8, "NCMachine: GCodeTask postDoFunction run(" << 1 << "), " << task->postDoFunctionDesc);
	}
	return 1;
}

//, std::function<void(int, GCodeTask*, NCMachine*)> postDoFunction, std::string postDoFunctionDesc
GCodeTask* NCMachine::addTask(std::string jCode, int lineNumber, int priority)
{
	GCodeTask* task = new GCodeTask(jCode, lineNumber, priority);
	//task->postDoFunctionDesc = postDoFunctionDesc;
	m_taskThread->addTask(task, priority);
	return task;
}

void NCMachine::startTaskThread()
{
	if (!m_taskThread) {
		std::function<int(GCodeTask*, TaskThread<GCodeTask>*)> doTaskFunction = [this](GCodeTask* task, TaskThread<GCodeTask>* taskThread) {
			return this->doTask(task, taskThread);
		};
		m_taskThread = new TaskThread<GCodeTask>(doTaskFunction);
	}

	m_taskThread->startTaskThread();
}

QHash<QString, QString> NCMachine::ParseCCodeLine(QString cCode)
{
	NCMachineParametersC& pInst = NCMachineParametersC::instance();
	QHash<QString, QString> ret;
	QRegExp regex("C(\\d{3})\\s*=\\s*(.*)");
	int pos = 0;
	if ((pos = regex.indexIn(cCode, pos)) != -1) {
		ret["No"] = regex.cap(1);
		QStringList ss = regex.cap(2).split(' ', Qt::SkipEmptyParts);
		for (int i = 0; i < ss.length(); ++i) {
			if (i < PARAMETERC_COLUMN_COUNT) {
				ret[QString::fromStdString(pInst.names[i])] = ss[i];
			}
		}
	}
	return ret;
}

QString NCMachine::ParseCCode(QString gcode)
{
	QRegExp regex("C(\\d{3})\\s*=\\s*(.*)");

	NCMachineParametersC& pInst = NCMachineParametersC::instance();

	QStringList linesNew;
	QStringList lines = gcode.split("\n");
	for (QString line : lines) {
		int pos = 0;
		if ((pos = regex.indexIn(line, pos)) != -1) {
			linesNew.append(";" + line);
		}
		else {
			if (line.contains("G01") || line.contains("g01")) {
				for (int j = 1; j <= 2; ++j) {
					for (int i = 0; i < PARAMETERC_COLUMN_COUNT; i++) {
						if (pInst.namesChar[i] == '0' + j) {
							line = line.replace(pInst.re4Search[i], pInst.re4Replace[i]);
							line = line.replace(pInst.re4Search2[i], pInst.re4Replace[i]);
						}
					}
				}
			}
			linesNew.append(line);
		}
	}
	return linesNew.join('\n');
}

QString NCMachine::RunCCode(QString gcode)
{
	QRegExp regex("C(\\d{3})\\s*=\\s*(.*)");

	NCMachineParametersC& pInst = NCMachineParametersC::instance();

	QStringList linesNew;
	QStringList lines = gcode.split("\n");
	for (QString line : lines) {
		int pos = 0;
		if ((pos = regex.indexIn(line, pos)) != -1) {

			QString match = regex.cap(1);
			//gcode.remove(pos, match.length());

			if (m_paramCsNames.count() == 0) {
				//m_paramCsNames = QString("ON OFF MA  IP  SV UP DN  LN STEP PL  V  HP PP  C  S  L").split(' ', QString::SkipEmptyParts);
				// (ON OFF MA IP SV UP DN LN LP STEP PL    V HP PP C S LS AL JS JM SJ OC LD)
				for (const auto& name : pInst.names) {
					m_paramCsNames.append(QString::fromStdString(name));
				}
			}

			QString ccommand = regex.cap(2).replace(";", "");
			QStringList parts = ccommand.split(' ', Qt::SkipEmptyParts);
			if (m_paramCsNames.count() != parts.count()) {
				LOG_WARNING("EDM: Invalid C Command: " << EUtils::QString2StdString(ccommand));
				THROW(EUtils::QString2StdString(QStringLiteral("C命令参数数量和要求的不一致！")));
				linesNew.append("; Error: " + line);
				continue;
			}
			for (int i = 0; i < m_paramCsNames.length() && i < parts.length(); ++i)
			{
				if (parts[i].isEmpty())
					continue;
				QRegExp rx(QString::fromStdString(pInst.namesRegex[i]));
				QString s = parts[i].rightJustified(NCMachineParametersC::namesLen[i], '0');

				if (rx.indexIn(s) == -1) {
					LOG_WARNING("EDM: Invalid C Command: " << EUtils::QString2StdString(ccommand));
					THROW(EUtils::QString2StdString(QStringLiteral("C命令参数%1=%2不满足要求！").arg(m_paramCsNames[i]).arg(s)));
					linesNew.append("; Error: " + line);
					continue;
				}
			}

			m_paramCs[match.toInt()] = parts;
			
			LOG_INFO(5, "EDM: C Command(" << EUtils::QString2StdString(match) << "): " << EUtils::QString2StdString(ccommand));

			linesNew.append(";" + line);
		}
		else {
			if (line.contains("G01") || line.contains("g01")) {
				for (int j = 1; j <= 2; ++j) {
					for (int i = 0; i < PARAMETERC_COLUMN_COUNT; i++) {
						if (pInst.namesChar[i] == '0' + j) {
							line = line.replace(pInst.re4Search[i], pInst.re4Replace[i]);
							line = line.replace(pInst.re4Search2[i], pInst.re4Replace[i]);
						}
					}
				}
				// QRegularExpression regex2("HP(\\d+)");
				//line = line.replace(regex2, "J\\1");
			}
			
			linesNew.append(line);
		}
	}
	return linesNew.join('\n');
}

QString NCMachine::ParseHCode(QString gcode)
{
	return gcode;

	//QRegularExpression regex("H(\\d+)\\s+=\\s+[\\+\\-]?([\\d\\.]+)");

	//QStringList linesNew;
	//QStringList lines = gcode.split("\n");
	//for (QString line : lines) {
	//	QRegularExpressionMatchIterator i = regex.globalMatch(gcode);
	//	if (i.hasNext()) {
	//		if (!onlyParse) {
	//			while (i.hasNext()) {
	//				QRegularExpressionMatch match = i.next();
	//				int h = match.captured(1).toInt();
	//				double v = this->GetController()->get("_h" + std::to_string(h));
	//			}
	//		}
	//	}
	//	else
	//	{
	//		linesNew.append(line);
	//	}
	//}
	//return linesNew.join('\n');
}
QString NCMachine::RunHCode(QString gcode)
{
	return ParseHCode(gcode);
}

bool NCMachine::RunGCode(QString gcode, std::function<void()> functionDoFinished)
{
	//if (IsGCodeRunning()) {
	//	QThread::create([this, gcode, functionDoFinished, functionDoPerLineBefore, immediate, keepControllerState] {
	//		WaitUntilRunFinished();
	//		RunGCodeSync(gcode, functionDoFinished, functionDoPerLineBefore, immediate, keepControllerState);
	//	})->start();
	//	EUtils::sleep(1000);
	//	return;
	//}
	return RunGCodeAsync(gcode, functionDoFinished);
}

QString NCMachine::PreprocessGCode(QString gcode)
{
	//if (gcode.contains("o900 call")) {
	//}
	return gcode;
}
bool NCMachine::RunGCodeSync(QString gcode)
{
	//if (IsGCodeRunning() || m_nowOperating > 0) {
	//	return false;
	//}
	if (m_gcodeTool->IsRunning() || m_nowOperating > 0) {
		return false;
	}
	try {
		//m_realtimeJsonMachine2->setTaskToContinue(true);
		//m_realtimeJsonMachine2->setCurrentPriority(0);
		m_realtimeJsonMachine2->setSimpleMode(true);
		return this->m_gcodeTool->RunGCode(m_realtimeJsonMachine2, EUtils::QString2StdString(gcode));
		//m_realtimeJsonMachine2->setSimpleMode(false);
	}
	catch (const std::exception& e) {
		LOG_ERROR("EDM: " << e.what() << ", RunGCodeSync " << EUtils::QString2StdString(gcode));
		return false;
	}
}

bool NCMachine::RunGCodeAsync(QString gcode, std::function<void()> functionDoFinished)
{
	// dont't toUpper because now 'H' is special meaning, like in 'DO_MESSAGE_YN TO_CLEAR_MACHINE_AXIS', it will be parsed as 'DO_MESSAGE_YN TO_CLEAR_MAC'
	//gcode = gcode.toUpper();

	//, std::function<void(cb::JSON::ValuePtr, int)> functionDoPerLineBefore, 
	bool immediate = false;
	bool keepControllerState = false;
	if (IsGCodeRunning() || m_nowOperating > 0) {
		// dont wait because it will block
		//WaitUntilRunFinished();
		return false;
	}

	try {
		gcode = RunCCode(gcode);
		gcode = RunHCode(gcode);
	}
	catch (const std::exception& e) {
		LOG_ERROR("EDM: " << e.what() << ", RunGCodeAsync " << EUtils::QString2StdString(gcode));
		if (functionDoFinished) {
			functionDoFinished();
		}
		return false;
	}

	if (keepControllerState) {
		gcode = "M70\n" + gcode + "M72\n";
	}
	
	m_currentGCodes = gcode.split('\n');
	//auto json = m_gcodeTool->ParseGCode(gcode, false);

	// also do enterSetPriority in doTask(), for modbus
	enterSetPriority(immediate ? 0 : 1);

	PropertyObjects::getInstance()->propertyObjectFdwc->setfdwcms(0);
	SystemSettings::instance().DeleteValue("LEJL");
	SystemSettings::instance().DeleteValue("LEJS");

	// Even if task is run and finished, the get_id() is not 0
	if (m_realtimeJsonMachineThread.get_id() != std::thread::id())
	{
		m_realtimeJsonMachine->setTaskToContinue(false);
		if (m_realtimeJsonMachineThread.joinable()) {
			m_realtimeJsonMachineThread.join();
		}
	}
	m_realtimeJsonMachine->setCurrentPriority(m_currentTaskPriority);
	m_realtimeJsonMachine->setTaskToContinue(true);

	m_g01Data.workStartDate = QDateTime::currentDateTime(); 
	m_g01Data.workName = SystemSettings::instance().LastRunNCFileName;
	m_g01Data.workDetails.clear();
	m_g01Data.allWorkSeconds = 0;
	m_g01Data.nowWorkSeconds = 0;
	m_g01Data.lastWorkSeconds = 0;
	m_g01Data.lastPosSet = false;
	m_g01Data.lastPosList.clear();

	if (m_realtimeJsonMachineThread.get_id() == std::thread::id()) {
		m_realtimeJsonMachineThread = std::thread([this, gcode, functionDoFinished]() {
			this->m_gcodeTool->RunGCode(m_realtimeJsonMachine, EUtils::QString2StdString(gcode));

			this->serialize();
			if (functionDoFinished) {
				functionDoFinished();
			}

			SaveWorkTimeToDb();

			m_realtimeJsonMachine->initFunctions();
			m_realtimeJsonMachineThreadDone = true;

			if (m_g01Data.powerOffAfterDone && m_g01Data.allWorkSeconds > 0 && m_realtimeJsonMachine->isTaskToContinue())
			{
				this->PowerOff();
			}
		});
		m_realtimeJsonMachineThreadDone = false;
	}
	else {
		LineLogger::instance().append(QStringLiteral("ERROR:上一个程序还在运行"));
		if (functionDoFinished) {
			functionDoFinished();
		}
	}

	exitSetPriority();

	return true;
}

void NCMachine::SaveWorkTimeToDb()
{
	if (m_g01Data.workDetails.length() == 0) {
		return;
	}
	m_g01Data.workEndDate = QDateTime::currentDateTime();

	try {
		long allWorkSeconds = m_g01Data.allWorkSeconds;
		allWorkSeconds = 0;
		for (int i = 0; i < m_g01Data.workDetails.length(); i++) {
			allWorkSeconds += std::get<3>(m_g01Data.workDetails[i]);
		}
		qint64 parentId = JiaGongJiLuForm::addRecordWork(m_g01Data.workName, m_g01Data.workStartDate, m_g01Data.workEndDate, allWorkSeconds);
		if (parentId != -1) {
			for (int i = 0; i < m_g01Data.workDetails.length(); i++)
			{
				JiaGongJiLuForm::addRecordWorkDetail(std::get<0>(m_g01Data.workDetails[i]), std::get<1>(m_g01Data.workDetails[i]), std::get<2>(m_g01Data.workDetails[i]), std::get<3>(m_g01Data.workDetails[i]), parentId);
			}
		}
	}
	catch (const std::exception& e) {
		LOG_ERROR("EDM: " << e.what() << ", SaveWorkTimeToDb ");
	}
}

//void NCMachine::RunGCode(QString gcode, std::function<void(int, GCodeTask*, NCMachine*)> function, bool immediate, bool keepControllerState)
//{
//	m_lastErrorMessage.clear();
//
//	if (keepControllerState) {
//		gcode = "M70\n" + gcode + "M72\n";
//	}
//	auto json = m_gcodeTool->ParseGCode(gcode, false);
//
//	enterSetPriority(immediate ? 0 : 1);
//
//	addTask("api", -1, m_currentTaskPriority, NULL);
//
//	if (!json.isNull()) {
//		if (json->isList()) {
//			cb::JSON::Value& cmds = json->getList();
//			for (unsigned i = 0; i < cmds.size(); i++) {
//				if (cmds[i]->getString("type") == "GCode") {
//					addTask(QString::fromStdString(cmds[i]->getString("value")), i, m_currentTaskPriority, i == cmds.size() - 1 ? function : NULL);
//				}
//				else {
//					LOG_ERROR("undefined gcode: type=" << cmds[i]->getString("type"));
//				}
//			}
//		}
//		else {
//			LOG_ERROR("ParseGCode should return list");
//		}
//	}
//
//	//for (int i = 0; i < blocks.count(); ++i)
//	//{
//	//	addTask(blocks[i], i, m_currentTaskPriority, i == blocks.count() - 1 ? function : NULL);
//	//}
//	addTask("jog", -1, m_currentTaskPriority, NULL);
//	
//	exitSetPriority();
//}

void NCMachine::StopRun()
{
	m_nowOperating = 0;

	if (m_g01Data.isRunning) {
		m_dwellEndTime = QDateTime::currentDateTime();
		m_g01Data.isRunning = false;

		// 正常加工的时候直接停止
		if (!m_g01Data.isPausing) {
			// 正常不会到这里，只能暂停后再取消
			m_g01Data.lastWorkSeconds = m_g01Data.nowWorkSeconds + m_g01Data.startTime.secsTo(QDateTime::currentDateTime());
			m_g01Data.allWorkSeconds += m_g01Data.startTime.secsTo(QDateTime::currentDateTime());
			m_g01Data.nowWorkSeconds = 0;

			//m_g01Data.allWorkSeconds += m_g01Data.startTime.secsTo(QDateTime::currentDateTime());
			//m_g01Data.nowWorkSeconds += m_g01Data.startTime.secsTo(QDateTime::currentDateTime());
		}
		else {
			m_g01Data.lastWorkSeconds = m_g01Data.nowWorkSeconds;
		}

		if (m_g01Data.workDetails.count() > 0) {
			std::tuple<QString, QDateTime, QDateTime, int>& lastItem = m_g01Data.workDetails[m_g01Data.workDetails.count() - 1];
			std::get<2>(lastItem) = QDateTime::currentDateTime();
			std::get<3>(lastItem) = m_g01Data.lastWorkSeconds;
		}
	}

	if (m_realtimeJsonMachineThread.get_id() != std::thread::id())
	{
		m_realtimeJsonMachine->setTaskToContinue(false);
		if (m_realtimeJsonMachineThread.joinable()) {
			m_realtimeJsonMachineThread.join();
		}
		m_realtimeJsonMachineThreadDone = true;
	}
	else
	{
		return;
	}

	LOG_INFO(8, "Modbus: clearTasks");
	for (int i = 0; i < 2; ++i) {
		m_modbus->clearTasks(i);
	}

	// in gcodet task, maybe there is some modbus task to add which will do after clearTask
	// 例如SV的值设为3，开机待机状态、加工停止后都需要设定
	LOG_INFO(8, "NCMachine: clearTasks");
	for (int i = 0; i < 2; ++i){
		this->clearTasks(i);
	}

	enterSetPriority(0);

	PropertyObjects::getInstance()->propertyObjectFdjg->ExecuteCmdsSetSv(this, 3);
	PropertyObjects::getInstance()->propertyObjectFdwc->setfdwcms(0);

	PropertyObjects::getInstance()->propertyObjectFuzhuIO->setyb1(false);
	PropertyObjects::getInstance()->propertyObjectFuzhuIO->setyb2(false);
	PropertyObjects::getInstance()->propertyObjectFuzhuIO->ExecuteCmds(this);

	PropertyObjects::getInstance()->propertyObjectAJC->setajc(9);
	PropertyObjects::getInstance()->propertyObjectAJC->ExecuteCmds(this);

	DataForm* dataForm3 = DataForms::getInstance()->getDataForm("xitongshezhi3");
	int v = dataForm3->getValue("TCQHTJL").toInt();
	PropertyObjects::getInstance()->propertyObjectYd->settcqhtjl(v);

	GoApi();
	GoJog();
	exitSetPriority();

	m_taskThread->resumeTaskThread();
}

void NCMachine::PauseRun()
{
	if (m_g01Data.isRunning) {
		if (!m_g01Data.isPausing) {
			// 如果此时modbus任务队列里正好运行到检测加工停止状态，可能会导致加工完成进行到下一段
			m_modbus->getTaskThread()->pauseTaskThread();
			std::this_thread::sleep_for(std::chrono::milliseconds(TASK_WAIT_MILLSECONDS_LARGE));

			m_g01Data.isPausing = true;
			m_g01Data.allWorkSeconds += m_g01Data.startTime.secsTo(QDateTime::currentDateTime());
			m_g01Data.nowWorkSeconds += m_g01Data.startTime.secsTo(QDateTime::currentDateTime());
			m_g01Data.startTime = QDateTime::currentDateTime();
			m_g01Data.lastPosSet = false;
			m_g01Data.lastPosList.clear();


			enterSetPriority(0);
			PropertyObjects::getInstance()->propertyObjectJog->ExecuteCmdsFdzlw(this, true);

			// Pos 需要在任务队列里设置，而不能直接在这里设置，因为任务队列里的命令（上一个停止加工命令会改变坐标）可能改变Pos
			std::function<void()> doFuncWait = [this]() {
				assert(m_state[2] == NCT_STATE_API_RUN);
				m_g01Data.lastPos = m_pos;
				m_g01Data.lastPosSet = true;
				LOG_INFO(8, "NCMachine: lastPos = " << m_pos);
				};
			executeCmdWait(convertWaitFunction(waitno(doFuncWait)), "set lastPos for pause");

			// 直接回到G01开始时候的位置
			int axis[] = { TMBS_MAP0_ID_XPOS, TMBS_MAP0_ID_YPOS, TMBS_MAP0_ID_ZPOS };
			int value[] = { m_g01Data.startPos.x(), m_g01Data.startPos.y(), m_g01Data.startPos.z() };
			Loc(axis, value, 3);

			GoJog();

			m_g01Data.lastSV = PropertyObjects::getInstance()->propertyObjectFdjg->fdjgcfdy();
			PropertyObjects::getInstance()->propertyObjectFdjg->ExecuteCmdsSetSv(this, 3);

			m_g01Data.lastYb1 = PropertyObjects::getInstance()->propertyObjectFuzhuIO->yb1();
			m_g01Data.lastYb2 = PropertyObjects::getInstance()->propertyObjectFuzhuIO->yb2();
			PropertyObjects::getInstance()->propertyObjectFuzhuIO->setyb1(false);
			PropertyObjects::getInstance()->propertyObjectFuzhuIO->setyb2(false);
			PropertyObjects::getInstance()->propertyObjectFuzhuIO->ExecuteCmds(this);

			exitSetPriority();

			m_modbus->getTaskThread()->resumeTaskThread();
			LOG_INFO(8, "NCMachine: PauseRun, set lastPos after goApi, lastSV=" << m_g01Data.lastSV);
		}
	}
	m_taskThread->pauseTaskThread();
}

void NCMachine::ResumeRun()
{
	if (m_g01Data.isRunning) {
		if (m_g01Data.isPausing) {
			if (!m_g01Data.lastPosSet) {
				LOG_INFO(8, "NCMachine: ResumeRun, but lastPos not set yet");
				return;
			}
			LOG_INFO(8, "NCMachine: ResumeRun, LocTo LastPos4Pause, setSV=" << m_g01Data.lastSV);

			m_g01Data.isPausing = false;
			m_g01Data.startTime = QDateTime::currentDateTime();

			enterSetPriority(0);
			GoApi();

			// 只有doTask中才会有StopRun，一般情况下Loc下出错不会StopRun，而是自己处理
			executeCmdWait(convertWaitFunction(waitno([this]() {
				this->doWhenWaitFail = [this]() {
					QMetaObject::invokeMethod(this, [this]() {
						this->StopRun();
						// task is deleted in StopRun()
						}, Qt::QueuedConnection);
					};
				})), "set doWhenWaitFail func");

			// 需要在任务队列里设置，而不能直接在这里执行，因为任务队列里的命令可能改变Pos
			// Pause后马上Resume，会导致lastPos此时还未设置，所以需要等待设置lastPos的命令完成
			// 放到队列中也不行，会导致Loc命令生成的Modbus命令会放到队列末尾

			int axis[] = { TMBS_MAP0_ID_XPOS, TMBS_MAP0_ID_YPOS, TMBS_MAP0_ID_ZPOS };
			for (int i = m_g01Data.lastPosList.size() - 1; i >= 0; i--)
			{
				if (i == 0 && m_g01Data.lastPos == m_g01Data.lastPosList[i])
					continue;
				int value[] = { m_g01Data.lastPosList[i].x(), m_g01Data.lastPosList[i].y(), m_g01Data.lastPosList[i].z() };
				Loc(axis, value, 3);
			}
			m_g01Data.lastPosList.clear();

			PropertyObjects::getInstance()->propertyObjectLoc->setV(DataForms::getInstance()->getDataForm("xitongshezhi4")->getValue("ZTHQSDSD").toInt());
			int value[] = { m_g01Data.lastPos.x(), m_g01Data.lastPos.y(), m_g01Data.lastPos.z() };
			// to generate error
			//value[0] = 1234;
			Loc(axis, value, 3);
			m_g01Data.lastPosSet = false;
			PropertyObjects::getInstance()->propertyObjectLoc->setV(DataForms::getInstance()->getDataForm("xitongshezhi2")->getValue("SPEED_LOC_V").toInt());

			//int axis[] = { TMBS_MAP0_ID_XPOS, TMBS_MAP0_ID_YPOS, TMBS_MAP0_ID_ZPOS };
			//int value[] = { m_g01Data.lastPos.x(), m_g01Data.lastPos.y(), m_g01Data.lastPos.z() };
			//Loc(axis, value, 3);

			PropertyObjects::getInstance()->propertyObjectFdjg->ExecuteCmdsSetSv(this, m_g01Data.lastSV);
			PropertyObjects::getInstance()->propertyObjectJog->ExecuteCmdsFdzlw(this, false);

			PropertyObjects::getInstance()->propertyObjectFuzhuIO->setyb1(m_g01Data.lastYb1);
			PropertyObjects::getInstance()->propertyObjectFuzhuIO->setyb2(m_g01Data.lastYb2);
			PropertyObjects::getInstance()->propertyObjectFuzhuIO->ExecuteCmds(this);

			executeCmdWait(convertWaitFunction(waitno([this]() {
				this->doWhenWaitFail = NULL;
				})), "clear doWhenWaitFail");

			exitSetPriority();
		}
	}
	m_taskThread->resumeTaskThread();
}

int NCMachine::WaitUntilRunFinished()
{
	while (IsGCodeRunning())
	{
		EUtils::sleep(1000);
	}
	if (!m_realtimeJsonMachine->isTaskToContinue()) {
		return -1;
	}
	return 1;
}

bool NCMachine::serialize()
{
	m_gcodeTool->serialize();

	std::lock_guard<std::mutex> lock(m_mutex);
	QJsonObject jsonObj;

	//jsonObj["gcode"] = m_currentGCodes;
	jsonObj["currentLine"] = m_currentGCodeLine;
	jsonObj["currentJCode"] = QString::fromStdString(m_currentJCode);

	QJsonDocument jsonDoc(jsonObj);
	NFile file("data/ncmachine.json");
	if (file.open(QIODevice::WriteOnly)) {
		file.write(jsonDoc.toJson());
		file.close();
	}

	return true;
}

bool NCMachine::deserialize()
{
	QFile file("data/ncmachine.json");
	if (!file.open(QIODevice::ReadOnly)) {
		return false;
	}

	QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
	QJsonObject jsonObj = jsonDoc.object();

	QString str = jsonObj["gcode"].toString();

	return true;
}
