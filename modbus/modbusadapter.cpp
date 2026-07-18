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
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QTime>

#define LIB_MODBUS_DEBUG_OUTPUT

ModbusAdapter* s_instance;

#define DATA_BUFFER_LEN 2000
#define DATA16_BUFFER_LEN 125

ModbusAdapter::ModbusAdapter(QObject* parent) :
	QObject(parent), m_taskThread(NULL),
	m_useThread(true), m_packets{ 0 }, m_errors{ 0 }, m_currentGuiTask(0, 0, 0, 0), m_timerCount(0),
	m_replayLogFile(nullptr), m_regModel(NULL), m_rawModel(NULL)
{
	if (true || cb::Logger::instance().getVerbosity() >= 10) {
		m_rawModel = new RawDataModel(this);
		m_rawModel->enableAddLines(true);
		connect(m_rawModel, &RawDataModel::lineAdded, [](QString line) {
			LOG_INFO(10, EUtils::QString2StdString(line));
			});
	}
	for (int i = 0; i < MODBUS_CONNECTION_COUNT; ++i) {
		QString iniFile = SystemSettings::GetPath("qModMaster.ini", SystemSettings::SystemFlag);
		if (i > 0) {
			// For additional connections, use qModMaster1.ini, qModMaster2.ini, etc.
			iniFile = SystemSettings::GetPath(QString("qModMaster%1.ini").arg(i + 1), SystemSettings::SystemFlag);
		}
		// check whether file exist
		if (QFile::exists(iniFile)) {
			m_modbusCommSettings[i] = new ModbusCommSettings(iniFile);
		}
		else {
			m_modbusCommSettings[i] = NULL;
		}
	}

	s_instance = this;

	// Initialize connection arrays
	for (int i = 0; i < MODBUS_CONNECTION_COUNT; ++i) {
		m_modbusList[i] = NULL;
		m_connectedList[i] = false;
	}

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

	// 初始化日志文件
	QString logDir = SystemSettings::GetPath("logs", SystemSettings::UserFlag);
	QString logFileName = logDir + "/modbus_write_log.csv";
	logFileName = "";
	if (!logFileName.isEmpty()) {
		QDir dir(logDir);
		if (!dir.exists()) {
			dir.mkpath(logDir);
		}

		m_replayLogFile = new QFile(logFileName);
		if (m_replayLogFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
			LOG_INFO(1, "Modbus write log initialized: " << logFileName.toUtf8().constData());
		}
		else {
			LOG_WARNING("Modbus: Failed to initialize write log file");
			delete m_replayLogFile;
			m_replayLogFile = nullptr;
		}
	}
}

ModbusAdapter::~ModbusAdapter()
{
	if (m_taskThread) {
		m_taskThread->stopTaskThread();
		delete m_taskThread;
	}

	modbusDisConnect();

	if (m_rawModel != NULL) {
		delete m_rawModel;
		m_rawModel = NULL;
	}
	if (m_regModel != NULL) {
		delete m_regModel;
		m_regModel = NULL;
	}
	for (int i = 0; i < MODBUS_CONNECTION_COUNT; ++i) {
		if (m_modbusCommSettings[i] != NULL) {
			delete m_modbusCommSettings[i];
			m_modbusCommSettings[i] = NULL;
		}
	}

	free(readDataDest);
	free(readDataDest16);
	//free(writeDataDest);
	free(writeDataDest16);

	if (m_replayLogFile) {
		m_replayLogFile->close();
		delete m_replayLogFile;
	}
}

void ModbusAdapter::modbusConnect(int connectionIndex)
{
	if (connectionIndex == -1) {
		// Connect all
		for (int i = 0; i < MODBUS_CONNECTION_COUNT; ++i) {
			modbusConnect(i);
		}
		return;
	}

	if (connectionIndex < 0 || connectionIndex >= MODBUS_CONNECTION_COUNT)
		return;

	ModbusCommSettings* settings = m_modbusCommSettings[connectionIndex];
	if (!settings)
		return;

	QString mode = settings->mode();
	if (mode == "TCP") {
		modbusConnectTCP(settings->slaveIP(),
			settings->TCPPort().toInt(),
			settings->responseTimeOut().toInt() / 1000,
			connectionIndex);
	}
	else if (mode == "RTU") {
		modbusConnectRTU(settings->serialPortName(),
			settings->baud().toInt(),
			EUtils::parity(settings->parity()),
			settings->dataBits().toInt(),
			settings->stopBits().toInt(),
			settings->RTS().toInt(),
			settings->responseTimeOut().toInt(),
			settings->byteTimeOut().toInt(),
			connectionIndex);
	}
	if (connectionIndex == 0) {
		this->setScanRate(settings->scanRate());
	}
}

void ModbusAdapter::modbusConnectRTU(QString port, int baud, QChar parity, int dataBits, int stopBits, int RTS, int responseTimeOut, int byteTimeout, int connectionIndex)
{
	//Modbus RTU connect

	modbusDisConnect(connectionIndex);

	modbus_t* modbus = modbus_new_rtu(port.toLatin1().constData(), baud, parity.toLatin1(), dataBits, stopBits);

#ifdef LIB_MODBUS_DEBUG_OUTPUT
	if (modbus) modbus_set_debug(modbus, 1);
#endif

	bool connected = false;
	if (modbus == NULL) {
		//mainWin->showUpInfoBar(tr("Unable to create the libmodbus context."), InfoBar::Error);
		//QLOG_ERROR()<<  "Connection failed. Unable to create the libmodbus context";

		return;
	}
	else if (modbus_set_slave(modbus, DEFAULT_MODBUS_SLAVE) == -1) {
		modbus_free(modbus);
		//mainWin->showUpInfoBar(tr("Invalid slave ID."), InfoBar::Error);
		//QLOG_ERROR()<<  "Connection failed. Invalid slave ID";

		return;
	}
#if HAVE_DECL_TIOCM_RTS
	else if (modbus_rtu_set_rts(modbus, RTS) == -1) {
		modbus_free(modbus);
		return;
	}
#endif
	else if (modbus_connect(modbus) == -1) {
		modbus_free(modbus);
		//mainWin->showUpInfoBar(tr("Connection failed\nCould not connect to serial port."), InfoBar::Error);
		//QLOG_ERROR()<<  "Connection failed. Could not connect to serial port";
		return;
	}
	else {
		//error recovery mode
		//modbus_set_error_recovery(m_modbus, MODBUS_ERROR_RECOVERY_PROTOCOL);

		modbus_set_error_recovery(modbus, MODBUS_ERROR_RECOVERY_NONE);
		modbus_set_response_timeout(modbus, 0, responseTimeOut * 1000);
		modbus_set_byte_timeout(modbus, 0, byteTimeout * 1000);
		connected = true;
	}

	// Ensure index is within bounds
	if (connectionIndex < 0 || connectionIndex >= MODBUS_CONNECTION_COUNT)
		return;

	m_modbusList[connectionIndex] = modbus;
	m_connectedList[connectionIndex] = connected;

	if (m_rawModel != NULL) {
		QString line;
		line = "Connecting to Serial Port [" + port + "]...";
		line += connected ? "OK" : "Failed";
		line = EUtils::SysTimeStamp() + line;
		m_rawModel->addLine(line);
	}
}

void ModbusAdapter::modbusConnectTCP(QString ip, int port, int timeOut, int connectionIndex)
{
	//Modbus TCP connect

	//QLOG_INFO()<<  "Modbus Connect TCP";

	QString strippedIP = stripIP(ip);
	if (strippedIP == "") {
		//mainWin->showUpInfoBar(tr("Connection failed\nBlank IP Address."), InfoBar::Error);
		//QLOG_ERROR()<<  "Connection failed. Blank IP Address";
		return;
	}

	modbusDisConnect(connectionIndex);

	modbus_t* modbus = modbus_new_tcp(strippedIP.toLatin1().constData(), port);
	//mainWin->hideInfoBar();
	//QLOG_TRACE() <<  "Connecting to IP : " << ip << ":" << port;

#ifdef LIB_MODBUS_DEBUG_OUTPUT
	if (modbus) modbus_set_debug(modbus, 1);
#endif

	bool connected = false;
	if (modbus == NULL) {
		//mainWin->showUpInfoBar(tr("Unable to create the libmodbus context."), InfoBar::Error);
		//QLOG_ERROR()<<  "Connection failed. Unable to create the libmodbus context";
		return;
	}
	else if (modbus_connect(modbus) == -1) {
		modbus_free(modbus);
		//mainWin->showUpInfoBar(tr("Connection failed\nCould not connect to TCP port."), InfoBar::Error);
		//QLOG_ERROR()<<  "Connection to IP : " << ip << ":" << port << "...failed. Could not connect to TCP port";
		return;
	}
	else {
		//error recovery mode
		modbus_set_error_recovery(modbus, MODBUS_ERROR_RECOVERY_PROTOCOL);
		//response_timeout;
		modbus_set_response_timeout(modbus, timeOut, 0);
		connected = true;
		//mainWin->hideInfoBar();
		//QLOG_TRACE() << line;
	}

	// Ensure index is within bounds
	if (connectionIndex < 0 || connectionIndex >= MODBUS_CONNECTION_COUNT)
		return;

	m_modbusList[connectionIndex] = modbus;
	m_connectedList[connectionIndex] = connected;

	if (m_rawModel != NULL) {
		//Add line to raw data model
		QString line;
		line = "Connecting to IP : " + ip + ":" + QString::number(port);
		line += connected ? "OK" : "Failed";
		line = EUtils::SysTimeStamp() + line;
		m_rawModel->addLine(line);
	}
}

void ModbusAdapter::modbusDisConnect(int connectionIndex)
{
	//Modbus disconnect

	//QLOG_INFO()<<  "Modbus disconnected";
	resetCounters();

	if (connectionIndex == -1) {
		// Disconnect all
		for (int i = 0; i < MODBUS_CONNECTION_COUNT; ++i) {
			if (m_modbusList[i]) {
				if (m_connectedList[i]) {
					modbus_close(m_modbusList[i]);
					modbus_free(m_modbusList[i]);
				}
				m_modbusList[i] = NULL;
				m_connectedList[i] = false;
			}
		}
	}
	else if (connectionIndex >= 0 && connectionIndex < MODBUS_CONNECTION_COUNT) {
		if (m_modbusList[connectionIndex]) {
			if (m_connectedList[connectionIndex]) {
				modbus_close(m_modbusList[connectionIndex]);
				modbus_free(m_modbusList[connectionIndex]);
			}
			m_modbusList[connectionIndex] = NULL;
			m_connectedList[connectionIndex] = false;
		}
	}
}


bool ModbusAdapter::isConnected(int connectionIndex) const
{
	if (connectionIndex >= 0 && connectionIndex < MODBUS_CONNECTION_COUNT) {
		return m_connectedList[connectionIndex];
	}
	return false;
}

modbus_t* ModbusAdapter::getModbusContext(int connectionIndex)
{
	if (connectionIndex >= 0 && connectionIndex < MODBUS_CONNECTION_COUNT) {
		return m_modbusList[connectionIndex];
	}
	return NULL;
}

ModbusCommSettings* ModbusAdapter::getCommSettings(int connectionIndex)
{
	if (connectionIndex >= 0 && connectionIndex < MODBUS_CONNECTION_COUNT) {
		return m_modbusCommSettings[connectionIndex];
	}
	return NULL;
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
	if (!m_taskThread || m_taskThread->getTasksCnt(TASK_TIMER_PRIORITY) > TASKQUEUE_ITEM_MAX_COUNT)
		return;

	m_timerCount++;

	if (m_tasksNormal.size() == 0) {
		QMetaObject::invokeMethod(this, "updateState", Qt::QueuedConnection, Q_ARG(int, 1));
		return;
	}

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

			ModbusTask* newtask = new ModbusTask(DEFAULT_MODBUS_SLAVE, DEFAULT_MODBUS_READ_FUNCTION, startAddr, numOfRegs, task.connectionIndex);
			newtask->setPostFunction(function, "Normal Task to update state");
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

int ModbusAdapter::modbusReadDataRaw(int slave, int functionCode, int startAddr, int numOfRegs, uint16_t* readData, int connectionIndex)
{
	modbus_t* modbus = getModbusContext(connectionIndex);
	if (modbus == NULL)
		return -1;

	// Test condition when modbus read error
	//std::mt19937 engine(std::time(nullptr));
	//std::uniform_real_distribution<> realDistribution(0.0, 1.0);
	//double randomNumberReal = realDistribution(engine);
	//if (randomNumberReal < 0.1) {
	//	m_errors += 1;
	//	modbus_flush(modbus); //flush data
	//	return -1;
	//}

	QTime time1 = QTime::currentTime();
	m_packets[connectionIndex] += 1;
	int ret = -1; //return value from read functions
	bool is16Bit = false;

	modbus_set_slave(modbus, slave);
	//request data from modbus
	switch (functionCode)
	{
	case MODBUS_FC_READ_COILS:
		assert(numOfRegs <= DATA_BUFFER_LEN);
		ret = modbus_read_bits(modbus, startAddr, numOfRegs, readDataDest);
		break;

	case MODBUS_FC_READ_DISCRETE_INPUTS:
		assert(numOfRegs <= DATA_BUFFER_LEN);
		ret = modbus_read_input_bits(modbus, startAddr, numOfRegs, readDataDest);
		break;

	case MODBUS_FC_READ_HOLDING_REGISTERS:
		assert(numOfRegs <= DATA16_BUFFER_LEN);
		ret = modbus_read_registers(modbus, startAddr, numOfRegs, readDataDest16);
		is16Bit = true;
		break;

	case MODBUS_FC_READ_INPUT_REGISTERS:
		assert(numOfRegs <= DATA16_BUFFER_LEN);
		ret = modbus_read_input_registers(modbus, startAddr, numOfRegs, readDataDest16);
		is16Bit = true;
		break;
	case MODBUS_FC_READ_FILE_RECORD:
		ret = modbus_read_file_record(modbus, DEFAULT_MODBUS_FILE_ADDRESS, startAddr, numOfRegs, readDataDest16);
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
		m_errors[connectionIndex] += 1;
		modbus_flush(modbus); //flush data
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

int ModbusAdapter::modbusWriteDataRaw(int slave, int functionCode, int startAddr, int numOfRegs, uint16_t* writeData, int connectionIndex)
{
	//std::random_device rd;
	//std::mt19937 gen(rd());
	//std::uniform_int_distribution<short> distrib2(-32768, 32767);
	//std::generate(writeDataDest16, writeDataDest16 + DATA16_BUFFER_LEN, [&]() { return distrib2(gen); });

	modbus_t* modbus = getModbusContext(connectionIndex);
	if (modbus == NULL)
		return -1;

	// Test condition when modbus write error
	//std::mt19937 engine(std::time(nullptr));
	//std::uniform_real_distribution<> realDistribution(0.0, 1.0);
	//double randomNumberReal = realDistribution(engine);
	//if (randomNumberReal < 0.1) {
	//	m_errors += 1;
	//	modbus_flush(modbus); //flush data
	//	return -1;
	//}

	QTime time1 = QTime::currentTime();
	m_packets[connectionIndex] += 1;
	int ret = -1; //return value from functions

	modbus_set_slave(modbus, slave);

	switch (functionCode)
	{
	case MODBUS_FC_WRITE_SINGLE_COIL:
	{
		uint8_t data = (uint8_t)writeData[0];
		ret = modbus_write_bit(modbus, startAddr, data);
		numOfRegs = 1;
		break;
	}
	case MODBUS_FC_WRITE_SINGLE_REGISTER:
	{
		ret = modbus_write_register(modbus, startAddr, writeData[0]);
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
		ret = modbus_write_bits(modbus, startAddr, numOfRegs, data);
		delete[] data;
		break;
	}
	case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
	{
		ret = modbus_write_registers(modbus, startAddr, numOfRegs, writeData);
		break;
	}
	case MODBUS_FC_WRITE_FILE_RECORD:
	{
		ret= modbus_write_file_record(modbus, DEFAULT_MODBUS_FILE_ADDRESS, startAddr, numOfRegs, writeData);
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
		m_errors[connectionIndex] += 1;
		modbus_flush(modbus); //flush data
	}

	QTime time2 = QTime::currentTime();
	m_msecComm += time1.msecsTo(time2);

	// Log write operation for replay
	logWriteOperation(startAddr, numOfRegs, writeData);

	return ret;
}

void ModbusAdapter::logWriteOperation(int startAddr, int numOfRegs, uint16_t* writeData)
{
	if (!m_replayLogFile)
		return;

	std::lock_guard<std::mutex> lock(m_replayLogMutex);

	if (!m_replayLogFile->isOpen())
		return;

	// 格式：startAddr,numOfRegs,hexdata1,hexdata2,...
	QStringList hexData;
	for (int i = 0; i < numOfRegs; ++i) {
		hexData << QString("%1").arg(writeData[i], 4, 16, QChar('0'));
	}

	QString logLine = QString("%1,%2,%3\n")
		.arg(startAddr)
		.arg(numOfRegs)
		.arg(hexData.join(":"));

	m_replayLogFile->write(logLine.toUtf8());
	m_replayLogFile->flush();
}

ModbusTask* ModbusAdapter::getTaskWriteFile(int subAddr, int nb, std::string writeData, int connectionIndex, int slave)
{
	ModbusTask* task = new ModbusTask(slave, MODBUS_FC_WRITE_FILE_RECORD, subAddr, nb, writeData, connectionIndex);
	return task;
}

ModbusTask* ModbusAdapter::getTaskReadFile(int subAddr, int nb, int connectionIndex, int slave)
{
	ModbusTask* task = new ModbusTask(slave, MODBUS_FC_READ_FILE_RECORD, subAddr, nb, connectionIndex);
	return task;
}

ModbusTask* ModbusAdapter::getTaskWrite(int startAddr, int numOfRegs, std::string writeData, int connectionIndex, int slave)
{
	ModbusTask* task = new ModbusTask(slave, DEFAULT_MODBUS_WRITE_FUNCTION, startAddr, numOfRegs, writeData, connectionIndex);
	return task;
}

ModbusTask* ModbusAdapter::getTaskRead(int startAddr, int numOfRegs, int connectionIndex, int slave)
{
	ModbusTask* task = new ModbusTask(slave, DEFAULT_MODBUS_READ_FUNCTION, startAddr, numOfRegs, connectionIndex);
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
	// 运行G代码任务时，不能依赖于定时器的状态读取。即使没有这个也要能执行完成
	//return;
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
					return modbusAdapter->modbusWriteDataRaw(task->slave, task->functionCode, task->startAddr, task->numOfRegs, writeData, task->connectionIndex); 
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
				ret = modbusWriteDataRaw(task->slave, task->functionCode, task->startAddr, task->numOfRegs, writeDataDest16, task->connectionIndex);
			}
			//if (task->startAddr == 61)
			//	ret = -1;

			if (ret == -1) {
				LOG_WARNING("Modbus: " << EUtils::ModbusDataTypeName(task->functionCode) << " failed, " << task->startAddr << ", " << task->numOfRegs << ", " << task->writeData.c_str() << ", " << EUtils::QString2StdString(EUtils::libmodbus_strerror(errno)));
// [AUTO-TRANSLATION-COMMENT] Modbus: 写错误
				//THROW(EUtils::QString2StdString(tr("MXCW")));
#ifdef _DEBUG
				// 只是显示，不停止运行
				ret = 1;
#else
				if (SystemSettings::instance().GetValue("Modbus/WriteErrorAsWarning") == "True") {
					ret = 1;
				}
				else {
// [AUTO-TRANSLATION-COMMENT] Modbus: 写错误
					LOG_ERROR(EUtils::QString2StdString(tr("MXCW")));
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
			ret = modbusAdapter->modbusReadDataRaw(task->slave, task->functionCode, task->startAddr, task->numOfRegs, NULL, task->connectionIndex);
			if (ret == -1) {
				LOG_WARNING("Modbus: " << EUtils::ModbusDataTypeName(task->functionCode) << " failed, " << task->startAddr << ", " << task->numOfRegs << ", " << EUtils::QString2StdString(EUtils::libmodbus_strerror(errno)));
				//LOG_ERROR(EUtils::QString2StdString(tr("MDCW")));
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
	modbus_t* modbus = getModbusContext(m_currentGuiTask.connectionIndex);
	if (modbus == NULL)
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
		int mode = (m_modbusCommSettings[0] && m_modbusCommSettings[0]->mode() == "TCP") ? EUtils::TCP : EUtils::RTU;
		line = EUtils::TxTimeStamp(mode) + line.toUpper();

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
		int mode = (m_modbusCommSettings[0] && m_modbusCommSettings[0]->mode() == "TCP") ? EUtils::TCP : EUtils::RTU;
		line = EUtils::RxTimeStamp(mode) + line.toUpper();

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
	if (!isConnected(0))
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
	std::fill(m_packets, m_packets + MODBUS_CONNECTION_COUNT, 0);
	std::fill(m_errors, m_errors + MODBUS_CONNECTION_COUNT, 0);

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

	if (!isConnected(0))
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

	if (!isConnected(0))
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
