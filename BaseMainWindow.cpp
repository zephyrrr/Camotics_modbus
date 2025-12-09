#include "BaseMainWindow.h"
#include <QDir>
#include <QToolButton>
#include "../modbus/NCMachine.h"

BaseMainWindow::BaseMainWindow(QWidget* parent)
	:QMainWindow(parent)
{
	//m_regModel = new RegistersModel(this);
	if (true || cb::Logger::instance().getVerbosity() >= 10) {
		m_rawModel = new RawDataModel(this);
		m_rawModel->enableAddLines(true);
		connect(m_rawModel, &RawDataModel::lineAdded, [](QString line) {
			LOG_INFO(10, EUtils::QString2StdString(line));
			});
	}

	m_modbusCommSettings = new ModbusCommSettings("data/qModMaster.ini");
	m_modbus = new ModbusAdapter(this, m_regModel, m_rawModel);
	m_ncMachine = new NCMachine(this, m_modbus);

	SystemSettings::instance().SetProjectDir(QDir::currentPath() + "/data/default");
	SystemSettings::instance().LoadFromFile(QDir::currentPath() + "/data/qGlobal.ini");

	modbusConnect(true);

	if (m_modbus->isConnected()) {

	}
}

BaseMainWindow::~BaseMainWindow()
{
	modbusConnect(false);

	if (m_modbus != NULL) {
		m_modbus->stopPollTimer();
		m_modbus->modbusDisConnect();
	}

	if (m_ncMachine != NULL) {
		delete m_ncMachine;
		m_ncMachine = NULL;
	}

	if (m_modbus != NULL) {
		delete m_modbus;
		m_modbus = NULL;
	}

	if (m_modbusCommSettings != NULL) {
		delete m_modbusCommSettings;
		m_modbusCommSettings = NULL;
	}
	if (m_rawModel != NULL) {
		delete m_rawModel;
		m_rawModel = NULL;
	}
	if (m_regModel != NULL) {
		delete m_regModel;
		m_regModel = NULL;
	}
}

void BaseMainWindow::modbusConnect(bool connect)
{
	modbusConnect(connect, m_modbus, m_modbusCommSettings, m_ncMachine);
}
void BaseMainWindow::modbusConnect(bool connect, ModbusAdapter* modbus, ModbusCommSettings* modbusCommSettings, NCMachine* ncMachine)
{
	if (connect) {
		modbus->modbusConnectRTU(
			modbusCommSettings->serialPortName(),
			modbusCommSettings->baud().toInt(),
			EUtils::parity(modbusCommSettings->parity()),
			modbusCommSettings->dataBits().toInt(),
			modbusCommSettings->stopBits().toInt(),
			modbusCommSettings->RTS().toInt(),
			modbusCommSettings->responseTimeOut().toInt(),
			modbusCommSettings->byteTimeOut().toInt()
		);
		modbus->setScanRate(modbusCommSettings->scanRate());
		modbus->startPollTimer();

		ncMachine->startTaskThread();
	}
	else { //Disconnect
		modbus->stopPollTimer();
		modbus->modbusDisConnect();

		ncMachine->stopTaskThread();
	}
}

void BaseMainWindow::addNormalTasks()
{
	std::function<void(int, ModbusTask*, ModbusAdapter*)> function1 = [this](int ret, ModbusTask* task, ModbusAdapter* adapter) {
		if (ret == -1)
			return;
		uint16_t* readData = adapter->GetReadedData16();
		m_ncMachine->ConvertModbusData4Pos(task, readData);

		uint16_t* readData2 = &readData[TMBS_MAP0_ID_XPOS_LEN * 4];
		m_ncMachine->ConvertModbusData4State(task, readData2);
		};
	ModbusTask* task1 = m_modbus->getTaskRead(TMBS_MAP0_ID_XPOS, TMBS_MAP0_ID_XPOS_LEN * POS_AXIS_LEN + TMBS_MAP0_ID_RSLT_LEN + TMBS_MAP0_ID_NCTSTATE_LEN + TMBS_MAP0_ID_SCAMANUL_LEN + TMBS_MAP0_ID_INPUT_LEN);
	task1->setPostFunction(function1, "Read Pos & State");
	m_modbus->addTaskAsNormal(task1, 100, true);

	//std::function<void(int, ModbusTask*, ModbusAdapter*)> function3 = [this](int ret, ModbusTask* task, ModbusAdapter* adapter) {
	//    if (ret == -1)
	//        return;
	//    uint16_t* readData = adapter->GetReadedData16();
	//    m_ncMachine->ConvertModbusDataResult(task, readData);
	//};
	//m_modbus->addTaskAsNormal(m_modbus->addTaskRead(TMBS_MAP0_ID_RSLT, TMBS_MAP0_ID_RSLT_LEN + TMBS_MAP0_ID_NCTSTATE_LEN + TMBS_MAP0_ID_SCAMANUL_LEN + TMBS_MAP0_ID_INPUT_LEN, function3), 100, false);

	std::function<void(int, ModbusTask*, ModbusAdapter*)> function2 = [this](int ret, ModbusTask* task, ModbusAdapter* adapter) {
		if (ret == -1)
			return;
		uint16_t* readData = adapter->GetReadedData16();
		m_ncMachine->ConvertModbusData4Key(task, readData);
		};
	ModbusTask* task2 = m_modbus->getTaskRead(TMBS_MAP0_ID_KEY, TMBS_MAP0_ID_KEY_LEN + TMBS_MAP0_ID_HOPR_LEN);
	task2->setPostFunction(function2, "Read Key");
	m_modbus->addTaskAsNormal(task2, 50, false);

	std::function<void(int, ModbusTask*, ModbusAdapter*)> function3 = [this](int ret, ModbusTask* task, ModbusAdapter* adapter) {
		if (ret == -1)
			return;
		uint16_t* readData = adapter->GetReadedData16();
		m_ncMachine->ConvertModbusData4JogDuanlu(task, readData);
		};
	ModbusTask* task3 = m_modbus->getTaskRead(TMBS_MAP0_ID_JOG_DUANLU, TMBS_MAP0_ID_JOG_DUANLU_LEN);
	task3->setPostFunction(function3, "Read Jog Duanlu");
	m_modbus->addTaskAsNormal(task3, 100, false);
}

void BaseMainWindow::showChildWindow(QString windowTitle)
{
	auto buttons = this->findChildren<QToolButton*>();
	for (QToolButton* button : buttons) {
		if (button->text() == windowTitle) {
			button->click();
			break;
		}
	}
}