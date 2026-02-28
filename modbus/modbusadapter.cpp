#include <thread>
#include <chrono>
#include <sstream>
#include <iostream>
#include <exception>
#include <QApplication>
#include <QtDebug>
#include "modbusadapter.h"
#include <errno.h>
#include <cbang/log/Logger.h>
#include "utils/TaskThread.cpp"
//template class TaskThread<ModbusTask>;
#include <random>
#include <algorithm>
#include "NCMachineProperties.h"

ModbusAdapter* s_instance;

#define DATA_BUFFER_LEN 2000
#define DATA16_BUFFER_LEN 125

ModbusAdapter::ModbusAdapter(QObject* parent, RegistersModel* regModel, RawDataModel* rawdataModel) :
	QObject(parent), m_taskThread(NULL),
	m_modbus(NULL), m_regModel(regModel), m_rawModel(rawdataModel),
	m_useThread(true), m_connected(false), m_packets(0), m_errors(0), m_currentGuiTask(0, 0, 0, 0), m_timerCount(0)
{
	s_instance = this;

	m_ModBusMode = EUtils::None;
	m_pollTimer = new QTimer(this);

	connect(m_pollTimer, SIGNAL(timeout()), this, SLOT(modbusTransaction()));
	//connect(m_regModel, SIGNAL(refreshView()), this, SLOT(refreshView()));

	//setup memory for data
	readDataDest = (uint8_t*)malloc(DATA_BUFFER_LEN * sizeof(uint8_t));
	if (readDataDest)
		memset(readDataDest, 0, DATA_BUFFER_LEN * sizeof(uint8_t));
	readDataDest16 = (uint16_t*)malloc(DATA16_BUFFER_LEN * sizeof(uint16_t));
	if (readDataDest16)
		memset(readDataDest16, 0, DATA16_BUFFER_LEN * sizeof(uint16_t));
	/*writeDataDest = (uint8_t*)malloc(DATA_BUFFER_LEN * sizeof(uint8_t));
	memset(writeDataDest, 0, DATA_BUFFER_LEN * sizeof(uint8_t));*/
	writeDataDest16 = (uint16_t*)malloc(DATA16_BUFFER_LEN * sizeof(uint16_t));
	if (writeDataDest16)
		memset(writeDataDest16, 0, DATA16_BUFFER_LEN * sizeof(uint16_t));
}

ModbusAdapter::~ModbusAdapter()
{
	if (m_taskThread) {
		m_taskThread->stopTaskThread();
		delete m_taskThread;
	}

	if (m_connected) {
		modbusDisConnect();
	}

	free(readDataDest);
	free(readDataDest16);
	//free(writeDataDest);
	free(writeDataDest16);
}

void ModbusAdapter::modbusConnectRTU(QString port, int baud, QChar parity, int dataBits, int stopBits, int RTS, int responseTimeOut, int byteTimeout)
{
	//Modbus RTU connect

	modbusDisConnect();

	//QLOG_INFO()<<  "Modbus Connect RTU";

	m_modbus = modbus_new_rtu(port.toLatin1().constData(), baud, parity.toLatin1(), dataBits, stopBits);  // RTS

	//QLOG_TRACE() <<  line;

	//Debug messages from libmodbus
#ifdef LIB_MODBUS_DEBUG_OUTPUT
	modbus_set_debug(m_modbus, 1);
#endif

	if (m_modbus == NULL) {
		//mainWin->showUpInfoBar(tr("Unable to create the libmodbus context."), InfoBar::Error);
		//QLOG_ERROR()<<  "Connection failed. Unable to create the libmodbus context";
		return;
	}
	else if (m_modbus && modbus_set_slave(m_modbus, DEFAULT_MODBUS_SLAVE) == -1) {
		modbus_free(m_modbus);
		m_modbus = NULL;
		//mainWin->showUpInfoBar(tr("Invalid slave ID."), InfoBar::Error);
		//QLOG_ERROR()<<  "Connection failed. Invalid slave ID";
		return;
	}
#if HAVE_DECL_TIOCM_RTS
	else if (m_modbus && modbus_rtu_set_rts(m_modbus, RTS) == -1) {
		modbus_free(m_modbus);
		m_modbus = NULL;
		return;
	}
#endif
	else if (m_modbus && modbus_connect(m_modbus) == -1) {
		modbus_free(m_modbus);
		m_modbus = NULL;
		//mainWin->showUpInfoBar(tr("Connection failed\nCould not connect to serial port."), InfoBar::Error);
		//QLOG_ERROR()<<  "Connection failed. Could not connect to serial port";
		m_connected = false;
	}
	else {
		//error recovery mode
		//modbus_set_error_recovery(m_modbus, MODBUS_ERROR_RECOVERY_PROTOCOL);
		modbus_set_error_recovery(m_modbus, MODBUS_ERROR_RECOVERY_NONE);
		//response_timeout;
		modbus_set_response_timeout(m_modbus, 0, responseTimeOut * 1000);
		modbus_set_byte_timeout(m_modbus, 0, byteTimeout * 1000);

		m_connected = true;

		//mainWin->hideInfoBar();
		//QLOG_TRACE() << line;
	}

	m_ModBusMode = EUtils::RTU;

	if (m_rawModel != NULL) {
		//Add line to raw data model
		QString line;
		line = "Connecting to Serial Port [" + port + "]...";
		if (m_connected)
			line += "OK";
		else
			line += "Failed";
		line = EUtils::SysTimeStamp() + line;
		m_rawModel->addLine(line);
	}
}

void ModbusAdapter::modbusConnectTCP(QString ip, int port, int timeOut)
{
	//Modbus TCP connect
	QString strippedIP = "";

	modbusDisConnect();

	//QLOG_INFO()<<  "Modbus Connect TCP";


	//QLOG_TRACE() <<  line;
	strippedIP = stripIP(ip);
	if (strippedIP == "") {
		//mainWin->showUpInfoBar(tr("Connection failed\nBlank IP Address."), InfoBar::Error);
		//QLOG_ERROR()<<  "Connection failed. Blank IP Address";
		return;
	}
	else {
		m_modbus = modbus_new_tcp(strippedIP.toLatin1().constData(), port);
		//mainWin->hideInfoBar();
		//QLOG_TRACE() <<  "Connecting to IP : " << ip << ":" << port;
	}

	//Debug messages from libmodbus
#ifdef LIB_MODBUS_DEBUG_OUTPUT
	modbus_set_debug(m_modbus, 1);
#endif

	if (m_modbus == NULL) {
		//mainWin->showUpInfoBar(tr("Unable to create the libmodbus context."), InfoBar::Error);
		//QLOG_ERROR()<<  "Connection failed. Unable to create the libmodbus context";
		return;
	}
	else if (m_modbus && modbus_connect(m_modbus) == -1) {
		modbus_free(m_modbus);
		m_modbus = NULL;
		//mainWin->showUpInfoBar(tr("Connection failed\nCould not connect to TCP port."), InfoBar::Error);
		//QLOG_ERROR()<<  "Connection to IP : " << ip << ":" << port << "...failed. Could not connect to TCP port";
		m_connected = false;
	}
	else {
		//error recovery mode
		modbus_set_error_recovery(m_modbus, MODBUS_ERROR_RECOVERY_PROTOCOL);
		//response_timeout;
		modbus_set_response_timeout(m_modbus, timeOut, 0);
		m_connected = true;
		//mainWin->hideInfoBar();
		//QLOG_TRACE() << line;
	}

	m_ModBusMode = EUtils::TCP;

	if (m_rawModel != NULL) {
		//Add line to raw data model
		QString line;
		line = "Connecting to IP : " + ip + ":" + QString::number(port);
		if (m_connected)
			line += "OK";
		else
			line += "Failed";
		line = EUtils::SysTimeStamp() + line;
		m_rawModel->addLine(line);
	}
}

void ModbusAdapter::modbusDisConnect()
{
	//Modbus disconnect

	//QLOG_INFO()<<  "Modbus disconnected";
	resetCounters();

	if (m_modbus) {
		if (m_connected) {
			modbus_close(m_modbus);
			modbus_free(m_modbus);
		}
		m_modbus = NULL;
	}

	m_connected = false;

	m_ModBusMode = EUtils::None;
}

void ModbusAdapter::fillWriteData(int functionCode, int numOfRegs)
{
	if (m_regModel == NULL)
		return;

	switch (functionCode)
	{
	case MODBUS_FC_WRITE_SINGLE_COIL:
		writeDataDest16[0] = m_regModel->value(0);
		break;

	case MODBUS_FC_WRITE_SINGLE_REGISTER:
		writeDataDest16[0] = m_regModel->value(0);
		break;

	case MODBUS_FC_WRITE_MULTIPLE_COILS:
	{
		assert(numOfRegs <= DATA16_BUFFER_LEN);
		for (int i = 0; i < numOfRegs; ++i)
		{
			writeDataDest16[i] = m_regModel->value(i);
		}
		break;
	}
	case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
	{
		assert(numOfRegs <= DATA16_BUFFER_LEN);
		for (int i = 0; i < numOfRegs; ++i)
		{
			writeDataDest16[i] = m_regModel->value(i);
		}
		break;
	}

	default:
		break;
	}
}

void ModbusAdapter::modbusTransaction()
{
	if (!m_taskThread || m_taskThread->getTasksCnt(TASK_TIMER_PRIORITY) > 100)
		return;

	m_timerCount++;

	for (std::tuple<ModbusTask, int, bool> taskIt : m_tasksNormal) {
		ModbusTask task = std::get<0>(taskIt);
		int timeInterval = std::get<1>(taskIt);
		bool showInGUI = std::get<2>(taskIt);

		int timeX = timeInterval / m_scanRate;
		if (timeX == 0 || m_timerCount % timeX != 0)
			continue;

		int functionCode = task.functionCode;
		int startAddr = task.startAddr;
		int numOfRegs = task.numOfRegs;

		if (EUtils::ModbusIsWriteFunction(functionCode)) {
			assert(false);
		}
		else {
			std::function<void(int, ModbusTask*, ModbusAdapter*)> function = [task, showInGUI](int ret, ModbusTask* taskI, ModbusAdapter* adapter) {
				if (ret == -1)
					return;
				task.postDoFunction(ret, taskI, adapter);
				if (showInGUI) {
					adapter->m_currentGuiTask = task;
					QMetaObject::invokeMethod(adapter, "updateState", Qt::QueuedConnection, Q_ARG(int, ret));
				}
			};

			ModbusTask* newtask = new ModbusTask(DEFAULT_MODBUS_SLAVE, DEFAULT_MODBUS_READ_FUNCTION, startAddr, numOfRegs);
			newtask->setPostFunction(function, "Normal Task to update GUI");
			addTask(newtask, TASK_TIMER_PRIORITY);
		}

		continue;


		//Modbus request data
		//QLOG_INFO() <<  "Modbus Transaction. Function Code = " << m_functionCode;
		//QApplication::setOverrideCursor(Qt::WaitCursor);

		//int ret = -1;
		//try {
		//  switch (functionCode)
		//  {
		//  case MODBUS_FC_READ_COILS:
		//  case MODBUS_FC_READ_DISCRETE_INPUTS:
		//  case MODBUS_FC_READ_HOLDING_REGISTERS:
		//  case MODBUS_FC_READ_INPUT_REGISTERS:
		//    ret = modbusReadDataRaw(slave, functionCode, startAddr, numOfRegs);
		//    afterModbusReadData(ret, task);
		//    break;

		//  case MODBUS_FC_WRITE_SINGLE_COIL:
		//  case MODBUS_FC_WRITE_SINGLE_REGISTER:
		//  case MODBUS_FC_WRITE_MULTIPLE_COILS:
		//  case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
		//    fillWriteData(functionCode, numOfRegs);
		//    ret = modbusWriteDataRaw(slave, functionCode, startAddr, numOfRegs, writeDataDest16);
		//    afterModbusWriteData(ret, task);
		//    break;
		//  default:
		//    break;
		//  }
		//}
		//catch (std::exception& e) {
		//  LOG_WARNING(e.what());
		//}
		//catch (...) {
		//}

		////QApplication::setOverrideCursor(Qt::ArrowCursor);

		//emit(refreshView());
	}
}

int ModbusAdapter::modbusReadDataRaw(int slave, int functionCode, int startAddr, int numOfRegs, uint16_t* readData)
{
	if (m_modbus == NULL)
		return -1;

	// Test condition when modbus read error
	//std::mt19937 engine(std::time(nullptr));
	//std::uniform_real_distribution<> realDistribution(0.0, 1.0);
	//double randomNumberReal = realDistribution(engine);
	//if (randomNumberReal < 0.1) {
	//	m_errors += 1;
	//	modbus_flush(m_modbus); //flush data
	//	return -1;
	//}

	QTime time1 = QTime::currentTime();
	m_packets += 1;
	int ret = -1; //return value from read functions
	bool is16Bit = false;

	modbus_set_slave(m_modbus, slave);
	//request data from modbus
	switch (functionCode)
	{
	case MODBUS_FC_READ_COILS:
		assert(numOfRegs <= DATA_BUFFER_LEN);
		ret = modbus_read_bits(m_modbus, startAddr, numOfRegs, readDataDest);
		break;

	case MODBUS_FC_READ_DISCRETE_INPUTS:
		assert(numOfRegs <= DATA_BUFFER_LEN);
		ret = modbus_read_input_bits(m_modbus, startAddr, numOfRegs, readDataDest);
		break;

	case MODBUS_FC_READ_HOLDING_REGISTERS:
		assert(numOfRegs <= DATA16_BUFFER_LEN);
		ret = modbus_read_registers(m_modbus, startAddr, numOfRegs, readDataDest16);
		is16Bit = true;
		break;

	case MODBUS_FC_READ_INPUT_REGISTERS:
		assert(numOfRegs <= DATA16_BUFFER_LEN);
		ret = modbus_read_input_registers(m_modbus, startAddr, numOfRegs, readDataDest16);
		is16Bit = true;
		break;
	case MODBUS_FC_READ_FILE_RECORD:
		ret = modbus_read_file_record(m_modbus, DEFAULT_MODBUS_FILE_ADDRESS, startAddr, numOfRegs, readDataDest16);
		is16Bit = true;
		break;
	default:
		break;
	}

	if (ret == numOfRegs) {
		if (readData != NULL) {
			for (int i = 0; i < numOfRegs; ++i)
			{
				if (is16Bit) {
					readData[i] = readDataDest16[i];
				}
				else {
					readData[i] = readDataDest[i];
				}
			}
		}
	}
	else {
		m_errors += 1;
		modbus_flush(m_modbus); //flush data
	}

	QTime time2 = QTime::currentTime();
	m_msecComm += time1.msecsTo(time2);

	//std::random_device rd;
	//std::mt19937 gen(rd());
	//std::uniform_int_distribution<short> distrib(-256, 255);
	//std::generate(readDataDest, readDataDest + DATA_BUFFER_LEN, [&]() { return distrib(gen); });
	//std::uniform_int_distribution<short> distrib2(-32768, 32767);
	//std::generate(readDataDest16, readDataDest16 + DATA16_BUFFER_LEN, [&]() { return distrib2(gen); });
	////ret = -1;
	////memset(readDataDest, 0, DATA_BUFFER_LEN * sizeof(uint8_t));
	////memset(readDataDest16, 0, DATA16_BUFFER_LEN * sizeof(uint16_t));

	return ret;
}

int ModbusAdapter::modbusWriteDataRaw(int slave, int functionCode, int startAddr, int numOfRegs, uint16_t* writeData)
{
	//std::random_device rd;
	//std::mt19937 gen(rd());
	//std::uniform_int_distribution<short> distrib2(-32768, 32767);
	//std::generate(writeDataDest16, writeDataDest16 + DATA16_BUFFER_LEN, [&]() { return distrib2(gen); });

	if (m_modbus == NULL)
		return -1;

	// Test condition when modbus write error
	//std::mt19937 engine(std::time(nullptr));
	//std::uniform_real_distribution<> realDistribution(0.0, 1.0);
	//double randomNumberReal = realDistribution(engine);
	//if (randomNumberReal < 0.1) {
	//	m_errors += 1;
	//	modbus_flush(m_modbus); //flush data
	//	return -1;
	//}

	QTime time1 = QTime::currentTime();
	m_packets += 1;
	int ret = -1; //return value from functions

	modbus_set_slave(m_modbus, slave);

	switch (functionCode)
	{
	case MODBUS_FC_WRITE_SINGLE_COIL:
	{
		uint8_t data = (uint8_t)writeData[0];
		ret = modbus_write_bit(m_modbus, startAddr, data);
		numOfRegs = 1;
		break;
	}
	case MODBUS_FC_WRITE_SINGLE_REGISTER:
	{
		ret = modbus_write_register(m_modbus, startAddr, writeData[0]);
		numOfRegs = 1;
		break;
	}
	case MODBUS_FC_WRITE_MULTIPLE_COILS:
	{
		uint8_t* data = new uint8_t[numOfRegs];
		for (int i = 0; i < numOfRegs; ++i)
		{
			data[i] = writeData[i];
		}
		ret = modbus_write_bits(m_modbus, startAddr, numOfRegs, data);
		delete[] data;
		break;
	}
	case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
	{
		ret = modbus_write_registers(m_modbus, startAddr, numOfRegs, writeData);
		break;
	}
	case MODBUS_FC_WRITE_FILE_RECORD:
	{
		ret= modbus_write_file_record(m_modbus, DEFAULT_MODBUS_FILE_ADDRESS, startAddr, numOfRegs, writeData);
		break;
	}

	default:
		break;
	}

	if (ret == numOfRegs)
	{
	}
	else
	{
		m_errors += 1;
		modbus_flush(m_modbus); //flush data
	}

	QTime time2 = QTime::currentTime();
	m_msecComm += time1.msecsTo(time2);

	return ret;
}

ModbusTask* ModbusAdapter::getTaskWriteFile(int subAddr, int nb, std::string writeData)
{
	ModbusTask* task = new ModbusTask(DEFAULT_MODBUS_SLAVE, MODBUS_FC_WRITE_FILE_RECORD, subAddr, nb, writeData);
	return task;
}

ModbusTask* ModbusAdapter::getTaskReadFile(int subAddr, int nb)
{
	ModbusTask* task = new ModbusTask(DEFAULT_MODBUS_SLAVE, MODBUS_FC_READ_FILE_RECORD, subAddr, nb);
	return task;
}

ModbusTask* ModbusAdapter::getTaskWrite(int startAddr, int numOfRegs, std::string writeData)
{
	ModbusTask* task = new ModbusTask(DEFAULT_MODBUS_SLAVE, DEFAULT_MODBUS_WRITE_FUNCTION, startAddr, numOfRegs, writeData);
	return task;
}

ModbusTask* ModbusAdapter::getTaskRead(int startAddr, int numOfRegs)
{
	ModbusTask* task = new ModbusTask(DEFAULT_MODBUS_SLAVE, DEFAULT_MODBUS_READ_FUNCTION, startAddr, numOfRegs);
	return task;
}

ModbusTask* ModbusAdapter::getTaskWait(std::function<void(int, ModbusTask*, ModbusAdapter*)> function, std::string postDoFuncDesc)
{
	ModbusTask* task = new ModbusTask(DEFAULT_MODBUS_SLAVE, MODBUS_FC_WAIT, -1, 0);
	task->setPostFunction(function, postDoFuncDesc);
	return task;
}

void ModbusAdapter::addTaskAsNormal(ModbusTask* task, int timeInterval, bool showInGui)
{
	m_tasksNormal.push_back(std::make_tuple(*task, timeInterval, showInGui));
	//return task;
}

void ModbusAdapter::addTask(ModbusTask* task, int priority)
{
	if (m_useThread) {
		m_taskThread->addTask(task, priority);
		//return task;
	}
	else {
		doTask(task, NULL);
		delete task;
		//return NULL;
	}
}

int ModbusAdapter::doTask(ModbusTask* task, TaskThread<ModbusTask>* taskThread)
{
	/*if (!m_modbus)
		return -1;*/
	try {
		auto modbusAdapter = this;
		int ret;

		if (task->preDoFunction) {
			int r = task->preDoFunction(task, modbusAdapter);
			if (r != 1) {
				LOG_INFO(8, "Modbus: Task pre function run exclude and return (" << r << "), " << task->preDoFunctionDesc);
				return 1;
			}
			else {
				LOG_INFO(8, "Modbus: Task pre function run ok, continue to do(1), " << task->preDoFunctionDesc);
			}
		}

		if (task->functionCode == MODBUS_FC_WAIT) {
#ifdef _DEBUG
			if (m_currentTaskDesc != task->postDoFunctionDesc) {
				std::lock_guard<std::mutex> lock(m_mutex);
				m_currentTaskDesc.assign(task->postDoFunctionDesc);
			}
#endif
			task->postDoFunction(0, task, modbusAdapter);
			ret = task->startAddr;
			if (ret == -9) {
				LOG_WARNING("Modbus: Task wait function run failed(-9), " << task->postDoFunctionDesc);
			}
			else if (ret >= -1) {
				LOG_INFO(8, "Modbus: Task wait function run succeed(" << ret << "), " << task->postDoFunctionDesc);
				//m_currentTask = "";
			}
			return task->startAddr;
		}

		if (EUtils::ModbusIsWriteFunction(task->functionCode))
		{
			//m_currentTask = "Write Reg";
			if (!task->writeData.empty()) {
				uint quantity = task->numOfRegs;
				std::stringstream ss(task->writeData);
				std::string word;
				uint idx = 0;
				uint16_t* writeData = new uint16_t[quantity];
				while (ss >> word) {
					writeData[idx] = std::stoi(word, 0, 16);
					idx++;
					if (idx >= quantity)
						break;
				}
				if (quantity > idx) {
					quantity = idx;
				}
				std::function<int()> func = [modbusAdapter, task, writeData]() {
					return modbusAdapter->modbusWriteDataRaw(task->slave, task->functionCode, task->startAddr, task->numOfRegs, writeData); 
				};
				int retryCnt = 3;
				ret = EUtils::doUntilOk(func, retryCnt);
				if (retryCnt > 1) {
					LOG_WARNING("Modbus: " << EUtils::ModbusDataTypeName(task->functionCode) << " do multiple times(" << retryCnt << "), " << task->startAddr << ", " << task->numOfRegs << ", " << task->writeData.c_str());
				}
				delete[] writeData;
			}
			else {
				fillWriteData(task->functionCode, task->numOfRegs);
				ret = modbusWriteDataRaw(task->slave, task->functionCode, task->startAddr, task->numOfRegs, writeDataDest16);
			}
			//if (task->startAddr == 61)
			//	ret = -1;

			if (ret == -1) {
				LOG_WARNING("Modbus: " << EUtils::ModbusDataTypeName(task->functionCode) << " failed, " << task->startAddr << ", " << task->numOfRegs << ", " << task->writeData.c_str() << ", " << EUtils::QString2StdString(EUtils::libmodbus_strerror(errno)));
				//THROW(EUtils::QString2StdString(QStringLiteral("Modbus写错误")));
				
				
#ifdef _DEBUG
				// 只是显示，不停止运行
				ret = 1;
#else
				if (!SystemSettings::instance().GetValue("Modbus/WriteErrorAsWarning").isEmpty()) {
					ret = 1;
				}
				else {
					LOG_ERROR(EUtils::QString2StdString(QStringLiteral("Modbus: 写错误")));
					ret = -9;
				}
#endif 
			}
			else {
				if (task->startAddr == 19)	// pad's pos
					LOG_INFO(9, "Modbus: " << EUtils::ModbusDataTypeName(task->functionCode) << ", " << task->startAddr << ", " << task->numOfRegs << ", " << task->writeData.c_str());
				else
					LOG_INFO(8, "Modbus: " << EUtils::ModbusDataTypeName(task->functionCode) << ", " << task->startAddr << ", " << task->numOfRegs << ", " << task->writeData.c_str());
			}
		}
		else
		{
			//m_currentTask = "Read Reg";
			ret = modbusAdapter->modbusReadDataRaw(task->slave, task->functionCode, task->startAddr, task->numOfRegs, NULL);
			if (ret == -1) {
				LOG_WARNING("Modbus: " << EUtils::ModbusDataTypeName(task->functionCode) << " failed, " << task->startAddr << ", " << task->numOfRegs << ", " << EUtils::QString2StdString(EUtils::libmodbus_strerror(errno)));
				//LOG_ERROR(EUtils::QString2StdString(QStringLiteral("Modbus: 读错误")));
			}
			else {
				LOG_INFO(9, "Modbus: " << EUtils::ModbusDataTypeName(task->functionCode) << ", " << task->startAddr << ", " << task->numOfRegs);
			}
		}

		if (task->postDoFunction) {
			task->postDoFunction(ret, task, modbusAdapter);
		}
		return ret;
	}
	catch (const std::exception& e) {
		//LOG_WARNING("Modbus: exception: " << e.what());
		LOG_ERROR("Modbus: " << e.what() << ", " << EUtils::ModbusDataTypeName(task->functionCode) << " failed, " << task->startAddr << ", " << task->numOfRegs << task->preDoFunctionDesc << "," << task->postDoFunctionDesc);
		//QMetaObject::invokeMethod(parent(), [this]() {
		//	NCMachine* machine = qobject_cast<NCMachine*>(parent());
		//	machine->StopRun();
		//	}, Qt::QueuedConnection);
		//LOG_WARNING("Modbus:" << std::stacktrace::current())
		return -1;
	}
	catch (...)
	{
		LOG_ERROR("Modbus: " << "Unknown error" << ", " << EUtils::ModbusDataTypeName(task->functionCode) << " failed, " << task->startAddr << ", " << task->numOfRegs << task->preDoFunctionDesc << "," << task->postDoFunctionDesc);
		return -1;
	}
}

void ModbusAdapter::updateState(int ret)
{
	if (m_modbus == NULL)
		return;

	//int slave = task.slave;
	int functionCode = m_currentGuiTask.functionCode;
	int startAddr = m_currentGuiTask.startAddr;
	int numOfRegs = m_currentGuiTask.numOfRegs;

	if (EUtils::ModbusIsWriteFunction(functionCode))
	{
		//QLOG_INFO() <<  "Modbus Write Data ";

		//QLOG_TRACE() <<  "Modbus Write Data return value = " << ret << ", errno = " << errno;;

		//update data model
		if (ret == numOfRegs)
		{
			//mainWin->hideInfoBar();
			if (m_regModel != NULL) {
			}

			if (m_rawModel != NULL) {
				m_rawModel->addLine(EUtils::SysTimeStamp() + QString().sprintf(" - Modbus Write Data addr %d len %d", startAddr, numOfRegs));
				m_rawModel->addLine(EUtils::SysTimeStamp() + " - values written correctly.");
			}
		}
		else
		{
			//mainWin->showUpInfoBar(line, InfoBar::Error);

			if (m_regModel != NULL) {
				m_regModel->setInvalidValues();
			}

			if (m_rawModel != NULL) {
				m_rawModel->addLine(EUtils::SysTimeStamp() + QString().sprintf(" - Modbus Write Data addr %d len %d", startAddr, numOfRegs));
				QString line;
				if (ret < 0) {
					line = QString(tr("Write data failed. Error : ")) + EUtils::libmodbus_strerror(errno);
					//QLOG_ERROR() <<  "Write Data failed. " << line;
					m_rawModel->addLine(EUtils::SysTimeStamp() + line);

				}
				else {
					line = QString(tr("Write data failed. Number of registers returned does not match number of registers requested!. Error : ")) + EUtils::libmodbus_strerror(errno);
					//QLOG_ERROR() <<  "Write Data failed. " << line;
					m_rawModel->addLine(EUtils::SysTimeStamp() + line);

				}
			}
		}
	}
	else
	{
		//QLOG_INFO() <<  "Modbus Read Data ";

		//int ret = modbusReadDataRaw(slave, functionCode, startAddr, numOfRegs, NULL);
		/*if (ret == -1)
			return;*/

			//QLOG_TRACE() <<  "Modbus Read Data return value = " << ret << ", errno = " << errno;

		bool is16Bit = functionCode == MODBUS_FC_READ_HOLDING_REGISTERS || functionCode == MODBUS_FC_READ_INPUT_REGISTERS;
		//update data model
		if (ret != -1 && ret == numOfRegs)
		{
			//mainWin->hideInfoBar();
			if (m_regModel != NULL) {
				for (int i = 0; i < numOfRegs; ++i)
				{
					int data = is16Bit ? readDataDest16[i] : readDataDest[i];
					m_regModel->setValue(i, data);
				}
			}
			if (m_rawModel != NULL) {
				//m_rawModel->addLine(EUtils::SysTimeStamp() + QString().sprintf(" - Modbus Read Data addr %d len %d", startAddr, m_numOfRegs));

				//QString line;
				//for (int i = 0; i < numOfRegs; ++i) {
				//    int data = is16Bit ? dest16[i] : dest[i];
				//    line += QString().sprintf("%.2x  ", data);
				//}
				//line = EUtils::TxTimeStamp(m_ModBusMode) + " - " + line.toUpper();
				//rawModel->addLine(line);
				//m_rawModel->addLine(EUtils::SysTimeStamp() + " - values read correctly.");
			}
		}
		else
		{
			//mainWin->showUpInfoBar(line, InfoBar::Error);
			if (m_regModel != NULL) {
				m_regModel->setInvalidValues();
			}
			if (m_rawModel != NULL) {
				//m_rawModel->addLine(EUtils::SysTimeStamp() + QString().sprintf(" - Modbus Read Data addr %d len %d", startAddr, m_numOfRegs));

				QString line = "";
				if (ret < 0) {
					line = QString(tr("Read data failed. Error : ")) + EUtils::libmodbus_strerror(errno);
					//QLOG_ERROR() <<  "Read Data failed. " << line;
					m_rawModel->addLine(EUtils::SysTimeStamp() + line);

				}
				else {
					line = QString(tr("Read data failed. Number of registers returned does not match number of registers requested!. Error : ")) + EUtils::libmodbus_strerror(errno);
					//QLOG_ERROR() <<  "Read Data failed. " << line;
					m_rawModel->addLine(EUtils::SysTimeStamp() + line);
				}
			}
		}
	}

	emit(refreshView());
}


void ModbusAdapter::busMonitorRequestData(uint8_t* data, int dataLen)
{
	//Request Raw data from port - Update raw data model
	if (m_rawModel != NULL) {
		QString line;

		for (int i = 0; i < dataLen; ++i) {
			line += QString().sprintf("%.2x  ", data[i]);
		}

		//QLOG_INFO() << "Tx Data : " << line;
		line = EUtils::TxTimeStamp(m_ModBusMode) + line.toUpper();

		m_rawModel->addLine(line);
	}
}

void ModbusAdapter::busMonitorResponseData(uint8_t* data, int dataLen)
{
	//Response Raw data from port - Update raw data model
	if (m_rawModel != NULL) {
		QString line;

		for (int i = 0; i < dataLen; ++i) {
			line += QString().sprintf("%.2x  ", data[i]);
		}

		//QLOG_INFO() << "Rx Data : " << line;
		line = EUtils::RxTimeStamp(m_ModBusMode) + line.toUpper();

		m_rawModel->addLine(line);
	}
}

void ModbusAdapter::addItems(int startAddr, int numOfRegs)
{
	if (m_regModel != NULL) {
		//m_regModel->addItems(m_startAddr, m_numOfRegs, EUtils::ModbusIsWriteFunction(m_functionCode));
		m_regModel->addItems(startAddr, numOfRegs, false);
	}
	//If it is a write function -> read registers
	if (!m_connected)
		return;
	/*else if (EUtils::ModbusIsWriteCoilsFunction(m_functionCode)){
		afterModbusReadData(m_slave,EUtils::ReadCoils,m_startAddr,m_numOfRegs);
		emit(refreshView());
	}
	else if (EUtils::ModbusIsWriteRegistersFunction(m_functionCode)){
		afterModbusReadData(m_slave,EUtils::ReadHoldRegs,m_startAddr,m_numOfRegs);
		emit(refreshView());
	}*/
}

void ModbusAdapter::resetCounters()
{
	m_packets = 0;
	m_errors = 0;
	m_msecComm = 0;
	m_timeStart = QDateTime::currentDateTime();
	emit(refreshView());
}

void ModbusAdapter::startPollTimer()
{
	if (m_useThread) {
		if (!m_taskThread) {
			std::function<int(ModbusTask*, TaskThread<ModbusTask>*)> doTaskFunction = [this](ModbusTask* task, TaskThread<ModbusTask>* taskThread) {
				return this->doTask(task, taskThread);
			};
			m_taskThread = new TaskThread<ModbusTask>(doTaskFunction);
		}
		m_taskThread->startTaskThread();
	}

	if (m_modbus == NULL || !m_connected)
		return;
	if (!m_pollTimer->isActive()) {
		m_pollTimer->start(m_scanRate);
	}
}

void ModbusAdapter::stopPollTimer()
{
	if (m_useThread) {
		m_taskThread->stopTaskThread();
	}

	if (m_modbus == NULL || !m_connected)
		return;
	
	if (m_pollTimer->isActive()) {
		m_pollTimer->stop();
	}
}

QString ModbusAdapter::stripIP(QString ip)
{
	//Strip zero's from IP
	QStringList ipBytes;
	QString res = "";
	int i;

	ipBytes = ip.split(".");
	if (ipBytes.size() == 4) {
		res = QString("").setNum(ipBytes[0].toInt());
		i = 1;
		while (i < ipBytes.size()) {
			res = res + "." + QString("").setNum(ipBytes[i].toInt());
			i++;
		}
		return res;
	}
	else
		return "";

}

extern "C" {

	void busMonitorRawResponseData(uint8_t* data, int dataLen)
	{
		s_instance->busMonitorResponseData(data, dataLen);
	}

	void busMonitorRawRequestData(uint8_t* data, int dataLen)
	{
		s_instance->busMonitorRequestData(data, dataLen);
	}

}
