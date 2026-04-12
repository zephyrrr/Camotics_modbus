#include <QToolButton>
#include <QToolBar>
#include <QDateTime>
#include <QFile>
#include <QCoreApplication>
#include <cbang/io/LineBufferStream.h>
#include <cbang/os/SystemUtilities.h>
#include <QtSerialPort/QSerialPortInfo>
#include "../modbus/NCMachineProperties.h"
#include "mainwindow2.h"
#include "./ui_mainwindow2.h"
#include "manual/weizhiyidongform.h"
#include "manual/zuobiaoxishezi.h"
#include "manual/weizhijiyi.h"
#include "manual/duanmianform.h"
#include "manual/zhuzhongxingform.h"
#include "manual/jiaodingweiform.h"
#include "manual/kongzhongxingform.h"
#include "manual/sandiandingwei.h"
#include "manual/fangdianform.h"
#include "manual/mdiform.h"
#include "edit/nceditform.h"
#include "auto/runautoform.h"
#include "../forms_v1/settingsmodbusrtu.h"
#include "../forms_v1/NCMachinePanel.h"
#include "other/luojubuchangform.h"
#include "other/fangdiancheshiform.h"
#include "other/fangdiancanshuform.h"
#include "other/xitongshezhiform.h"
#include "other/huilingform.h"
#include "other/about.h"
#include "other/jiagongjiluform.h"
#include "run/runmanual.h"
#include "run/runmanualmulti.h"
#include "run/ncrun.h"
#include "qtquickwindow.h"
#include "../utils/PythonQtRuntime.h"
#include "utils/formutils.h"
#include "utils/dataform.h"
#include "utils/pluginutils.h"
#include "utils/MathTool.h"
#include "component/gaotiao4jiagong.h"
#include "component/containerchildwindow.h"
#include "component/pythonpluginchildwindow.h"
#include "../../widgets/advancedtoolbox.h"
#include "widgets/menuwith2frames.h"
#include "modbus/NCMachineParametersC.h"
#include "widgets/filedialog.h"
#include "utils/nfile.h"
#include "multibuttonswidget.h"

//#pragma execution_character_set("utf-8")

#define LOGGER_FILE_NAME "logs/logger.txt"

ModbusMain::ModbusMain(QObject* parent)
	: m_parent(parent)
{
	//m_regModel = new RegistersModel(this);
	if (true || cb::Logger::instance().getVerbosity() >= 10) {
		m_rawModel = new RawDataModel(m_parent);
		m_rawModel->enableAddLines(true);
		m_parent->connect(m_rawModel, &RawDataModel::lineAdded, [](QString line) {
			LOG_INFO(10, EUtils::QString2StdString(line));
			});
	}

	m_modbusCommSettings = new ModbusCommSettings("data/qModMaster.ini");
	m_modbus = new ModbusAdapter(m_parent, m_regModel, m_rawModel);
	m_ncMachine = new NCMachine(m_parent, m_modbus);

	modbusConnect(true);

	if (m_modbus->isConnected()) {

	}
}

ModbusMain::~ModbusMain()
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

void ModbusMain::modbusConnect(bool connect)
{
	modbusConnect(connect, m_modbus, m_modbusCommSettings, m_ncMachine);
}
void ModbusMain::modbusConnect(bool connect, ModbusAdapter* modbus, ModbusCommSettings* modbusCommSettings, NCMachine* ncMachine)
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

void ModbusMain::addNormalTasks()
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

QtWin2::QtWin2(QWidget* parent)
	: ModbusMain(parent), BaseMainWindow(parent)
	, ui(new Ui::QtWin2)
{
	bool isPythonOk = PythonQtRuntime::getInstance()->Init();
	
	if (isPythonOk) {
		//PythonQt::self()->registerCPPClass("NCMachine", "QObject", "", PythonQtCreateObject<NCMachineWrapper>, NULL);
		//PythonQtRuntime::registerCPPClass<NCMachineWrapper>("NCMachine", "QObject");
		PythonQt::self()->registerClass(&NCMachine::staticMetaObject, "Qt", PythonQtCreateObject<NCMachineWrapper>, nullptr);

		//PythonQtRuntime::registerCPPClass<BaseChildWindowWrapper>("BaseChildWindow", "QWidget");
		PythonQt::self()->registerClass(&BaseChildWindow::staticMetaObject, "QtGui", PythonQtCreateObject<BaseChildWindowWrapper>, nullptr);
		PythonQt::self()->registerClass(&BaseChildWindowWithTools::staticMetaObject, "QtGui", PythonQtCreateObject<BaseChildWindowWithToolsWrapper>, nullptr);
		PythonQt::self()->registerClass(&Tool4All::staticMetaObject, "QtGui", PythonQtCreateObject<Tool4AllWrapper>, nullptr);

		//PythonQtRuntime::registerCPPClass<QLineEdit4AxisValueWrapper>("QLineEdit4AxisValue", "QLineEdit");
		PythonQt::self()->registerClass(&QLineEdit4AxisValue::staticMetaObject, "QtGui", PythonQtCreateObject<QLineEdit4AxisValueWrapper>, nullptr);
		//PythonQt::self()->registerQObjectClassNames(QStringList() << "QLineEdit4AxisValue");
		PythonQt::self()->registerClass(&QLineEdit4Int::staticMetaObject, "QtGui", PythonQtCreateObject<QLineEdit4IntWrapper>, nullptr);
		PythonQt::self()->registerClass(&QLineEdit4Keyboard::staticMetaObject, "QtGui", PythonQtCreateObject<QLineEdit4KeyboardWrapper>, nullptr);

		PythonQt::self()->registerClass(&ButtonEditTableWidget::staticMetaObject, "QtGui", PythonQtCreateObject<ButtonEditTableWidgetWrapper>);
		PythonQt::self()->registerClass(&MultiButtonsWidget::staticMetaObject, "QtGui", PythonQtCreateObject<MultiButtonsWidgetWrapper>, nullptr);

	}

	cb::SmartPointer<cb::LineBufferStream<LineLogger>> consoleStream = new cb::LineBufferStream<LineLogger>(LineLogger::instance());
	cb::Logger::instance().setScreenStream(consoleStream);
	cb::Logger::instance().setLogCRLF(false);
	cb::Logger::instance().setLogDate(true);
	cb::Logger::instance().setVerbosity(8);  // 8 // 10
	cb::Logger::instance().setLogDebug(true);
	cb::Logger::instance().setLogRotateMax(30);
	//cb::Logger::instance().setScreenStream(NULL);
	cb::Logger::instance().startLogFile(LOGGER_FILE_NAME);

	NFile::cleanStaleTempFiles("data");
	NFile::cleanStaleTempFiles(SystemSettings::instance().GetProjectDir());
	NFile::cleanStaleTempFiles("data/nc");

	PropertyObjects::getInstance()->CreateData();
	PropertyObjects::getInstance()->LoadData();

	ui->setupUi(this);

	//m_displayLoran->draw();
	m_displayLoran = new DisplayLoran(this);
	auto layout = new QVBoxLayout(ui->loran);
	layout->setAlignment(Qt::AlignCenter);
	layout->setMargin(0);
	ui->loran->setMaximumSize(250, 250);
	ui->loran->setFixedSize(250, 250);
	layout->addWidget(m_displayLoran);
	m_displayLoran->draw();

	m_displayZ= new DisplayZ(this);
	layout = new QVBoxLayout(ui->displayz);
	layout->setAlignment(Qt::AlignCenter);
	layout->setMargin(0);
	ui->displayz->setMaximumSize(400, 250);
	ui->displayz->setFixedSize(400, 250);
	layout->addWidget(m_displayZ);
	m_displayZ->draw();

	//connect(m_displayLoran, &DisplayLoran::clicked, [this]() {
	//	m_displayLoran->hide();
	//	m_displayZ->show();
	//	});
	//connect(m_displayZ, &DisplayZ::clicked, [this]() {
	//	m_displayZ->hide();
	//	m_displayLoran->show();
	//	});

	m_buttonGroup4Menu = new QButtonGroup(this);

	showTaskbar();
    //ui->actionTouchSense->setCheckable(true);
	deserialize();

    QDateTime now = QDateTime::currentDateTime();
    //QString dateStr = now.toString("yy-MM-dd\nhh:mm:ss");
    ui->lblNowDate->setText(now.toString("yy-MM-dd"));
	ui->lblNowTime->setText(now.toString("hh:mm:ss"));

	ui->lblResultMessage->setText("");
	//ui->labelFileName->setText(getProjectName());
	ui->btnFileName->setText(QDir(SystemSettings::instance().GetProjectDir()).dirName());

	connect(ui->btnFileName, &QToolButton::clicked, [this](bool checked) {
		//NFileDialog* dialog = new NFileDialog(this, SystemSettings::instance().GetProjectDir(), true);
		//dialog->showNormal();
		return;

		QString chosenDir = NFileDialog::getExistingDirectory(this, tr("XMML"),
		"data",
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks ); // | QFileDialog::DontUseNativeDialog
		chosenDir = QDir::cleanPath(chosenDir);
		SystemSettings::instance().SetProjectDir(QDir::currentPath() + QDir::separator() + chosenDir);
		ui->btnFileName->setText(QDir(SystemSettings::instance().GetProjectDir()).dirName());
	});

	auto ljbcForm = new LuoJuBuChangForm(this->ui->stackedChildWidget);
	auto fdcsForm = new FangDianCheShiForm(this->ui->stackedChildWidget);
	auto xtszForm = new XiTongSheZhiForm(this->ui->stackedChildWidget);

	auto ncEditForm = new NcEditForm(this->ui->stackedChildWidget);

	const int MAIN_MENU_TYPE = 2;

	if (MAIN_MENU_TYPE == 2) {
		QFrame* subMenuWidget = new QFrame(this);
		subMenuWidget->setObjectName("subMenuWidget");
		MenuWith2Frames* mainMenuWidget = new MenuWith2Frames(subMenuWidget, this);
		mainMenuWidget->setObjectName("mainMenuWidget");
		ui->verticalLayout_groupBoxMenus->addWidget(mainMenuWidget);
		//ui->verticalLayout_groupBoxMenus->addSpacerItem(new QSpacerItem(0, 10, QSizePolicy::Minimum, QSizePolicy::Minimum));
		QLabel* labelMenuSepartor = new QLabel(mainMenuWidget);
		labelMenuSepartor->setObjectName("labelMenuSepartor");
		ui->verticalLayout_groupBoxMenus->addWidget(labelMenuSepartor);
		ui->verticalLayout_groupBoxMenus->addWidget(subMenuWidget);
		ui->verticalLayout_groupBoxMenus->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

		mainMenuWidget->addItem(tr("SD"));
		mainMenuWidget->addItem(tr("EZD"));
		mainMenuWidget->addItem(tr("BianJi"));
		mainMenuWidget->addItem(tr("JG"));
		mainMenuWidget->addItem(tr("SheDing"));

		mainMenu = mainMenuWidget;
	}
	else if (MAIN_MENU_TYPE == 1) {
		//mainMenuWidget = new QToolBox(this);
		AdvancedToolBox* mainMenuWidget = new AdvancedToolBox(this);
		mainMenuWidget->setObjectName("mainMenuWidget");
		//mainMenuWidget->setBaseSize(0, 2222);
		QFont textFont = mainMenuWidget->font();
		textFont.setPointSize(26);
		mainMenuWidget->setFont(textFont);
		ui->verticalLayout_groupBoxMenus->addWidget(mainMenuWidget);

		mainMenuWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
		//mainMenuWidget->addItem(new QWidget(mainMenuWidget), tr("CY"));
		mainMenuWidget->addItem(new QWidget(mainMenuWidget), tr("SD"));
		mainMenuWidget->addItem(new QWidget(mainMenuWidget), tr("ZD"));
		mainMenuWidget->addItem(new QWidget(mainMenuWidget), tr("BianJi"));
		mainMenuWidget->addItem(new QWidget(mainMenuWidget), tr("YX"));
		mainMenuWidget->addItem(new QWidget(mainMenuWidget), tr("QT"));

		mainMenu = mainMenuWidget;
	}

	//for (const QString& var : { tr("CY"), tr("SD"), tr("BJ") ,tr("YX") ,tr("QT") })
	for(int pageIdx=0; pageIdx < 5; ++pageIdx)
	{
		QBoxLayout* layout1 = NULL;
		if (MAIN_MENU_TYPE == 1) {
			QWidget* page1 = dynamic_cast<AdvancedToolBox*>(mainMenu)->widget(pageIdx);
			layout1 = new QVBoxLayout(page1);
		}
		else if (MAIN_MENU_TYPE == 2) {
			layout1 = dynamic_cast<MenuWith2Frames*>(mainMenu)->getSubMenuLayout(pageIdx);
		}
		m_layouts.append(layout1);
		if (pageIdx == -1) {
			for (const QString& str : m_historyMenus)
			{
				AddChildWindow(str, layout1, (std::function<BaseChildWindow * ()>)NULL);
			}
		}
		if (pageIdx == 0) {
			//AddChildWindow(QStringLiteral(QT_TR_NOOP_UTF8("坐标设置")), layout1, [this]() {
			//	return new ZuoBiaoXiSheZiForm(this->ui->stackedChildWidget);
			//	});
			AddChildWindow(tr("ZBSZ"), layout1, [this]() {
				return new ZuoBiaoXiSheZiForm(this->ui->stackedChildWidget);
				});
			AddChildWindow(tr("DWYD"), layout1, [this]() {
				return new WeiZhiYiDongForm(this->ui->stackedChildWidget);
				});
			AddChildWindow(tr("WZJY"), layout1, [this]() {
				return new WeiZhiJiYiForm(this->ui->stackedChildWidget);
				});
			AddChildWindow(tr("DMDW"), layout1, [this]() {
				return new DuanMianForm(this->ui->stackedChildWidget);
				});
			AddChildWindow(tr("ZZX"), layout1, [this]() {
				return new ZhuZhongXingForm(this->ui->stackedChildWidget);
				});
			AddChildWindow(tr("JZX"), layout1, [this]() {
				return new JiaoDingWeiForm(this->ui->stackedChildWidget);
				});
			AddChildWindow(tr("KZX"), layout1, [this]() {
				return new KongZhongXingForm(this->ui->stackedChildWidget);
				});
			AddChildWindow(tr("SDDY"), layout1, [this]() {
				return new SanDianDingWeiForm(this->ui->stackedChildWidget);
				});
			AddChildWindow(tr("FDDW"), layout1, [this]() {
				return new FangDianForm(this->ui->stackedChildWidget);
				});
			//AddChildWindow(QString("MDI"), layout1, [this]() {
			//	return new MdiForm(this);
			//	});
		} 
		else if (pageIdx == 1) {
			AddChildWindow(tr("ZDCS"), layout1, [this]() {
				return new RunAutoForm(this->ui->stackedChildWidget);
				});
		}
		else if (pageIdx == 2) {
			//AddChildWindow(QString("NC"), layout1, [this]() {
			//	return new NcEditForm(this->ui->stackedChildWidget);
			//	});
			AddChildWindow(QString("NC"), layout1, ncEditForm);
		}
		else if (pageIdx == 3) {
			AddChildWindow(tr("SDDG"), layout1, [this]() {
				return new RunManual(this->ui->stackedChildWidget);
				});
			AddChildWindow(tr("SDMG"), layout1, [this]() {
				return new RunManualMulti(this->ui->stackedChildWidget);
				});
			//AddChildWindow(tr("YXDG"), layout1, [this]() {
			//	return new RunManual(this);
			//	});
			//AddChildWindow(tr("LWJG"), layout1, [this]() {
			//	return new RunManual(this);
			//	});
			AddChildWindow(tr("NJG"), layout1, [this]() {
				return new NcRunForm(this->ui->stackedChildWidget);
				});
		}
		else if (pageIdx == 4) {
			AddChildWindow(tr("JGJL"), layout1, [this]() {
				return new JiaGongJiLuForm(this->ui->stackedChildWidget);
				});

			AddChildWindow(tr("XTSZ"), layout1, xtszForm);
			AddChildWindow(tr("FangDianCanShu"), layout1, [this]() {
				return new FangDianCanShuForm(this->ui->stackedChildWidget);
				});
			AddChildWindow(tr("FDCS"), layout1, fdcsForm);
			AddChildWindow(tr("LJBC"), layout1, ljbcForm);
			//AddChildWindow(tr("HL"), layout1, [this]() {
			//	//return new NCMachinePanel(this, m_ncMachine, m_modbus, m_modbusCommSettings);
			//	return new HuiLingForm(this);
			//	});
			AddChildWindow(tr("CKSZ"), layout1, [this]() {
				return new SettingsModbusRTU(this->ui->stackedChildWidget, m_modbusCommSettings);
				});
			AddChildWindow(tr("GY"), layout1, [this]() {
				return new AboutForm(this->ui->stackedChildWidget);
				});

		}

		layout1->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
	}

	LoadPluginsAsChildWindow();
	
	if (MAIN_MENU_TYPE == 1)
	{
		AdvancedToolBox* mainMenuWidget = dynamic_cast<AdvancedToolBox*>(mainMenu);
		//mainMenuWidget->setItemVisible(0, false);
		mainMenuWidget->setItemExpand(0, true);
		mainMenuWidget->setItemExpand(1, true);
		mainMenuWidget->setItemExpand(2, true);
		mainMenuWidget->setItemExpand(3, true);
		auto x = mainMenuWidget->widget(0)->findChildren<QToolButton*>();
		if (x.count() > 0) {
			x[0]->click();
		}
	}
	else if (MAIN_MENU_TYPE == 2)
	{
		MenuWith2Frames* mainMenuWidget = dynamic_cast<MenuWith2Frames*>(mainMenu);
		auto x = mainMenuWidget->findChildren<QAbstractButton*>();
		if (x.count() > 0) {
			x[0]->click();
		}
	}


	//connect(ui->actionRun, SIGNAL(triggered()), this, SLOT(on_actionRun_triggered()));
	//connect(ui->actionCancel, SIGNAL(triggered()), this, SLOT(on_actionCancel_triggered()));

	//this->showMaximized();


	if (!m_modbus->isConnected()) {
		m_serialPortNeedReconnect = true;
		//ui->CNCstate->setText(tr("BJ"));
		ui->lblResultMessage->setText(tr("CKWLJ"));
		ui->actionRun->setEnabled(false);
		ui->actionCancel->setEnabled(false);
	}
	else {
		ui->lblResultMessage->setText(tr("DDYX"));

		addNormalTasks();

		{
			std::function<void(int, ModbusTask*, ModbusAdapter*)> function = [](int ret, ModbusTask*, ModbusAdapter* adapter) {
				if (ret == -1)
					return;
				uint16_t* readData = adapter->GetReadedData16();
				PropertyObjects::getInstance()->propertyObjectVersion->SetValues1(std::vector<uint16_t> {  readData[0], readData[1]});
				};
			ModbusTask* task = m_modbus->getTaskRead(TMBS_MAP0_ID_HANDVER, TMBS_MAP0_ID_HANDVER_LEN + TMBS_MAP0_ID_DISPVER_LEN);
			task->setPostFunction(function, "Read Haredware Version");
			m_modbus->addTask(task, 0);
		}
		{
			std::function<void(int, ModbusTask*, ModbusAdapter*)> function = [](int ret, ModbusTask*, ModbusAdapter* adapter) {
				if (ret == -1)
					return;
				uint16_t* readData = adapter->GetReadedData16();
				PropertyObjects::getInstance()->propertyObjectVersion->SetValues2(std::vector<uint16_t> {  readData[0], readData[1] });
				};
			ModbusTask* task = m_modbus->getTaskRead(TMBS_MAP0_ID_HANDVER2, TMBS_MAP0_ID_HANDVER2_LEN);
			task->setPostFunction(function, "Read Haredware Version 2");
			m_modbus->addTask(task, 0);
		}

		xtszForm->LoadData();
		xtszForm->RunGCode();	// run on poweron(), will not load data

		fdcsForm->LoadData();
		fdcsForm->RunGCode();

		m_ncMachine->enterSetPriority(0);
		PropertyObjects::getInstance()->propertyObjectFdjg->ExecuteCmdsSetSv(m_ncMachine, 3);
		PropertyObjects::getInstance()->propertyObjectYouwenJianche->ExecuteCmds(m_ncMachine);
		m_ncMachine->Beep(0);
		m_ncMachine->exitSetPriority();
		
		ljbcForm->LoadData();
		ljbcForm->RunGCode();

		connect(m_ncMachine, &NCMachine::keyPressed, [this](unsigned long long key) {
			if (key == KNLK_START) {
				if (ui->actionRun->isEnabled()) {
					//G01Data& g01Data = m_ncMachine->GetG01Data();
					//g01Data.isSimulate = false;
					ui->actionRun->trigger();

					m_ncMachine->enterSetPriority(0);
					m_ncMachine->Beep();
					m_ncMachine->exitSetPriority();
				}
				else {
					G01Data& g01Data = m_ncMachine->GetG01Data();
					if (g01Data.isRunning) {
						m_ncMachine->ResumeRun();
					}
					else {
						if (m_ncMachine->isTaskThreadPausing()) {
							m_ncMachine->ResumeRun();
						}
						else {
							m_ncMachine->enterSetPriority(0);
							m_ncMachine->Beep(2);
							m_ncMachine->exitSetPriority();
						}
					}
				}
			}
			else if (key == KNLK_STOP) {
				if (ui->actionCancel->isEnabled()) {
					
					m_ncMachine->enterSetPriority(0);
					m_ncMachine->Beep();
					m_ncMachine->exitSetPriority();

					G01Data& g01Data = m_ncMachine->GetG01Data();
					if (g01Data.isRunning) {
						if (!g01Data.isPausing && g01Data.cNo != SPK_CNO_SPECIAL) {
							m_ncMachine->PauseRun();
						}
						else {
							ui->actionCancel->trigger();
						}
					}
					else {
						ui->actionCancel->trigger();
					}
					
				}
				else {
					m_ncMachine->enterSetPriority(0);
					m_ncMachine->Beep(2);
					m_ncMachine->exitSetPriority();

					LineLogger::instance().clear();
				}
			}
			});

		if (m_isUserMode) {
			QTimer::singleShot(500, this, [this] {
				ui->actionPowerOn->trigger();
				});
		}
	}

	auto csNames = GCodeTool::csNames;
	int csIndex = m_ncMachine->GetController()->get(CURRENT_COORD_SYSTEM) - 1;
	ui->btnCoor->setText(QString("G%1%2").arg(csIndex / 6).arg(csNames[csIndex % 6]));

	m_g54menu.setObjectName("btnCoorMenu");
	for (int i = 0; i < 6; ++i) {
		QAction* action = new QAction(QString::number(i), &m_g54menu);
		action->setData(QVariant(i + 1));
		connect(action, &QAction::triggered, [this, action](bool checked) {
			int itemNumber = action->data().toInt() - 1;
			int csIndex = m_ncMachine->GetController()->get(CURRENT_COORD_SYSTEM) - 1;
			m_ncMachine->RunGCode(QString("G%1%2").arg(csIndex / 6).arg(GCodeTool::csNames[itemNumber]));
			});
		m_g54menu.addAction(action);
	}
	connect(ui->btnCoor, &QPushButton::clicked, [this]() {
		auto csNames = GCodeTool::csNames;
		int csIndex = m_ncMachine->GetController()->get(CURRENT_COORD_SYSTEM) - 1;
		for (int i = 0; i < 6; ++i) {
			m_g54menu.actions().at(i)->setText(QString("G%1%2").arg(csIndex / 6).arg(csNames[i]));
		}
		m_g54menu.exec(ui->btnCoor->mapToGlobal(ui->btnCoor->pos() + QPoint(ui->btnCoor->width() / 2, 0)));
		});
	connect(ui->btnYouWei, &QPushButton::toggled, [this](bool checked) {
		PropertyObjects::getInstance()->propertyObjectYouwenJianche->setYouWei(checked);
		PropertyObjects::getInstance()->propertyObjectYouwenJianche->ExecuteCmds(m_ncMachine, 0);
		});
	connect(ui->btnYouWen, &QPushButton::toggled, [this](bool checked) {
		PropertyObjects::getInstance()->propertyObjectYouwenJianche->setYouWen(checked);
		PropertyObjects::getInstance()->propertyObjectYouwenJianche->ExecuteCmds(m_ncMachine, 0);
		});
	connect(ui->btnHuoYan, &QPushButton::toggled, [this](bool checked) {
		PropertyObjects::getInstance()->propertyObjectYouwenJianche->setHuoYan(checked);
		PropertyObjects::getInstance()->propertyObjectYouwenJianche->ExecuteCmds(m_ncMachine, 0);
		});

	ui->btnT0->setEnabled(false);
	ui->btnST->setEnabled(false);
	ui->btnT1->setEnabled(false);
	ui->btnFloat->setEnabled(false);
	ui->btnYouWei->setEnabled(true);
	ui->btnYouWen->setEnabled(true);
	ui->btnHuoYan->setEnabled(true);
	ui->btnT0->setCheckable(true);
	ui->btnST->setCheckable(true);
	ui->btnT1->setCheckable(true);
	ui->btnFloat->setCheckable(true);
	ui->btnYouWei->setCheckable(true);
	ui->btnYouWen->setCheckable(true);
	ui->btnHuoYan->setCheckable(true);

	ui->btnFloat->hide();

	ui->labelInfoFangdian->setEnabled(false);
	ui->labelInfoLianjie->setEnabled(false);

	for (int i = 0; i < 5; ++i)
	{
		QPushButton* btn = new QPushButton(ui->topinfobar_shortcut);
		btn->setCheckable(true);
		btn->setChecked(false);
		m_topinfobarButtons.append(btn);
		ui->horizontalLayout_topinfobar_shortcut->insertWidget(10, btn);
	}

	//m_topinfobarButtons[0]->setCheckable(false);
	m_topinfobarButtons[0]->setText(tr("MNJG"));
	connect(m_topinfobarButtons[0], &QPushButton::clicked, [this]() {
		//if (ui->actionRun->isEnabled()) {
			G01Data& g01Data = m_ncMachine->GetG01Data();
			g01Data.isSimulate = m_topinfobarButtons[0]->isChecked();
			//ui->actionRun->trigger();
		//}
		});
	m_topinfobarButtons[1]->setText(tr("JGHGJ"));
	connect(m_topinfobarButtons[1], &QPushButton::clicked, [this]() {
			G01Data& g01Data = m_ncMachine->GetG01Data();
			g01Data.powerOffAfterDone = !g01Data.powerOffAfterDone;
			m_topinfobarButtons[1]->setChecked(g01Data.powerOffAfterDone);
		});

	if (true) {
		showWindowMode(m_isUserMode);

		QLineEdit* lineEdit = new QLineEdit(ui->toolBar);
		lineEdit->setText("4000");
		lineEdit->setMaximumWidth(50);
		ui->toolBar->insertWidget(ui->actionTouchSense2, lineEdit);
		lineEdit4TouchSense2 = lineEdit;
	}

	ui->labelManufactoryIcon->hide();
	ui->widgetInfoIcons->setLayoutDirection(Qt::RightToLeft);

	connect(m_modbus, SIGNAL(refreshView()), this, SLOT(UpdateState()));

	m_pollTimer = new QTimer(this);
	connect(m_pollTimer, SIGNAL(timeout()), this, SLOT(UpdateStateByTimer()));
	m_pollTimer->start(1000);

	JiaGongJiLuForm::createDb();

	//auto x = this->findChildren<QToolButton*>();
	//for (QToolButton* button : x) {
	//	if (button->objectName().isEmpty()) {
	//		auto sss = button->text();
	//		auto xy = button->pos();
	//		auto pp = button->parentWidget()->objectName();
	//		button->setStyleSheet("background-color: lightblue;");
	//	}
	//	else {
	//	}
	//}
}


QtWin2::~QtWin2()
{
	if (m_pollTimer->isActive()) {
		m_pollTimer->stop();
	}

	disconnect(m_modbus, SIGNAL(refreshView()), this, SLOT(UpdateState()));

	showTaskbar();

	serialize();

	delete ui;
	if (m_dlgRegWindow)
		delete m_dlgRegWindow;
	//if (m_rawModel)
	//	delete m_rawModel;
	if (m_formFangDianCheShi)
		delete m_formFangDianCheShi;
	if (m_formGaotiao4Jiagong)
		delete m_formGaotiao4Jiagong;
	if (m_dialog4FangDianCheShi)
		delete m_dialog4FangDianCheShi;	
	if (m_dialog4Gaotiao)
		delete m_dialog4Gaotiao;

	//m_ncMachine->PowerOff();
}

bool QtWin2::serialize()
{
	//QString fileName = "data/history_menu.txt";
	//QFile file(fileName);
	//if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	//	return false;

	//QTextStream out(&file);
	//out.setCodec("UTF-8");
	//for (const QString& str : m_historyMenus.mid(0, 10))
	//{
	//	out << str << "\n";
	//}

	QJsonObject jsonObj;
	QJsonArray jsonArray;

	//for (const QString& item : m_historyMenus.mid(0, 10)) {
	//	jsonArray.append(item);
	//}
	//jsonObj["history"] = jsonArray;
	jsonObj["active_project"] = SystemSettings::instance().GetProjectDir();
#ifdef _DEBUG
	jsonObj["is_usermode"] = m_isUserMode ? "true" : "false";
#else
	jsonObj["is_usermode"] = "true";
#endif // DEBUG

	QJsonDocument jsonDoc(jsonObj);
	NFile file("data/settings.json");
	if (file.open(QIODevice::WriteOnly)) {
		file.write(jsonDoc.toJson());
		file.close();
	}

	return true;
}

bool QtWin2::deserialize()
{
	QFile file("data/settings.json");
	if (!file.open(QIODevice::ReadOnly)) {
		return false;
	}

	QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
	QJsonObject jsonObj = jsonDoc.object();

	//QJsonArray jsonArray = jsonObj["history"].toArray();
	//QHash<QString, QString> alreadyHave;
	//for (const QJsonValue& value : jsonArray) {
	//	QString str = value.toString();
	//	if (alreadyHave.contains(str))
	//		continue;
	//	m_historyMenus.append(str);
	//}
	//m_historyMenus = m_historyMenus.mid(0, 10);

	QString projectDir = jsonObj["active_project"].toString();
	if (projectDir.startsWith(QDir::currentPath())) {
		QDir dir(projectDir);
		if (dir.exists()) {
			SystemSettings::instance().SetProjectDir(projectDir);
		}
	}
	
#ifdef _DEBUG
	QString str = jsonObj["is_usermode"].toString();
	if (!str.isEmpty()) {
		if (str == "true")
			m_isUserMode = true;
		else if (str == "false")
			m_isUserMode = false;
	}
#else
	m_isUserMode = true;
#endif // !_DEBUG

	return true;
}

void QtWin2::showWindowMode(bool isUserMode)
{
	if (isUserMode) {
		ui->statusbar->hide();
		ui->toolBar->hide();
		setWindowFlag(Qt::FramelessWindowHint);
		hideTaskbar();
		showFullScreen();
	}
	else {
		//setWindowState(Qt::WindowNoState);
		ui->statusbar->show();
		ui->toolBar->show();
		setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
		//setWindowState(Qt::WindowMaximized);
		showTaskbar();
		showMaximized();
	}

	//m_buttonsText[tr("XTSZ")]->setVisible(!m_isUserMode);
	//m_buttonsText[tr("FDCS")]->setVisible(!m_isUserMode);
	//m_buttonsText[tr("FangDianCanShu")]->setVisible(!m_isUserMode);
	//m_buttonsText[tr("LJBC")]->setVisible(!m_isUserMode);
	//m_buttonsText[tr("CKSZ")]->setVisible(!m_isUserMode);
}

void QtWin2::addNormalTasks()
{
	std::function<void(int, ModbusTask*, ModbusAdapter*)> function1 = [this](int ret, ModbusTask* task, ModbusAdapter* adapter) {
		if (ret == -1)
			return;
		uint16_t* readData = adapter->GetReadedData16();
		G01Data& g01Data = m_ncMachine->GetG01Data();
		g01Data.dssysj = readData[0] + (readData[1] << 16);
		g01Data.jgcyl = readData[2] + (readData[3] << 16);
		};
	ModbusTask* task1 = m_modbus->getTaskRead(TMBS_MAP0_ID_REG79, 4);
	task1->setPostFunction(function1, "Read Dssysj&Jgcyl");	// // 定时剩余时间,加工残余量
	m_modbus->addTaskAsNormal(task1, 100, true);

	ModbusMain::addNormalTasks();
}

void QtWin2::showChildWindow(QString windowTitle)
{
	MenuWith2Frames* mainMenuWidget = dynamic_cast<MenuWith2Frames*>(mainMenu);
	if (mainMenuWidget != NULL) {
		bool hasOpened = false;
		for (int i = 0; i < 5; ++i) {
			if (hasOpened)
				break;
			QBoxLayout* layout1 = mainMenuWidget->getSubMenuLayout(i);
			if (layout1 == NULL)
				break;
			auto buttons = layout1->parentWidget()->findChildren<QToolButton*>();
			for (QToolButton* button : buttons) {
				if (button->text() == windowTitle) {
					mainMenuWidget->openItem(i);
					hasOpened = true;
					break;
				}
			}
		}
	}

	BaseMainWindow::showChildWindow(windowTitle);
}

void QtWin2::showEvent(QShowEvent* event)
{
	if (m_isUserMode && ui->statusbar->isVisible())
		showWindowMode(m_isUserMode);
	//else if (!m_showFullScreen)
	//	showWindowMode(m_showFullScreen);

	BaseMainWindow::showEvent(event);
}

void QtWin2::on_actionRun_triggered()
{
	LineLogger::instance().clear();

	BaseChildWindow* currentChild = dynamic_cast<BaseChildWindow*>(ui->stackedChildWidget->currentWidget());

	if (!currentChild)
		return;

	try {
		currentChild->HideKeyboard();

#ifdef PYTHON_SUPPORT
		QString childName = currentChild->objectName();

		QMap<QString, QString> pyFiles = PluginUtils::loadPythonScripts(childName);
		for (const auto& pyFileName : pyFiles.keys()) {
			if (pyFileName == "init") {
				QString value = pyFiles.value(pyFileName);
				PluginUtils::RunFile(value, currentChild);

				ui->actionRun->setEnabled(false);
				ui->lblResultMessage->setText(tr("YXZ"));
				return;
				//break;
			}
		}
#endif

		currentChild->RunGCode();

		//QThread::create([currentChild] {
		//	currentChild->RunGCode();
		//	})->start();

		ui->actionRun->setEnabled(false);
		ui->lblResultMessage->setText(tr("YXZ"));
	}
	catch (const std::exception& e) {
		ui->lblResultMessage->setText(e.what());
	}
}

void QtWin2::on_actionCancel_triggered()
{
	this->m_ncMachine->StopRun();
	LineLogger::instance().clear();
	ui->lblResultMessage->setText("");
}

void QtWin2::on_actionDebug_triggered()
{
	//QString pw = FormUtils::ShowPasswordInput();

	//if (m_ncMachine->GetG01Data().isRunning) {

	//	if (!m_ncMachine->GetG01Data().isPausing)
	//		m_ncMachine->PauseRun();
	//	else
	//		m_ncMachine->PauseRun();
	//	return;
	//}

	OpenRegWindow();
}

void QtWin2::on_actionTouchSense_triggered()
{
	m_ncMachine->enterSetPriority(0);
    PropertyObjects::getInstance()->propertyObjectDebug->setdl(-1);
    PropertyObjects::getInstance()->propertyObjectDebug->ExecuteCmds(m_ncMachine);

	PropertyObjects::getInstance()->propertyObjectDebug->setdl(0);
	PropertyObjects::getInstance()->propertyObjectDebug->ExecuteCmds(m_ncMachine);
	m_ncMachine->exitSetPriority();
}

void QtWin2::on_actionTouchSense2_triggered()
{
	if (ui->actionTouchSense2->isChecked()) {
		int dl = lineEdit4TouchSense2->text().toInt();
		//ui->actionTouchSense2->setChecked(true);
		m_ncMachine->enterSetPriority(0);
		PropertyObjects::getInstance()->propertyObjectDebug->setdl(dl);
		PropertyObjects::getInstance()->propertyObjectDebug->ExecuteCmds(m_ncMachine);
		m_ncMachine->exitSetPriority();
	}
	else {
		//ui->actionTouchSense2->setChecked(false);
		m_ncMachine->enterSetPriority(0);
		PropertyObjects::getInstance()->propertyObjectDebug->setdl(0);
		PropertyObjects::getInstance()->propertyObjectDebug->ExecuteCmds(m_ncMachine);
		m_ncMachine->exitSetPriority();
	}
}

void QtWin2::on_actionLoadStylesheet_triggered()
{
	QFile file("./resources/myQss.qss");
	//QFile file("./resources/Adaptic.qss");

	if (file.open(QIODevice::ReadOnly))
	{
		qobject_cast<QApplication*>(QApplication::instance())->setStyleSheet(file.readAll());
		file.close();
	}
}

void QtWin2::on_actionPythonScript_triggered()
{
	BaseChildWindow* currentChild = dynamic_cast<BaseChildWindow*>(ui->stackedChildWidget->currentWidget());
	PythonQtRuntime::getInstance()->ShowConsole(currentChild);
}

void QtWin2::on_actionPowerOn_triggered()
{
	tr("to_clear_machine_axis");
	if (m_modbus->isConnected()) {
		this->setEnabled(false);

		// PowerOn
		m_ncMachine->PowerOn();

		bool b = FormUtils::MessageBoxYesNo(tr("SFJXZDHL"));
		
		if (b) {
			// 回零
			QString gcode;
			//gcode += "do_message_yn to_clear_machine_axis\n";
			DataForm* dataForm = DataForms::getInstance()->getDataForm("xitongshezhi");
			
			if (dataForm->getValue("HLFX_Z") == "-")
				//m_ncMachine->BacktoZero(DdfxEnum::Z_MINUS);
				gcode += "do_backtozeron Z\n";
			else
				//m_ncMachine->BacktoZero(DdfxEnum::Z_PLUS);
				gcode += "do_backtozero Z\n";
			if (dataForm->getValue("HLFX_Y") == "-")
				//m_ncMachine->BacktoZero(DdfxEnum::Y_MINUS);
				gcode += "do_backtozeron Y\n";
			else
				//m_ncMachine->BacktoZero(DdfxEnum::Y_PLUS);
				gcode += "do_backtozero Y\n";
			if (dataForm->getValue("HLFX_X") == "-")
				//m_ncMachine->BacktoZero(DdfxEnum::X_MINUS);
				gcode += "do_backtozeron X\n";
			else
				//m_ncMachine->BacktoZero(DdfxEnum::X_PLUS);
				gcode += "do_backtozero X\n";

			gcode += "do_message_yn to_clear_machine_axis\n";
			gcode += QString(R"(o101 if[#<message_ret> EQ 1]
do_zeroaxis 7
o101 endif)");

			m_ncMachine->RunGCode(gcode);
			//m_ncMachine->WaitUntilRunFinished();


			//// 统一清零机械坐标
			//std::function<void()> doFuncZeroAxis2 = [this]() {
			//	QMetaObject::invokeMethod(this, [this]() {
			//		bool b = FormUtils::MessageBoxYesNo(tr("SFQLJXZB？"));
			//		if (b) {
			//			m_ncMachine->enterSetPriority(1);
			//			m_ncMachine->GoApi();

			//			// 统一螺距补偿使能
			//			for (int i = 0; i < CURRENT_AXIS_LEN; ++i) {
			//				PropertyObjects::getInstance()->propertyObjectZLJBC[i]->setbcsx(true);
			//			}
			//			ZLJBCPropertyObject::ExecuteCmds2(m_ncMachine, PropertyObjects::getInstance()->propertyObjectZLJBC);

			//			for (int i = 0; i < CURRENT_AXIS_LEN; ++i) {
			//				m_ncMachine->SetPosOne(TMBS_MAP0_ID_XPOS + i, 0);
			//				//std::function<void(int, ModbusTask*, ModbusAdapter*)> function = [this, i](int ret, ModbusTask* task, ModbusAdapter* adapter) {
			//				//	m_ncMachine->GetController()->setAxisAbsolutePosition(GCodeTool::axis[i], 0, GCode::Units::METRIC);
			//				//	task->startAddr = 1;
			//				//	};
			//				//m_ncMachine->executeCmdWait(function);
			//				std::function<void()> doFuncZeroAxisInController = [this, i]() {
			//					m_ncMachine->GetController()->setAxisAbsolutePosition(GCodeTool::axis[i], 0, GCode::Units::METRIC);
			//					};
			//				m_ncMachine->executeCmdWait(m_ncMachine->convertWaitFunction(m_ncMachine->waitno(doFuncZeroAxisInController)), "doFuncZeroAxisInController");
			//			}
			//			m_ncMachine->GoJog();
			//			m_ncMachine->exitSetPriority();
			//		}
			//		}, Qt::QueuedConnection);
			//	};
			//m_ncMachine->executeCmdWait(m_ncMachine->convertWaitFunction(m_ncMachine->waitno(doFuncZeroAxis2)), "doFuncZeroAxis");
		}

		this->setEnabled(true);
	}
}

void QtWin2::on_actionPowerOff_triggered()
{
	m_ncMachine->PowerOff();
}

void QtWin2::on_actionSaveScreen_triggered()
{
	QScreen* screen = QGuiApplication::primaryScreen();
	if (const QWindow* window = QGuiApplication::focusWindow()) {
		screen = window->screen();
	}
	if (screen) {
		try {
			SystemUtilities::rotate(
				"logs/screenshot.png", "logs", 10);
		} CATCH_ERROR;

		try {
			SystemUtilities::rotate(
				"logs/core.dmp", "logs", 5);
		} CATCH_ERROR;

		QDateTime now = QDateTime::currentDateTime();
		//QString fileName = QString("logs/screenshot-%1.png").arg(now.toString("yyMMdd-hhmmss"));
		QString fileName = QString("logs/screenshot.png");
		QPixmap originalPixmap = screen->grabWindow(0);
		originalPixmap.save(fileName);
	}

	QString filePath = QDir::currentPath() + QDir::separator() + PLUGIN_PATH + "/python/send_logs.py";
	PythonQtRuntime::getInstance()->RunFile(filePath);
}

void QtWin2::on_actionSaveGCode_triggered()
{
	BaseChildWindow* currentChild = dynamic_cast<BaseChildWindow*>(ui->stackedChildWidget->currentWidget());
	if (currentChild) {
		QString s = currentChild->GetGCode();
		if (!s.isEmpty()) {
			QString m_path = "data/nc";
			QString filePath = NFileDialog::getSaveFileName(this, tr("BCWJ"), m_path, QString("(*.nc)"));
			if (!filePath.isEmpty()) {

				NFile file(filePath);
				if (file.open(QIODevice::WriteOnly)) {
					file.write(s.toUtf8());
					file.close();

					BaseMainWindow* mainWindow = BaseChildWindow::GetMainWindow();
					mainWindow->showChildWindow(QString("NC"));

					NcEditForm* form = dynamic_cast<NcEditForm*>(mainWindow->getChildWindow(QString("NC")));
					form->OpenFile(filePath.toUtf8().data());
				}
			}
		}
	}
}

//void QtWin2::on_btnErrorDetail_clicked()
//{
//#ifdef _DEBUG
//	if (!m_showFullScreen) {
//		char filename[] = LOGGER_FILE_NAME;
//		WCHAR buffer[MAX_PATH];
//		GetCurrentDirectory(MAX_PATH, buffer);
//		std::string fullpath = filename;
//		std::wstring wstrFullpath = std::wstring(buffer) + L"\\" + std::wstring(fullpath.begin(), fullpath.end());
//		LPCWSTR lpcwstrFullpath = wstrFullpath.c_str();
//		HINSTANCE result = ShellExecute(NULL, L"open", lpcwstrFullpath, NULL, NULL, SW_SHOWNORMAL);
//		//int x = reinterpret_cast<int>(result);
//		//if (x > 32) {
//		//	std::cout << "File opened successfully." << std::endl;
//		//}
//		//else {
//		//	DWORD errCode = GetLastError();
//		//	std::cout << "Failed to open the file." << std::endl;
//		//}
//	}
//#else
//	//ui->lblResultMessage->setToolTip(tr("QLXGLYCKRZWJ！QLXGLYCKRZWJ！"));
//	QHelpEvent* event = new QHelpEvent(QEvent::ToolTip,
//		ui->lblResultMessage->pos(),
//		ui->lblResultMessage->mapToGlobal(QPoint(0, 0)));
//	QApplication::postEvent(ui->lblResultMessage, event);
//#endif
//}

BaseChildWindow* QtWin2::getChildWindow(QString windowTitle)
{
	auto buttons = ui->groupBoxMenus->findChildren<QToolButton*>(QString(), Qt::FindChildOption::FindChildrenRecursively);
	for (QToolButton* button : buttons) {
		if (button->text() == windowTitle) {
			if (m_buttonIdxs.contains(button))
			{
			}
			else if (m_buttonForms.contains(button))
			{
				auto childWidget = m_buttonForms[button]();
				childWidget->SetMachine(m_ncMachine);
				m_buttonIdxs[button] = m_currentChildWindowCnt;
				m_currentChildWindowCnt++;

				ui->stackedChildWidget->addWidget(childWidget);
				//QWidget* childPage2 = new QWidget();
				//childPage2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
				//QVBoxLayout* childLayout = new QVBoxLayout(childPage2);
				//childLayout->addWidget(childWidget);
				//ui->stackedChildWidget->addWidget(childPage2);
			}
			if (m_buttonIdxs.contains(button))
			{
				BaseChildWindow* currentChild = dynamic_cast<BaseChildWindow*>(ui->stackedChildWidget->widget(m_buttonIdxs[button]));
				return currentChild;
			}
		}
	}
	return nullptr;
}

void QtWin2::onNavigatorButtonClicked()
{
	//G01Data& g01Data = m_ncMachine->GetG01Data();
	//if (g01Data.isRunning) {  // && g01Data.isPausing
	//	return;
	//}

	QAbstractButton* button = qobject_cast<QAbstractButton*>(sender());

	//if (m_buttonForms.contains(button)) {
	//	// check is it is exist in m_historyMenus
	//	QString menuText = button->text();
	//	m_historyMenus.removeOne(menuText);
	//	m_historyMenus.insert(0, menuText);
	//	AddChildWindow(menuText, m_layouts[0], (std::function<BaseChildWindow * ()>)NULL, true);
	//}

	button->setChecked(true);

	if (!m_buttonForms.contains(button))
	{
		button = m_buttonsText[button->text()];
	}
	if (!m_buttonForms.contains(button))
	{
		return;
	}

	if (m_buttonIdxs.contains(button))
	{
	}
	else
	{
		auto childWidget = m_buttonForms[button]();
		if (childWidget != nullptr) {
			childWidget->SetMachine(m_ncMachine);
			m_buttonIdxs[button] = m_currentChildWindowCnt;
			m_currentChildWindowCnt++;

			ui->stackedChildWidget->addWidget(childWidget);
			//QWidget* childPage2 = new QWidget();
			//childPage2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			//QVBoxLayout* childLayout = new QVBoxLayout(childPage2);
			//childLayout->addWidget(childWidget);
			//ui->stackedChildWidget->addWidget(childPage2);
		}
	}
	//qobject_cast<BaseChildWindow*>(ui->stackedChildWidget->currentWidget())->HideKeyboard();

	if (m_buttonIdxs.contains(button)) {
		ui->stackedChildWidget->setCurrentIndex(m_buttonIdxs[button]);
	}

	if (m_dialog4FangDianCheShi != NULL && m_dialog4FangDianCheShi->isVisible()) {
		m_dialog4FangDianCheShi->hide();
		ui->btnJgtj->setChecked(false);
	}
	if (m_dialog4Gaotiao != NULL && m_dialog4Gaotiao->isVisible()) {
		m_dialog4Gaotiao->hide();
		ui->btnJgtjGt->setChecked(false);
	}
}

void QtWin2::on_btnJgtj_clicked()
{
	G01Data& g01Data = m_ncMachine->GetG01Data();
	if (!g01Data.isRunning)
		return;

	if (m_formFangDianCheShi == NULL) {
		m_dialog4FangDianCheShi = new SoftKeyboardWidget(this); // SoftKeyboardWidget(this); //QDialog(this, Qt::FramelessWindowHint);
		m_dialog4FangDianCheShi->setObjectName("dialogFangDianCheShiRealtime");
		//qobject_cast<QDialog*>(m_dialog4FangDianCheShi)->setModal(false);

		m_formFangDianCheShi = new FangDianCanShuSingleForm(m_dialog4FangDianCheShi);
		m_formFangDianCheShi->setObjectName("fangDianCheShiRealtime");
		//m_formFangDianCheShi->setStyleSheet("background-color: red;");

		m_dialog4FangDianCheShi->setWindowTitle(tr("FDCS"));
		//m_dialog4FangDianCheShi->setWindowFlags(m_dialog4FangDianCheShi->windowFlags() & ~Qt::WindowContextHelpButtonHint);  // 禁用 "?" 按钮
		m_dialog4FangDianCheShi->setWindowFlags((Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint | Qt::CustomizeWindowHint) & ~Qt::WindowContextHelpButtonHint);

		// 创建一个包含 "OK" 按钮的按钮盒
		auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, m_formFangDianCheShi);
		QPushButton* okButton = buttonBox->button(QDialogButtonBox::StandardButton::Ok);
		QPushButton* cancelButton = buttonBox->button(QDialogButtonBox::StandardButton::Cancel);
		//okButton->setFixedSize(100, 50);
		//cancelButton->setFixedSize(100, 50);
		//okButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
		//cancelButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
		okButton->setText(tr("FS"));
		cancelButton->setText(tr("GB"));

		//layout->addWidget(buttonBox);
		m_formFangDianCheShi->GetLayout(0)->addWidget(buttonBox);
		auto layout = new QVBoxLayout(m_dialog4FangDianCheShi);
		layout->addWidget(m_formFangDianCheShi);

		// 当 "OK" 按钮被点击时，关闭对话框
		//connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
		//connect(buttonBox, &QDialogButtonBox::rejected, m_dialog4FangDianCheShi, &QDialog::reject);
		//disconnect(okButton, &QPushButton::clicked, m_dialog4FangDianCheShi, &QDialog::accept);

		connect(okButton, &QPushButton::clicked, [this]() {
			m_formFangDianCheShi->RunGCode(m_ncMachine);
			});
		connect(cancelButton, &QPushButton::clicked, [this]() {
			m_dialog4FangDianCheShi->hide();
			});
		QNumericPad::setButtonDownModeWhenInTouch(m_dialog4FangDianCheShi);
	}

	if (m_dialog4FangDianCheShi->isVisible()) {
		m_dialog4FangDianCheShi->hide();
	}
	else {
		QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
		//QRect screenGeometry = QApplication::desktop()->availableGeometry();
		m_dialog4FangDianCheShi->setFixedSize(screenGeometry.width() - 250, 240);

		//int x = screenGeometry.right() - dialog->width();
		//int y = screenGeometry.bottom() - m_dialog4FangDianCheShi->height() - 15;
		int x = 5;
		int y = screenGeometry.top() + 340;
		m_dialog4FangDianCheShi->move(x, y);

		m_formFangDianCheShi->LoadFromParametersC();
		m_dialog4FangDianCheShi->show();
		//if (dialog->exec() == QDialog::Accepted) {
		//	//form->RunGCode();
		//}
		if (m_dialog4Gaotiao != NULL && m_dialog4Gaotiao->isVisible()) {
			m_dialog4Gaotiao->hide();
			ui->btnJgtjGt->setChecked(false);
		}
	}
}

void QtWin2::on_btnJgtjGt_clicked()
{
	G01Data& g01Data = m_ncMachine->GetG01Data();
	if (!g01Data.isRunning)
		return;

	if (m_formGaotiao4Jiagong == NULL) {
		m_dialog4Gaotiao = new SoftKeyboardWidget(this);  //new QDialog(this, Qt::FramelessWindowHint);
		m_dialog4Gaotiao->setObjectName("dialogGaotiao4Jiagong");

		m_formGaotiao4Jiagong = new Gaotiao4Jiagong(m_ncMachine, m_dialog4Gaotiao);

		m_dialog4Gaotiao->setWindowTitle(tr("GT"));
		//m_dialog4Gaotiao->setWindowFlags(m_dialog4Gaotiao->windowFlags() & ~Qt::WindowContextHelpButtonHint);  // 禁用 "?" 按钮
		m_dialog4Gaotiao->setWindowFlags((Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint | Qt::CustomizeWindowHint) & ~Qt::WindowContextHelpButtonHint);

		auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, m_dialog4Gaotiao);
		QPushButton* okButton = buttonBox->button(QDialogButtonBox::StandardButton::Ok);
		QPushButton* cancelButton = buttonBox->button(QDialogButtonBox::StandardButton::Cancel);
		//okButton->setFixedSize(100, 50);
		//cancelButton->setFixedSize(100, 50);
		okButton->setText(tr("FS"));
		cancelButton->setText(tr("GB"));

		auto layout = new QVBoxLayout(m_dialog4Gaotiao);
		layout->addWidget(buttonBox);
		layout->addWidget(m_formGaotiao4Jiagong);

		connect(okButton, &QPushButton::clicked, this, [this]() {
			m_formGaotiao4Jiagong->RunGCode();
		});
		connect(cancelButton, &QPushButton::clicked, this, [this]() {
			m_dialog4Gaotiao->hide();
			});
		QNumericPad::setButtonDownModeWhenInTouch(m_dialog4Gaotiao);
	}

	if (m_dialog4Gaotiao->isVisible()) {
		m_dialog4Gaotiao->hide();
	}
	else {
		QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
		m_dialog4Gaotiao->setFixedSize(350, 250);

		int x = screenGeometry.right() - m_dialog4Gaotiao->width() - 250;
		//int y = screenGeometry.bottom() - m_dialog4Gaotiao->height() - 20;
		int y = screenGeometry.top() + 340;
		m_dialog4Gaotiao->move(x, y);

		//m_dialog4Gaotiao->setModal(false);

		//if (m_dialog4Gaotiao->exec() == QDialog::Accepted) {
		//	//form->RunGCode();
		//}
		m_formGaotiao4Jiagong->LoadData();
		m_dialog4Gaotiao->show();

		if (m_dialog4FangDianCheShi != NULL && m_dialog4FangDianCheShi->isVisible()) {
			m_dialog4FangDianCheShi->hide();
			ui->btnJgtj->setChecked(false);
		}
	}
}

void QtWin2::switchUserMode()
{
	m_isUserMode = !m_isUserMode;
	showWindowMode(m_isUserMode);
}

void QtWin2::hideTaskbar() {
	HWND hTaskbar = FindWindow(L"Shell_TrayWnd", NULL);
	if (hTaskbar) {
		ShowWindow(hTaskbar, SW_HIDE);
	}
}

void QtWin2::showTaskbar() {
	HWND hTaskbar = FindWindow(L"Shell_TrayWnd", NULL);
	if (hTaskbar) {
		ShowWindow(hTaskbar, SW_SHOW);
	}
}


void QtWin2::UpdateStateByTimer()
{
	m_pollTimerCounter++;
	if (m_pollTimerCounter >= 60) {
		m_pollTimerCounter = 0;

		bool serialPortIsFound = false;
		QString targetPortName = "COM7";
		QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
		for (const QSerialPortInfo& portInfo : availablePorts) {
			if (portInfo.portName() == targetPortName) {
				serialPortIsFound = true;
			}
		}
		if (!serialPortIsFound) {
			m_serialPortNeedReconnect = true;
		}
		if (m_modbus->getErrorsCount() > 100)
		{
			m_serialPortNeedReconnect = true;
		}
		if (m_serialPortNeedReconnect && serialPortIsFound)
		{
			modbusConnect(false);
			modbusConnect(true);
			if (!m_modbus->isConnected()) {
				ui->lblResultMessage->setText(tr("CKWLJ"));
				ui->actionRun->setEnabled(false);
				ui->actionCancel->setEnabled(false);
			}
			else {
				m_serialPortNeedReconnect = false;
			}
		}
	}


	ui->labelInfoLianjie->setEnabled(false);

	bool isRunning = m_ncMachine->IsGCodeRunning() || m_ncMachine->GetNowOperating() > 0;
	ui->actionRun->setEnabled(!isRunning);
	ui->actionCancel->setEnabled(isRunning);

	if (ui->lblResultMessage->text().isEmpty()) {
		QStringList errorMsgs;
		QString errorMsg = LineLogger::instance().getErrors();
		if (!errorMsg.isEmpty()) {
			errorMsgs.append(errorMsg);
		}
		if (errorMsgs.count() > 0)
		{
			ui->lblResultMessage->setText(errorMsgs.join(';').left(50));
			if (!errorMsg.isEmpty()) {
				ui->lblResultMessage->setToolTip(LineLogger::instance().getErrors(5));
			}
		}
	}
}

void QtWin2::UpdateState()
{
	
	//ui->labelInfoLianjie->setEnabled(true);
//#ifdef _DEBUG
	QTime time1 = QTime::currentTime();
//#endif

	try
	{
		// auto pos = m_ncMachine->GetXYZUWithOffset();
		// auto x = pos.x() / 1000.0;
		// pos.data[3] / 1000.0
		auto x = m_ncMachine->GetController()->getAxisPosition('X');
		auto y = m_ncMachine->GetController()->getAxisPosition('Y');
		auto z = m_ncMachine->GetController()->getAxisPosition('Z');
		auto u = m_ncMachine->GetController()->getAxisPosition('U');
		auto state = m_ncMachine->GetState();

		ui->xLabel->setText(GCodeTool::FormatPos(x));
		ui->yLabel->setText(GCodeTool::FormatPos(y));
		ui->zLabel->setText(GCodeTool::FormatPos(z));
		ui->uLabel->setText(GCodeTool::FormatPos(u));

		//m_formNCMachineState.xLabel->setText(NCMachine::GetStateDesc(state[2]));
		//m_formNCMachineState.yLabel->setText(NCMachine::GetRLSTDesc(state[0], state[1]));
		//m_formNCMachineState.msgLabel->setText(m_ncMachine->GetDebugMsg());

		QDateTime now = QDateTime::currentDateTime();
		ui->lblNowDate->setText(now.toString("yy-MM-dd"));
		ui->lblNowTime->setText(now.toString("hh:mm:ss"));

		auto csNames = GCodeTool::csNames;
		int csIndex = m_ncMachine->GetController()->get(CURRENT_COORD_SYSTEM) - 1;
		ui->btnCoor->setText(QString("G%1%2").arg(csIndex / 6).arg(csNames[csIndex % 6]));

		bool isRunning = m_ncMachine->IsGCodeRunning() || m_ncMachine->GetNowOperating() > 0;

		//ui->rb3->setChecked(m_modbus->isConnected());
		//ui->rb2->setChecked(isRunning);

		//{
		//	std::string jm = NCMachineParametersC::GetCurrentByName("JM");
		//	size_t len = jm.length();
		//	if (len > 1) {
		//		ui->btnG24->setChecked(jm[len - 2] == '0');
		//	}
		//}

		G01Data& g01Data = m_ncMachine->GetG01Data();

		QStringList errorMsgs;
		QString errorMsg = LineLogger::instance().getErrors();
		//ui->rb1->setChecked(!s.isEmpty());
		//ui->rb1->setToolTip(s);

		bool setNewLights = false;
		if (!errorMsg.isEmpty()) {
			errorMsgs.append(errorMsg);
			// 系统报警时，显示红色  (Reg24 Bit10 = 1，Bit11 = 0）
			if (!PropertyObjects::getInstance()->propertyObjectFuzhuIO->out1()) {
				PropertyObjects::getInstance()->propertyObjectFuzhuIO->setout1(true);
				PropertyObjects::getInstance()->propertyObjectFuzhuIO->setout2(false);
				setNewLights = true;
			}
		}
		else
		{
			if (!isRunning) {
				// 正常待机的时候不显示（Reg24 Bit10 = 0，Bit11 = 0)
				if (PropertyObjects::getInstance()->propertyObjectFuzhuIO->out1() || PropertyObjects::getInstance()->propertyObjectFuzhuIO->out2()) {
					PropertyObjects::getInstance()->propertyObjectFuzhuIO->setout1(false);
					PropertyObjects::getInstance()->propertyObjectFuzhuIO->setout2(false);
					setNewLights = true;
				}

				QString s = NCMachine::GetRLSTDesc(state[0], state[1]);
				if (s.isEmpty()) {
					ui->lblResultMessage->setText(tr("DDYX"));
				}
				else {
					//ui->lblResultMessage->setText(s);
					errorMsgs.append(s);
				}
			}
			else {
				// 加工状态下，显示绿色（Reg24 Bit10 = 0，Bit11 = 1）
				if (!PropertyObjects::getInstance()->propertyObjectFuzhuIO->out2()) {
					PropertyObjects::getInstance()->propertyObjectFuzhuIO->setout1(false);
					PropertyObjects::getInstance()->propertyObjectFuzhuIO->setout2(true);
					setNewLights = true;
				}
				if (m_ncMachine->GetNowOperating() == 1) {
					ui->lblResultMessage->setText(tr("DDZ"));
				}
				else if (m_ncMachine->GetNowOperating() == 2) {
					ui->lblResultMessage->setText(tr("HLZ"));
				}
				else {
					if (g01Data.isRunning) {
						if (!g01Data.isPausing) {
							ui->lblResultMessage->setText(tr("FDZ"));
						}
						else {
							ui->lblResultMessage->setText(tr("FDZT"));
						}
					}
					else {
						if (m_ncMachine->isTaskThreadPausing()) {
							ui->lblResultMessage->setText(tr("ZTZ"));
						}
						else {
							ui->lblResultMessage->setText(tr("YXZ"));
						}
					}
				}
			}
		}

		if (setNewLights) {
			m_ncMachine->enterSetPriority(0);
			PropertyObjects::getInstance()->propertyObjectFuzhuIO->ExecuteCmds(m_ncMachine);
			m_ncMachine->exitSetPriority();
		}

		if (m_ncMachine->GetIsJogDuanlu()) {
			errorMsgs.append(tr("DLBJ"));
			ui->labelInfoDuanLu->show();
		}
		else {
			ui->labelInfoDuanLu->hide();
		}
		ui->labelInfoZhaoMing->hide();

		if (PropertyObjects::getInstance()->propertyObjectFuzhuIO->yb1()) {
			ui->btnT0->setChecked(true);
		}
		else {
			ui->btnT0->setChecked(false);
		}
		if (PropertyObjects::getInstance()->propertyObjectFuzhuIO->yb2()) {
			ui->btnT1->setChecked(true);
		}
		else {
			ui->btnT1->setChecked(false);
		}

		if (PropertyObjects::getInstance()->propertyObjectFuzhuIO->yb1() || PropertyObjects::getInstance()->propertyObjectFuzhuIO->yb2()) {
			ui->labelInfoYouBang->setEnabled(true);
		}
		else {
			ui->labelInfoYouBang->setEnabled(false);
		}

		if (m_ncMachine->GetScamanulFlag(15)) {
			ui->btnST->setChecked(true);
		}
		else {
			ui->btnST->setChecked(false);
		}
		ui->btnYouWen->setChecked(PropertyObjects::getInstance()->propertyObjectYouwenJianche->youWen());
		ui->btnYouWei->setChecked(PropertyObjects::getInstance()->propertyObjectYouwenJianche->youWei());
		ui->btnHuoYan->setChecked(PropertyObjects::getInstance()->propertyObjectYouwenJianche->huoYan());

		bool inputFlagError = false;
		if (m_ncMachine->GetInputFlag(16 + 7)) {
			errorMsgs.append(tr("CFDJBJ"));
			inputFlagError = true;
		}


		if (m_ncMachine->GetInputFlag(8)) {
			//errorMsgs.append(tr("YWBJ"));
			//inputFlagError = true;
			//ui->labelInfoYouWen->show();
		}
		else {
			ui->labelInfoYouWen->hide();
		}

		if (m_ncMachine->GetInputFlag(9)) {
			if (g01Data.isRunning) {
				errorMsgs.append(tr("DDYWZ"));
			}
			else {
				//errorMsgs.append(tr("YWBJ"));
			}
			//inputFlagError = true;
			//ui->labelInfoYouWei->show();
		}
		else {
			ui->labelInfoYouWei->hide();
		}

		if (m_ncMachine->GetInputFlag(10)) {
			//errorMsgs.append(tr("HYBJ"));
			//inputFlagError = true;
		}
		else {
		}


		if (m_ncMachine->GetInputFlag(0)) {
			errorMsgs.append(tr("XXW"));
			inputFlagError = true;
		}
		else if (m_ncMachine->GetInputFlag(1)) {
			errorMsgs.append(tr("XXW"));
			inputFlagError = true;
		}
		if (m_ncMachine->GetInputFlag(2)) {
			errorMsgs.append(tr("YXW"));
			inputFlagError = true;
		}
		else if (m_ncMachine->GetInputFlag(3)) {
			errorMsgs.append(tr("YXW"));
			inputFlagError = true;
		}
		if (m_ncMachine->GetInputFlag(4)) {
			errorMsgs.append(tr("ZXW"));
			inputFlagError = true;
		}
		else if (m_ncMachine->GetInputFlag(5)) {
			errorMsgs.append(tr("ZXW"));
			inputFlagError = true;
		}

		if (inputFlagError) {
			if (!m_lastInputFlagError) {
				m_lastInputFlagError = true;
#ifndef  _DEBUG999
				m_ncMachine->enterSetPriority(0);
				m_ncMachine->Beep(1, 500);
				m_ncMachine->exitSetPriority();
#endif
			}
		}
		else {
			m_lastInputFlagError = false;
		}

		if (errorMsgs.count() > 0)
		{
			QString errorMsg = errorMsgs.join(';').left(50);
			ui->lblResultMessage->setText(errorMsg);
			if (!errorMsg.isEmpty()) {
				ui->lblResultMessage->setToolTip(LineLogger::instance().getErrors(5));
			}

			if (errorMsg.contains(tr("YW"))) {
				ui->labelInfoYouWei->show();
			}
			if (errorMsg.contains(tr("YW"))) {
				ui->labelInfoYouWen->show();
			}
			//if (errorMsg.contains(tr("HY"))) {
			//	ui->labelInfoYouWen->show();
			//}
		}

		// check when start and stop
		//if (!ui->toolBar->isHidden()) 
		{
			ui->actionRun->setEnabled(!isRunning);
			ui->actionCancel->setEnabled(isRunning);

			// 模拟加工
			//m_topinfobarButtons[0]->setEnabled(!isRunning);
			//m_topinfobarButtons[0]->setChecked(isRunning && g01Data.isRunning && g01Data.isSimulate);

			ui->btnCoor->setEnabled(!isRunning);
		}

		//ui->groupBoxMenus->setEnabled(!isRunning);

		QDateTime date2 = m_ncMachine->GetDwellEndTime();
		if (date2 > now) {
			ui->labelG04RemnantTime->setText(SecondsToStr(now.secsTo(date2)));
		}
		else {
			ui->labelG04RemnantTime->setText("00:00:00");
		}


		if (g01Data.isRunning && !g01Data.isPausing) {
			m_displayLoran->setIsRunning(true);
			// STEP maybe changed in G01
			g01Data.step = PropertyObjects::getInstance()->propertyObjectYd->ydbj();
			m_displayLoran->setP(g01Data.step, 0, 0);

			cb::Vector4I mpos = m_ncMachine->GetXYZU();

			//case GCode::MachineEnum::plane_t::XY:
			if (g01Data.to_p.x() == g01Data.from_p.x() && g01Data.to_p.y() == g01Data.from_p.y()) {
				// 加工方向Z轴向下时，按照通用计算，（X，Y）平面是向下的，但显示还是尽量和实际一致，取反
				//Vector3I point_a(mpos.x(), mpos.y(), mpos.z());
				//Vector3I p = MathTool::TransformPoint(point_a, g01Data.from_p, g01Data.to_p);

				m_displayLoran->setCurrentPos(mpos.x() - g01Data.from_p.x(), mpos.y() - g01Data.from_p.y());
				if (g01Data.to_p.z() > g01Data.from_p.z())
					g01Data.now_z = std::max(g01Data.now_z, mpos.z());
				else
					g01Data.now_z = std::min(g01Data.now_z, mpos.z());
			}
			//case GCode::MachineEnum::plane_t::XZ:
			else if (g01Data.to_p.x() == g01Data.from_p.x() && g01Data.to_p.z() == g01Data.from_p.z()) {
				m_displayLoran->setCurrentPos(mpos.x() - g01Data.from_p.x(), mpos.z() - g01Data.from_p.z());
				if (g01Data.to_p.y() > g01Data.from_p.y())
					g01Data.now_z = std::max(g01Data.now_z, mpos.y());
				else
					g01Data.now_z = std::min(g01Data.now_z, mpos.y());
			}
			//case GCode::MachineEnum::plane_t::YZ:
			else if (g01Data.to_p.y() == g01Data.from_p.y() && g01Data.to_p.z() == g01Data.from_p.z()) {
				m_displayLoran->setCurrentPos(mpos.y() - g01Data.from_p.y(), mpos.z() - g01Data.from_p.z());
				if (g01Data.to_p.x() > g01Data.from_p.x())
					g01Data.now_z = std::max(g01Data.now_z, mpos.x());
				else
					g01Data.now_z = std::min(g01Data.now_z, mpos.x());
			}
			else
			{
				//// 多轴时，通用算法
				//Vector3I point_a(mpos.x(), mpos.y(), mpos.z());
				//Vector3I p = MathTool::TransformPoint(point_a, g01Data.from_p, g01Data.to_p);
				//m_displayLoran->setCurrentPos(-p.x(), -p.y());
				//if (p.z() > g01Data.start_z)
				//	g01Data.now_z = std::max(g01Data.now_z, p.z());
				//else
				//	g01Data.now_z = std::min(g01Data.now_z, p.z());
			}

			ui->labelLength->setText(QString::number(std::min(9999.9999, abs(g01Data.to_z - g01Data.start_z) / 1000.0), 'f', 3));
			//ui->labelRemnantLength->setText(QString::number(std::min(9999.9999, abs(g01Data.to_z - g01Data.now_z) / 1000.0), 'f', 3));
			ui->labelRemnantLength->setText(QString::number(g01Data.jgcyl / 1000.0, 'f', 3)); // 绝对值

			qint64 remnantSeconds;
			if (PropertyObjects::getInstance()->propertyObjectFdwc->fdwcms() != 0)
			{
				remnantSeconds = g01Data.dssj / 1000 - g01Data.startTime.secsTo(now) - g01Data.nowWorkSeconds;
				if (remnantSeconds < 0)
					remnantSeconds = 0;
			}
			else {
				remnantSeconds = 0;
			}
			remnantSeconds = g01Data.dssysj / 1000;

			ui->labelProcessTime->setText(SecondsToStr(g01Data.nowWorkSeconds + g01Data.startTime.secsTo(now)));
			ui->labelRemnantTime->setText(SecondsToStr(remnantSeconds));

			ui->labelProcessSpeed->setText(QString::number(std::min(9999.9999, abs(g01Data.now_z - g01Data.start_z) / 1000.0 / g01Data.startTime.secsTo(now) * 60), 'f', 3));

			ui->labelTotalTime->setText(SecondsToStr(g01Data.allWorkSeconds + g01Data.startTime.secsTo(now)));
			ui->labelCNo->setText("C" + QString::number(g01Data.cNo).rightJustified(3, '0'));

			m_displayZ->setIsRunning(true);
			m_displayZ->setEndData(now.addSecs(remnantSeconds), g01Data.to_z);
			m_displayZ->addData(now, g01Data.now_z);
		}
		else {
			m_displayLoran->setIsRunning(false);
			m_displayZ->setIsRunning(false);

			//ui->labelLength->setText("0.000");
			//ui->labelRemnantLength->setText("0.000");
			//ui->labelProcessTime->setText("00:00:00");
			//ui->labelRemnantTime->setText("00:00:00");
			//ui->labelProcessSpeed->setText("0.000");

			ui->labelProcessTime->setText(SecondsToStr(g01Data.nowWorkSeconds));
			ui->labelTotalTime->setText(SecondsToStr(g01Data.allWorkSeconds));
			//ui->labelCNo->setText("C000");
		}


		if (!m_displayLoran->isHidden()) {
			m_displayLoran->draw();
		}
		if (!m_displayZ->isHidden()) {
			m_displayZ->draw();
		}

		ui->labelInfoFangdian->setEnabled(g01Data.isRunning);
		ui->labelInfoDiandong->setEnabled(PropertyObjects::getInstance()->propertyObjectShouKongHe->ddsdkz() == SKHKZPropertyObject::Enum3::DD);
		QMetaEnum metaEnum = QMetaEnum::fromType<SKHKZPropertyObject::Enum2>();
		QString enumString = metaEnum.key(static_cast<int>(PropertyObjects::getInstance()->propertyObjectShouKongHe->sdxskz()));
		ui->labelInfoJogSpeed->setText(enumString);
		ui->labelInfoLianjie->setEnabled(true);

		//#ifdef _DEBUG
		QTime time2 = QTime::currentTime();
		m_msecUI += time1.msecsTo(time2);
		m_cntUI++;

		if (!ui->statusbar->isHidden()) {
			QString msg = QString("Modbus Time: %1 / %2, %3%. Packet: %4 / %5. UI Time: %6 / %2, %7. State: %8,%9.  Debug Msg: %10")
				.arg(m_modbus->getCommMSec()).arg(m_modbus->getAllMSec()).arg(m_modbus->getCommMSec() * 100 / (m_modbus->getAllMSec() + 1))
				.arg(m_modbus->getErrorsCount()).arg(m_modbus->getPacketsCount())
				.arg(m_msecUI).arg(m_cntUI)
				.arg(NCMachine::GetStateDesc(state[2])).arg(NCMachine::GetRLSTDesc(state[0], state[1]))
				.arg(m_ncMachine->GetDebugMsg());
			ui->statusbar->showMessage(msg);
		}

		if (g01Data.newData) {
			if (m_formFangDianCheShi != NULL && m_formFangDianCheShi->isVisible()) {
				m_formFangDianCheShi->LoadFromParametersC();
				g01Data.newData = false;
			}
			if (m_formGaotiao4Jiagong != NULL && m_formGaotiao4Jiagong->isVisible()) {
				m_formGaotiao4Jiagong->LoadData();
				g01Data.newData = false;
			}
		}
		ui->btnFileName->setText(SystemSettings::instance().LastRunNCFileName);
	}
	catch (const std::exception& e) {
		LOG_WARNING("EDM: exception: " << e.what());
	}
	catch (...) {
	}


	BaseChildWindow* currentChild = dynamic_cast<BaseChildWindow*>(ui->stackedChildWidget->currentWidget());
	if (currentChild != NULL) {
		currentChild->UpdateState();
	}
//#endif
}

QString QtWin2::SecondsToStr(qint64 totalSeconds)
{
	int hours = totalSeconds / 3600;
	if (hours > 99)
		hours = 99;
	int minutes = (totalSeconds % 3600) / 60;
	int seconds = totalSeconds % 60;
	QString timeStr = QString("%1:%2:%3")
		.arg(hours, 2, 10, QChar('0'))
		.arg(minutes, 2, 10, QChar('0'))
		.arg(seconds, 2, 10, QChar('0'));
	return timeStr;
}

void QtWin2::OpenRegWindow()
{
	if (m_dlgRegWindow == NULL) {
		m_dlgRegWindow = new RegWindow(this, m_ncMachine, m_modbus, m_modbusCommSettings);
	}
	m_dlgRegWindow->show();
}

void QtWin2::AddChildWindow(QString title, QBoxLayout* layout, BaseChildWindow* childWidget)
{
	childWidget->SetMachine(m_ncMachine);
	childWidget->hide();
	AddChildWindow(title, layout, [childWidget]() {
		return childWidget;
	});
}

void QtWin2::AddChildWindow(QString title, QBoxLayout* layout, std::function<BaseChildWindow* ()> func, bool insertAtBegin)
{
	QToolButton* btn = new QToolButton(mainMenu);
	btn->setObjectName(QString("%1_menuButton_%2").arg(this->objectName()).arg(title));
	m_buttonGroup4Menu->addButton(btn);
	btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	btn->setCheckable(true);
	btn->setText(title);
	if (!insertAtBegin) {
		layout->addWidget(btn);
	}
	else {
		layout->insertWidget(0, btn);
	}
	if (func != NULL) {
		m_buttonForms[btn] = func;
		m_buttonsText[title] = btn;
	}

	connect(btn, &QAbstractButton::clicked, this, &QtWin2::onNavigatorButtonClicked);
}

void QtWin2::LoadPluginsAsChildWindow()
{
	// Load Python init scripts (global scripts without UI)
	QMap<QString, QString> pyFiles = PluginUtils::loadPythonScripts("main");
	for (const auto& pyFileName : pyFiles.keys()) {
		if (pyFileName == "init") {
			QString value = pyFiles.value(pyFileName);
			PluginUtils::RunFile(value, this);
		}
	}

	// Track Python plugin names for priority (Python overrides C++ and system)
	QSet<QString> pythonPluginNames;

	// Load Python menu plugins from different directories
	// Directory mapping: directory name -> page index
	// Page 0: Manual, Page 1: Auto, Page 2: Edit, Page 3: Run, Page 4: Settings
	QList<QPair<QString, int>> pluginDirs = {
		{"menu/manual",   0},
		{"menu/auto",     1},
		{"menu/edit",     2},
		{"menu/run",      3},
		{"menu/settings", 4}
	};

	for (const auto& dirInfo : pluginDirs) {
		QString dir = dirInfo.first;
		int pageIdx = dirInfo.second;
		QList<PythonPluginInfo> pyPlugins = PluginUtils::loadPythonMenuPlugins(dir, pageIdx);
		for (const auto& pluginInfo : pyPlugins) {
			QString menuText = pluginInfo.name;
			QString filePath = pluginInfo.filePath;
			int pageIndex = pluginInfo.pageIndex;

			// Mark as Python plugin
			pythonPluginNames.insert(menuText);

			// Ensure page index is valid
			if (pageIndex < 0 || pageIndex >= m_layouts.size()) {
				pageIndex = 1;
			}

			// Create the factory function
			auto factoryFunc = [this, filePath, menuText]() -> BaseChildWindow* {
				QWidget* pyWidget = PluginUtils::createPythonWidget(filePath, this);
				if (pyWidget) {
					PythonPluginChildWindow* child = new PythonPluginChildWindow(this);
					child->SetPythonWidget(pyWidget);
					child->SetPluginFile(filePath);
					child->SetPluginName(menuText);
					return child;
				}
				return nullptr;
			};

			// Check if menu with same name already exists (Python > system)
			if (m_buttonsText.contains(menuText)) {
				// Replace existing button's function
				QAbstractButton* btn = m_buttonsText[menuText];
				m_buttonForms[btn] = factoryFunc;
			}
			else {
				// Add new menu item
				AddChildWindow(menuText, m_layouts[pageIndex], factoryFunc);
			}
		}
	}

	// Load C++ plugins (DLL) - skip if Python version exists
	QList<IDoPlugin*> plugins = PluginUtils::loadPlugin<IDoPlugin>("main");
	for (IDoPlugin* it : plugins) {
		QString menuText = it->getName();

		// Skip if Python version already loaded (Python has priority)
		if (pythonPluginNames.contains(menuText)) {
			continue;
		}

		// Check if menu with same name already exists (C++ > system, but Python > C++)
		if (m_buttonsText.contains(menuText)) {
			// Replace existing button's function
			QAbstractButton* btn = m_buttonsText[menuText];
			m_buttonForms[btn] = [it, this]() -> BaseChildWindow* {
				QWidget* itWidget = it->getWidget();
				if (itWidget) {
					ContainerChildWindow* child = new ContainerChildWindow(this);
					child->SetPlugin(it);
					QHBoxLayout* layout = new QHBoxLayout(child);
					layout->addWidget(itWidget);
					return child;
				}
				return nullptr;
			};
		}
		else {
			AddChildWindow(menuText, m_layouts[1], [it, this]() -> BaseChildWindow* {
				QWidget* itWidget = it->getWidget();
				if (itWidget) {
					ContainerChildWindow* child = new ContainerChildWindow(this);
					child->SetPlugin(it);
					QHBoxLayout* layout = new QHBoxLayout(child);
					layout->addWidget(itWidget);
					return child;
				}
				return nullptr;
			});
		}
	}
}

void QtWin2::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_F12) {
		if (!m_isUserMode) {
			switchUserMode();
			event->accept();
		}
		else {
			//QString pw = FormUtils::ShowPasswordInput();
			//if (!pw.isEmpty() && pw == SystemSettings::instance().GetValue("System/Password")) 
			{
				switchUserMode();
				event->accept();
			}
		}
	}
	else if (event->key() == Qt::Key_F11) {
		on_actionSaveScreen_triggered();
		event->accept();
	}
	BaseMainWindow::keyPressEvent(event);
}

QtWin3::QtWin3(QWidget* parent)
	: ModbusMain(parent), BaseMainWindow(parent)
{
	mdiForm = new MdiForm(this);
	this->resize(1400, 500);
	this->setCentralWidget(mdiForm);
	mdiForm->SetMachine(m_ncMachine);

	statusBar = new QStatusBar(this);
	this->setStatusBar(statusBar);

	if (m_modbus->isConnected()) {
		addNormalTasks();

		connect(m_modbus, SIGNAL(refreshView()), this, SLOT(UpdateState()));
	}
}

QtWin3::~QtWin3()
{
	disconnect(m_modbus, SIGNAL(refreshView()), this, SLOT(UpdateState()));

	delete mdiForm;
}

void QtWin3::UpdateState()
{
	QString msg = QString("Pos: %1. Debug Msg: %2")
		.arg(QString::fromStdString(m_ncMachine->GetXYZU().toString()))
		.arg(m_ncMachine->GetDebugMsg());
	statusBar->showMessage(msg);
}
