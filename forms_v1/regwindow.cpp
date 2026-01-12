#include "regwindow.h"
#include "ui_regwindow.h"
#include <QFileDialog>
#include <QButtonGroup>
#include <QLineEdit>
#include <QGroupBox>
#include <QMessageBox>

#include <cbang/json/JSON.h>
#include <cbang/SmartPointer.h>
#include <cbang/io/InputSource.h>
#include <cbang/Catch.h>

#include "ModbusFile.h"
#include "NCMachinePanel.h"
#include "../modbus/registersmodel.h"

#include "../modbus/NCCommand.h"
#include "BaseMainWindow.h"
#include "forms/mainwindow2.h"

RegWindow::RegWindow(QWidget* parent, NCMachine* machine, ModbusAdapter* adapter, ModbusCommSettings* settings)
	: QDialog{ parent }, m_ncMachine(machine), m_modbus(adapter), m_modbusCommSettings(settings), ui(new Ui::RegWindow)
{
	ui->setupUi(this);
	this->setWindowFlags(this->windowFlags() | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);

	auto regModel = m_modbus->GetRegistersModel();
	if (regModel != NULL) {
		ui->tblRegisters->setItemDelegate(regModel->itemDelegate());
		ui->tblRegisters->setModel(regModel->model);
		ui->tblRegisters->horizontalHeader()->show();
		ui->tblRegisters->verticalHeader()->show();
		ui->tblRegisters->setProperty("anchorHorizontal", "right");
		ui->tblRegisters->setProperty("anchorVertical", "bottom");

		regModel->setIs16Bit(true);
		regModel->setBase(EUtils::Hex);
		regModel->setIsSigned(false);
	}
	else {
		//QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->tblRegisters->model());
		//if (model == NULL) {
		QStandardItemModel* model = new QStandardItemModel(this);
		ui->tblRegisters->setModel(model);
		ui->tblRegisters->horizontalHeader()->show();
		ui->tblRegisters->verticalHeader()->show();
		//}

		for (int i = 0; i < 10; ++i)
		{
			QStandardItem* item = new QStandardItem("...");
			model->setItem(0, i, item);
		}
	}
	m_modbus->resetCounters();
	//m_modbus->setSlave(1);
	//m_modbus->setFunctionCode(EUtils::ModbusFunctionCode(2));
	//m_modbus->setStartAddr(0);
	//m_modbus->setNumOfRegs(18);
	m_modbus->addItems(0, 13);

	//QLOG_INFO() << "Add Items. Function Code = " << QString::number(EUtils::ModbusFunctionCode(ui->cmbFunctionCode->currentIndex()), 16);

	//m_modbus->modbusTransaction();

	// Connect the clicked() signal of the button to the slot function
	/*connect(ui->btnStart, SIGNAL(clicked()), this, SLOT(on_btnStart_clicked()));
	connect(ui->btnStop, SIGNAL(clicked()), this, SLOT(on_btnStop_clicked()));*/

	connect(m_modbus, SIGNAL(refreshView()), this, SLOT(UpdateState()));
	//connect(m_ncMachine, SIGNAL(keyPressed(unsigned long long)), this, SLOT(UpdateState2(unsigned long long)));

	init();

	on_btnStart_clicked();

	//static_cast<BaseMainWindow*>(this->topLevelWidget())->addNormalTasks();
}

RegWindow::~RegWindow()
{
	disconnect(m_modbus, SIGNAL(refreshView()), this, SLOT(UpdateState()));

	delete ui;

	//QLOG_INFO()<<  "Stop Program" ;
}

void RegWindow::addProperyEditor(QtPropertyEditor::QtPropertyTreeEditor& propertyEditor, BasePropertyObject* propertyObject)
{
	// Dynamic properties.
	 /*object->setProperty("myDynamicBool", false);
	 object->setProperty("myDynamicInt", 3);
	 object->setProperty("myDynamicDouble", 3.0);
	 object->setProperty("myDynamicString", "3 amigos");
	 object->setProperty("myDynamicDateTime", QDateTime::currentDateTime());*/

	 //QtPropertyEditor::QtPropertyTreeEditor* editor = new QtPropertyEditor::QtPropertyTreeEditor();
	propertyEditor.treeModel.propertyNames = QtPropertyEditor::getPropertyNames(propertyObject);
	// editor->treeModel.addProperty("myInt");
	for (int i = 0; i < propertyEditor.treeModel.propertyNames.size(); ++i) {
		propertyEditor.treeModel.propertyHeaders[propertyEditor.treeModel.propertyNames[i]] = propertyObject->headers[propertyEditor.treeModel.propertyNames[i]];
	}
	for (const auto& element : propertyObject->enumHeaders) {
		propertyEditor.treeModel.enumHeaders[element.first] = element.second;
		propertyEditor.treeModel.enumHeadersReverse[element.second] = element.first;
	}
	propertyEditor.treeModel.setObject(propertyObject);

	propertyEditor.resizeColumnsToContents();
}

void RegWindow::init()
{
	auto propertyObjects = PropertyObjects::getInstance();
	// Tab2
	{
		{
			addProperyEditor(m_propertyEditorTab2, propertyObjects->propertyObjectJog);
			ui->verticalLayoutTab2_1->insertWidget(0, &m_propertyEditorTab2);
		}

		// 机械坐标设定
		{
			QVBoxLayout* vertialLayout2 = new QVBoxLayout();
			QGroupBox* groupBox2 = new QGroupBox(QStringLiteral("机械坐标设定"));
			for (int i = 0; i < POS_AXIS_LEN; ++i)
			{
				QLineEdit* lineEdit = new QLineEdit();
				//lineEdit->setMaximumWidth(100);
				lineEdit->setText("0");
				vertialLayout2->addWidget(lineEdit);
				m_tab2LineEdits[i] = lineEdit;
			}
			QPushButton* btnSendCurrentPos = new QPushButton(QStringLiteral("发送"));
			connect(btnSendCurrentPos, &QPushButton::clicked, [=]() {
				std::vector<uint16_t> v;
				try {
					for (int i = 0; i < POS_AXIS_LEN; ++i)
					{
						QString text = m_tab2LineEdits[i]->text();
						int value = text.toInt();
						v.push_back(value & 0xFFFF);
						v.push_back(value >> 16);
					}
				}
				catch (...) {
					return;
				}
				m_ncMachine->enterSetPriority(0);
				m_ncMachine->executeCmdsByAddress(TMBS_MAP0_ID_XPOS, v);
				m_ncMachine->exitSetPriority();
				});
			vertialLayout2->addWidget(btnSendCurrentPos);
			groupBox2->setLayout(vertialLayout2);

			ui->verticalLayoutTab2_2->addWidget(groupBox2);
		}
	}

	// Tab3
	addProperyEditor(m_propertyEditorTab3, propertyObjects->propertyObjectLoc);
	ui->verticalLayoutTab3->insertWidget(0, &m_propertyEditorTab3);

	// Tab4
	addProperyEditor(m_propertyEditorTab4, propertyObjects->propertyObjectCmvTouch);
	ui->verticalLayoutTab4->insertWidget(0, &m_propertyEditorTab4);

	// Tab5
	addProperyEditor(m_propertyEditorTab5, propertyObjects->propertyObjectCmvToLMT);
	ui->verticalLayoutTab5->insertWidget(0, &m_propertyEditorTab5);


	// Tab6
	addProperyEditor(m_propertyEditorTab6, propertyObjects->propertyObjectCmvFRMLMT);
	ui->verticalLayoutTab6->insertWidget(0, &m_propertyEditorTab6);

	// Tab7
	addProperyEditor(m_propertyEditorTab7, propertyObjects->propertyObjectCmvToZSig);
	ui->verticalLayoutTab7->insertWidget(0, &m_propertyEditorTab7);

	// Tab8
	addProperyEditor(m_propertyEditorTab8, propertyObjects->propertyObjectSPK);
	ui->verticalLayoutTab8->insertWidget(0, &m_propertyEditorTab8);

	// Tab9
	QButtonGroup* buttonGroup = new QButtonGroup(ui->tab_8);
	buttonGroup->setExclusive(false);
	ui->gridLayoutTab9->setSizeConstraint(QLayout::SetMinimumSize);
	for (int i = 0; i < 32; i++) {
		if (m_machineInputFlags[i] == NULL)
			continue;
		QRadioButton* radioButton1 = new QRadioButton(m_machineInputFlags[i], ui->tab_8);
		radioButton1->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
		radioButton1->setEnabled(false);
		radioButton1->setCheckable(true);
		radioButton1->setToolTip(m_machineInputNames[i]);

		ui->gridLayoutTab9->addWidget(radioButton1, i / 8, i % 8);
		buttonGroup->addButton(radioButton1);

		radioButton1->setChecked(m_ncMachine->GetInputFlag(i));

		m_machineInputRadioButtons[i] = radioButton1;
	}

	// Tab10
	{
		QHBoxLayout* horizontalLayout = new QHBoxLayout();
		ui->tab_10->setLayout(horizontalLayout);

		// 手控盒按钮
		{
			QGridLayout* gridLayout = new QGridLayout();
			// Create 24 buttons and add them to the grid layout
			for (int row = 0; row < 6; row++) {
				for (int col = 0; col < 4; col++) {
					// Calculate the index of the QString in the array
					int index = row * 4 + col;

					// Create the button with the text from the QString in the array
					m_machinePadPushButtons[index] = new QPushButton(m_machinePads[index]);
					m_machinePadPushButtons[index]->setMaximumWidth(50);

					// Add the button to the grid layout at the current row and column
					gridLayout->addWidget(m_machinePadPushButtons[index], row, col);
				}
			}
			horizontalLayout->addLayout(gridLayout);
		}

		// 手控盒操作数据
		{
			QVBoxLayout* vertialLayout = new QVBoxLayout();
			QGroupBox* groupBox = new QGroupBox(QStringLiteral("手控盒操作数据"));
			groupBox->setLayout(vertialLayout);
			for (int i = 0; i < 2; ++i)
			{
				QLineEdit* lineEdit = new QLineEdit();
				lineEdit->setMaximumWidth(100);
				lineEdit->setText("...");
				lineEdit->setReadOnly(true);
				vertialLayout->addWidget(lineEdit);
				m_tab10LineEdits[i] = lineEdit;
			}
			//horizontalLayout->addLayout(vertialLayout);
			horizontalLayout->addWidget(groupBox);
		}

		// 当前在用坐标系值
		{
			QVBoxLayout* vertialLayout2 = new QVBoxLayout();
			QGroupBox* groupBox2 = new QGroupBox(QStringLiteral("当前在用坐标系值"));
			for (int i = 0; i < POS_AXIS_LEN; ++i)
			{
				QLineEdit* lineEdit = new QLineEdit();
				//lineEdit->setMaximumWidth(100);
				lineEdit->setText("");
				vertialLayout2->addWidget(lineEdit);
				m_tab10LineEdits[i + 2] = lineEdit;
			}
			QPushButton* btnSendCurrentPos = new QPushButton(QStringLiteral("发送"));
			connect(btnSendCurrentPos, &QPushButton::clicked, [=]() {
				std::vector<uint16_t> v;
				try {
					for (int i = 0; i < POS_PAD_AXIS_LEN; ++i)
					{
						QString text = m_tab10LineEdits[i + 2]->text();
						int value = text.toInt();
						v.push_back(value & 0xFFFF);
						v.push_back(value >> 16);
					}
				}
				catch (...) {
					return;
				}
				m_ncMachine->executeCmdsByAddress(TMBS_MAP0_ID_XOFST, v);
				});
			vertialLayout2->addWidget(btnSendCurrentPos);
			groupBox2->setLayout(vertialLayout2);
			//horizontalLayout->addLayout(vertialLayout2);
			horizontalLayout->addWidget(groupBox2);
		}
	}


	// Tab11
	{
		addProperyEditor(m_propertyEditorTab11, propertyObjects->propertyObjectFanddianIO);
		ui->verticalLayoutTab11->insertWidget(0, &m_propertyEditorTab11);
	}

	// Tab12
	{
		addProperyEditor(m_propertyEditorTab12, propertyObjects->propertyObjectFuzhuIO);
		ui->verticalLayoutTab12->insertWidget(0, &m_propertyEditorTab12);
	}

	// Tab13
	{
		addProperyEditor(m_propertyEditorTab13, propertyObjects->propertyObjectShouKongHe);
		ui->verticalLayoutTab13->insertWidget(0, &m_propertyEditorTab13);
	}

	// Tab14
	{
		QString asixNames[4] = { QStringLiteral("X轴"), QStringLiteral("Y轴"),QStringLiteral("Z轴"),QStringLiteral("U轴") };

		for (int i = 0; i < LJBC_AXIS_LEN; ++i)
		{
			QVBoxLayout* vertialLayout = new QVBoxLayout();
			QGroupBox* groupBox = new QGroupBox(asixNames[i] + QStringLiteral("轴螺距补偿"));
			groupBox->setLayout(vertialLayout);

			propertyObjectZLJBC[i] = propertyObjects->propertyObjectZLJBC[i];
			addProperyEditor(m_propertyEditorTab14[i], propertyObjectZLJBC[i]);
			vertialLayout->insertWidget(0, &m_propertyEditorTab14[i]);

			ui->horizontalLayout14->addWidget(groupBox);
		}
	}

	// Tab15
	{
		ModbusFile* dialog = new ModbusFile(this, m_modbus);
		ui->verticalLayoutTab15->addWidget(dialog, 1);
	}

	// Tab16
	{
		QHBoxLayout* horizontalLayout = new QHBoxLayout();
		ui->tab_16->setLayout(horizontalLayout);

		// 手控盒控制
		addProperyEditor(horizontalLayout, propertyObjects->propertyObjectDianDong);

		addProperyEditor(horizontalLayout, propertyObjects->propertyObjectDianJiShiNeng);
		addProperyEditor(horizontalLayout, propertyObjects->propertyObjectFengMingQi);
	}

	// Tab17
	{
		NCMachinePanel* dialog = new NCMachinePanel(this, m_ncMachine, m_modbus, m_modbusCommSettings);
		ui->verticalLayoutTab17->addWidget(dialog, 1);
	}

	// Tabs
	{
		QHBoxLayout* horizontalLayout = new QHBoxLayout();
		addProperyEditor(horizontalLayout, propertyObjects->propertyObjectADC);
		//horizontalLayout->addWidget(new QPushButton("Send", ui->tabWidget));

		QWidget* newWidget = new QWidget();
		newWidget->setLayout(horizontalLayout);
		ui->tabWidget->addTab(newWidget, QStringLiteral("ADC"));
	}
	{
		QHBoxLayout* horizontalLayout = new QHBoxLayout();
		addProperyEditor(horizontalLayout, propertyObjects->propertyObjectFdhl);
		QWidget* newWidget = new QWidget();
		newWidget->setLayout(horizontalLayout);
		ui->tabWidget->addTab(newWidget, QStringLiteral("放电回路"));
	}
	{
		QHBoxLayout* horizontalLayout = new QHBoxLayout();
		addProperyEditor(horizontalLayout, propertyObjects->propertyObjectFdjg);
		QWidget* newWidget = new QWidget();
		newWidget->setLayout(horizontalLayout);
		ui->tabWidget->addTab(newWidget, QStringLiteral("放电加工"));
	}
	{
		QHBoxLayout* horizontalLayout = new QHBoxLayout();
		addProperyEditor(horizontalLayout, propertyObjects->propertyObjectFdwc);
		QWidget* newWidget = new QWidget();
		newWidget->setLayout(horizontalLayout);
		ui->tabWidget->addTab(newWidget, QStringLiteral("放电完成"));
	}
	{
		QHBoxLayout* horizontalLayout = new QHBoxLayout();
		addProperyEditor(horizontalLayout, propertyObjects->propertyObjectYd);
		QWidget* newWidget = new QWidget();
		newWidget->setLayout(horizontalLayout);
		ui->tabWidget->addTab(newWidget, QStringLiteral("摇动"));
	}
}

void RegWindow::addProperyEditor(QHBoxLayout* horizontalLayout, BasePropertyObject* propertyObject)
{
	QVBoxLayout* vertialLayout2 = new QVBoxLayout();
	//QGroupBox* groupBox2 = new QGroupBox(QStringLiteral(""));

	QtPropertyEditor::QtPropertyTreeEditor* propertyEditorTab = new QtPropertyEditor::QtPropertyTreeEditor();
	addProperyEditor(*propertyEditorTab, propertyObject);
	vertialLayout2->insertWidget(0, propertyEditorTab);
	m_propertyEditors.push_back(propertyEditorTab);

	QPushButton* btnSendCurrentPos = new QPushButton(QStringLiteral("发送"));
	connect(btnSendCurrentPos, &QPushButton::clicked, [=]() {
		m_ncMachine->enterSetPriority(0);
		propertyObject->ExecuteCmds(m_ncMachine);
		m_ncMachine->exitSetPriority();
		});
	vertialLayout2->addWidget(btnSendCurrentPos);
	//groupBox2->setLayout(vertialLayout2);
	//horizontalLayout->addWidget(groupBox2);
	horizontalLayout->addLayout(vertialLayout2);
}

void RegWindow::UpdateState()
{
	for (int i = 0; i < 32; i++) {
		if (m_machineInputFlags[i] == NULL)
			continue;
		bool b = m_ncMachine->GetInputFlag(i);
		m_machineInputRadioButtons[i]->setChecked(b);
	}

	if (m_readDataBufferLen > 0) {
		for (int i = 0; i < m_readDataBufferLen; ++i)
		{
			QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->tblRegisters->model());
			if (model != NULL) {
				QStandardItem* item;
				if (false && m_readDataBuffer[i] == NULL) {
					item = new QStandardItem("...");
				}
				else {
					item = new QStandardItem(QString::number(m_readDataBuffer[i]));
				}
				int row = i / 10;
				int col = i % 10;
				model->setItem(row, col, item);
			}
		}
		// only show once
		m_readDataBufferLen = 0;
	}
	// 
	//if (model == NULL) {
	//	auto regModel = new RegistersModel(this);
	//	ui->tblRegisters->setItemDelegate(regModel->itemDelegate());
	//	ui->tblRegisters->setModel(regModel->model);
	//	ui->tblRegisters->horizontalHeader()->show();
	//	ui->tblRegisters->verticalHeader()->show();
	//	ui->tblRegisters->setProperty("anchorHorizontal", "right");
	//	ui->tblRegisters->setProperty("anchorVertical", "bottom");

	//	regModel->setIs16Bit(true);
	//	regModel->setBase(EUtils::Hex);
	//	regModel->setIsSigned(false);
	//}
	//model->clear();
}

void RegWindow::UpdateState2(unsigned long long key)
{
	int selectedButtonIndex = -1;
	switch (key)
	{
	case KNLK_XSUB:
		selectedButtonIndex = 1 * 4 + 0;
		break;
	case KNLK_XADD:
		selectedButtonIndex = 1 * 4 + 1;
		break;
	case KNLK_YSUB:
		selectedButtonIndex = 2 * 4 + 0;
		break;
	case KNLK_YADD:
		selectedButtonIndex = 2 * 4 + 1;
		break;
	case KNLK_ZSUB:
		selectedButtonIndex = 3 * 4 + 0;
		break;
	case KNLK_ZADD:
		selectedButtonIndex = 3 * 4 + 1;
		break;
	case KNLK_CENT:
		selectedButtonIndex = 4 * 4 + 2;
		break;
	case KNLK_ST:
		selectedButtonIndex = 1 * 4 + 3;
		break;
	case KNLK_STEP:
		selectedButtonIndex = 3 * 4 + 2;
		break;
	case KNLK_SPEEDD:
		selectedButtonIndex = 2 * 4 + 2;
		break;
	case KNLK_SPEEDU:
		selectedButtonIndex = 1 * 4 + 2;
		break;
	case KNLK_PUMP1:
		selectedButtonIndex = 2 * 4 + 3;
		break;
	case KNLK_PUMP2:
		selectedButtonIndex = 3 * 4 + 3;
		break;
	case KNLK_START:
		selectedButtonIndex = 4 * 4 + 3;
		break;
	case KNLK_STOP:
		selectedButtonIndex = 5 * 4 + 3;
		break;
	case KNLK_ENT:
		selectedButtonIndex = 5 * 4 + 2;
		break;
	default:

		break;
	}


	for (int i = 0; i < sizeof(m_machinePadPushButtons) / sizeof(QPushButton*); ++i) {
		if (i == selectedButtonIndex)
			m_machinePadPushButtons[selectedButtonIndex]->setStyleSheet("background-color: red;");
		else
			m_machinePadPushButtons[i]->setStyleSheet("");
	}


	m_tab10LineEdits[0]->setText(QString::number(m_ncMachine->GetTargetPosOne()));
	uint16_t targetType = m_ncMachine->GetTargetType();

	QString s1, s2;
	switch (targetType & 0x000F)
	{
	case 0:
		s1 = QStringLiteral("X轴");
		break;
	case 1:
		s1 = QStringLiteral("Y轴");
		break;
	case 2:
		s1 = QStringLiteral("Z轴");
		break;
	}
	switch (targetType & 0x00F0)
	{
	case 0x00:
		s2 = QStringLiteral("无操作");
		break;
	case 0x10:
		s2 = QStringLiteral("设定当前");
		m_tab10LineEdits[1]->setText(s1 + s2);
		break;
	case 0x20:
		s2 = QStringLiteral("定位移动");
		m_tab10LineEdits[1]->setText(s1 + s2);
		break;
	}

}

void RegWindow::on_btnStart_clicked()
{
	ModbusMainWindow::modbusConnect(true, m_modbus, m_modbusCommSettings, m_ncMachine);

	ui->btnStart->setEnabled(!m_modbus->isConnected());
	ui->btnStop->setEnabled(m_modbus->isConnected());
	ui->btnReadOnce->setEnabled(m_modbus->isConnected());
	//ui->btnWriteReg->setEnabled(m_modbus->isConnected());
}

void RegWindow::on_btnStop_clicked()
{
	ModbusMainWindow::modbusConnect(false, m_modbus, m_modbusCommSettings, m_ncMachine);

	ui->btnStart->setEnabled(!m_modbus->isConnected());
	ui->btnStop->setEnabled(m_modbus->isConnected());
	ui->btnReadOnce->setEnabled(m_modbus->isConnected());
	//ui->btnWriteReg->setEnabled(m_modbus->isConnected());
}

void RegWindow::on_btnReadOnce_clicked()
{
	//m_ncMachine->executeCmds();
	//m_modbus->modbusTransaction();

	int regAddr = ui->lineEditReadRegAddr->text().toInt();
	int regLen = ui->lineEditReadRegNum->text().toInt();
	regLen = regLen > 60 ? 60 : regLen;
	std::function<void(int, ModbusTask*, ModbusAdapter*)> function = [this, regLen](int ret, ModbusTask*, ModbusAdapter* adapter) {
		m_readDataBufferLen = 0;
		if (ret == -1) {
			for (int i = 0; i < regLen; ++i) {
				m_readDataBuffer[i] = 0;
			}
			return;
		}
		else {
			uint16_t* readData = adapter->GetReadedData16();
			m_readDataBufferLen = regLen;
			for (int i = 0; i < regLen; ++i) {
				m_readDataBuffer[i] = readData[i];
			}
		}
	};
	ModbusTask* task = m_modbus->getTaskRead(regAddr, regLen);
	task->setPostFunction(function, "Read Reg");
	m_modbus->addTask(task, 0);
}

void RegWindow::on_btnWriteReg_clicked()
{
	//std::string filename = QFileDialog::getOpenFileName
	//(this, tr("Open File"), "jsons",
	//	(tr("Supported Files") +
	//		QString("(*.json);;") +
	//		tr("All Files") + QString(" (*.*)"))).toUtf8().data();

	//if (filename.empty())
	//	return;

	QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->tblRegisters->model());
	if (model == NULL)
		return;
	int regAddr = ui->lineEditReadRegAddr->text().toInt();
	int regLen = ui->lineEditReadRegNum->text().toInt();
	std::vector<uint16_t> vs;
	for (int i = 0; i < regLen; ++i)
	{
		int row = i / 10;
		int col = i % 10;
		QModelIndex index = model->index(row, col, QModelIndex());
		QVariant value = model->data(index, Qt::DisplayRole);
		if (value.canConvert<QString>())
			vs.push_back(value.toString().toInt());
		else
			vs.push_back(0);
	}
	std::string s = NCCommand::GetWriteString(regAddr, regLen, vs);
	m_ncMachine->enterSetPriority(0);
	//m_ncMachine->executeCmdsInFile(filename);
	m_ncMachine->executeCmds(s);
	m_ncMachine->exitSetPriority();
}

void RegWindow::on_ckbVerbosity_stateChanged(int state)
{
	if (m_modbus->isConnected()) {
		if (state == Qt::Checked) {
			//m_modbus->startPollTimer();
			cb::Logger::instance().setVerbosity(10);
		}
		else {
			//m_modbus->stopPollTimer();
			cb::Logger::instance().setVerbosity(1);
		}
	}
}

void RegWindow::on_btnStartJog_clicked()
{
	m_ncMachine->enterSetPriority(0);
	PropertyObjects::getInstance()->propertyObjectJog->ExecuteCmds(m_ncMachine);
	//m_ncMachine->executeCmdsInFile("jsons/enable_jog.json");
	m_ncMachine->exitSetPriority();
}

void RegWindow::on_btnStopJog_clicked()
{
	m_ncMachine->enterSetPriority(0);
	m_ncMachine->executeCmds(NCCommand::DisableAll);
	//m_ncMachine->executeCmdsInFile("jsons/disable_jog.json");
	m_ncMachine->exitSetPriority();
}

void RegWindow::on_btnStartLoc_clicked()
{
	m_ncMachine->enterSetPriority(0);
	PropertyObjects::getInstance()->propertyObjectLoc->ExecuteCmds(m_ncMachine);
	m_ncMachine->exitSetPriority();
}

void RegWindow::on_btnStopLoc_clicked()
{
	on_btnStopJog_clicked();
}

void RegWindow::on_btnStartCmvTouch_clicked()
{
	m_ncMachine->enterSetPriority(0);
	PropertyObjects::getInstance()->propertyObjectCmvTouch->ExecuteCmds(m_ncMachine);
	m_ncMachine->exitSetPriority();
}

void RegWindow::on_btnStopCmvTouch_clicked()
{
	on_btnStopJog_clicked();
}

void RegWindow::on_btnStartCmvToLMT_clicked()
{
	m_ncMachine->enterSetPriority(0);
	PropertyObjects::getInstance()->propertyObjectCmvToLMT->ExecuteCmds(m_ncMachine);
	m_ncMachine->exitSetPriority();
}

void RegWindow::on_btnStopCmvToLMT_clicked()
{
	on_btnStopJog_clicked();
}

void RegWindow::on_btnStartCmvFRMLMT_clicked()
{
	m_ncMachine->enterSetPriority(0);
	PropertyObjects::getInstance()->propertyObjectCmvFRMLMT->ExecuteCmds(m_ncMachine);
	m_ncMachine->exitSetPriority();
}

void RegWindow::on_btnStopCmvFRMLMT_clicked()
{
	on_btnStopJog_clicked();
}

void RegWindow::on_btnStartTOZSIG_clicked()
{
	m_ncMachine->enterSetPriority(0);
	PropertyObjects::getInstance()->propertyObjectCmvToZSig->ExecuteCmds(m_ncMachine);
	m_ncMachine->exitSetPriority();
}

void RegWindow::on_btnStopTOZSIG_clicked()
{
	on_btnStopJog_clicked();
}

void RegWindow::on_btnStartSPK_clicked()
{
	m_ncMachine->enterSetPriority(0);
	PropertyObjects::getInstance()->propertyObjectSPK->ExecuteCmds(m_ncMachine);
	m_ncMachine->exitSetPriority();
}
void RegWindow::on_btnStopSPK_clicked()
{
	on_btnStopJog_clicked();
}

void RegWindow::on_btnSendFangdianIO_clicked()
{
	m_ncMachine->enterSetPriority(0);
	PropertyObjects::getInstance()->propertyObjectFanddianIO->ExecuteCmds(m_ncMachine);
	m_ncMachine->exitSetPriority();
}
void RegWindow::on_btnSendFuzhuIO_clicked()
{
	m_ncMachine->enterSetPriority(0);
	PropertyObjects::getInstance()->propertyObjectFuzhuIO->ExecuteCmds(m_ncMachine);
	m_ncMachine->exitSetPriority();
}

void RegWindow::on_btnSendKshkz_clicked()
{
	m_ncMachine->enterSetPriority(0);
	PropertyObjects::getInstance()->propertyObjectShouKongHe->ExecuteCmds(m_ncMachine);
	m_ncMachine->exitSetPriority();
}
void RegWindow::on_btnSendZLJBC_clicked()
{
	m_ncMachine->enterSetPriority(0);
	ZLJBCPropertyObject::ExecuteCmds1(m_ncMachine, PropertyObjects::getInstance()->propertyObjectZLJBC);
	ZLJBCPropertyObject::ExecuteCmds2(m_ncMachine, PropertyObjects::getInstance()->propertyObjectZLJBC);
	m_ncMachine->exitSetPriority();
}
void RegWindow::on_btnSendKshkz2_clicked()
{
	assert(TMBS_MAP0_ID_HANDVER_LEN == 1);
	assert(TMBS_MAP0_ID_DISPVER_LEN == 1);

	std::function<void(int, ModbusTask*, ModbusAdapter*)> function = [](int ret, ModbusTask*, ModbusAdapter* adapter) {
		if (ret == -1)
			return;
		uint16_t* readData = adapter->GetReadedData16();
		PropertyObjects::getInstance()->propertyObjectVersion->SetValues1(std::vector<uint16_t> {  readData[0], readData[1]});
		};
	ModbusTask* task = m_modbus->getTaskRead(TMBS_MAP0_ID_HANDVER, TMBS_MAP0_ID_HANDVER_LEN + TMBS_MAP0_ID_DISPVER_LEN);
	task->setPostFunction(function, "Read HandVer and DispVer");
	m_modbus->addTask(task, 0);
}


bool RegWindow::MessageBoxYesNo(QString title)
{
	QMessageBox msgBox;
	msgBox.setText(title);
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::Yes);

	int ret = msgBox.exec();

	return ret == QMessageBox::Yes;
}



#pragma region "Message Parse"
QString RegWindow::parseMsg(QString val)
{
	if (val.indexOf("Sys") > -1)
		return parseSysMsg(val);
	else if (val.indexOf("Tx") > -1)
		return parseTxMsg(val);
	else if (val.indexOf("Rx") > -1)
		return parseRxMsg(val);
	else
		return "Type : Unknown Message";
}

QString RegWindow::parseTxMsg(QString msg)
{
	QString ret;
	ret.append("Type : Tx Message");
	QStringList row = msg.split(QRegExp("\\s+"));
	ret.append("Timestamp : " + row[2]);
	if (msg.indexOf("RTU") > -1) {//RTU message
		QStringList pdu;
		if (row.length() < 5) {//check message length
			ret.append("Error! Cannot parse Message");
			return ret;
		}
		for (int i = 4; i < row.length() - 1; i++)
			pdu.append(row[i]);
		parseTxPDU(pdu, "Slave Addr : ");
		ret.append("CRC : " + pdu[pdu.length() - 2] + pdu[pdu.length() - 1]);
	}
	else if (msg.indexOf("TCP") > -1) {//TCP message
		if (row.length() < 11) {//check message length
			ret.append("Error! Cannot parse Message");
			return ret;
		}
		ret.append("Transaction ID : " + row[4] + row[5]);
		ret.append("Protocol ID : " + row[6] + row[7]);
		ret.append("Length : " + row[8] + row[9]);
		QStringList pdu;
		for (int i = 10; i < row.length() - 1; i++)
			pdu.append(row[i]);
		parseTxPDU(pdu, "Unit ID : ");
	}
	else
		ret.append("Error! Cannot parse Message");

	return ret;
}

QString RegWindow::parseTxPDU(QStringList pdu, QString slave)
{
	QString ret;
	if (pdu.length() < 6) {//check message length
		ret.append(slave + pdu[0]);
		ret.append("Function Code : " + pdu[1]);
		//ret.append("Error! Cannot parse Message");
		return ret;
	}
	ret.append(slave + pdu[0]);
	ret.append("Function Code : " + pdu[1]);
	ret.append("Starting Address : " + pdu[2] + pdu[3]);
	bool ok;
	int fcode = pdu[1].toInt(&ok, 16);
	if (fcode == 1 || fcode == 2 || fcode == 3 || fcode == 4) {//read
		ret.append("Quantity of Registers : " + pdu[4] + pdu[5]);
	}
	else if (fcode == 5 || fcode == 6) {//write
		ret.append("Output Value : " + pdu[4] + pdu[5]);
	}
	else if (fcode == 15 || fcode == 16) {//write multiple
		ret.append("Quantity of Registers : " + pdu[4] + pdu[5]);
		if (pdu.length() < 8) {//check message length
			ret.append("Error! Cannot parse Message");
			return ret;
		}
		ret.append("Byte Count : " + pdu[6]);
		int byteCount = pdu[6].toInt(&ok, 16);
		QString outputValues = "";
		for (int i = 7; i < 7 + byteCount; i++)
			outputValues += pdu[i] + " ";
		ret.append("Output Values : " + outputValues);
	}
	return ret;
}

QString RegWindow::parseRxMsg(QString msg)
{
	QString ret;
	ret.append("Type : Rx Message");
	QStringList row = msg.split(QRegExp("\\s+"));
	ret.append("Timestamp : " + row[2]);
	if (msg.indexOf("RTU") > -1) {//RTU message
		QStringList pdu;
		if (row.length() < 5) {//check message length
			ret.append("Error! Cannot parse Message");
			return ret;
		}
		for (int i = 4; i < row.length() - 1; i++)
			pdu.append(row[i]);
		parseRxPDU(pdu, "Slave Addr : ");
		ret.append("CRC : " + pdu[pdu.length() - 2] + pdu[pdu.length() - 1]);
	}
	else if (msg.indexOf("TCP") > -1) {//TCP message
		if (row.length() < 11) {//check message length
			ret.append("Error! Cannot parse Message");
			return ret;
		}
		ret.append("Transaction ID : " + row[4] + row[5]);
		ret.append("Protocol ID : " + row[6] + row[7]);
		ret.append("Length : " + row[8] + row[9]);
		QStringList pdu;
		for (int i = 10; i < row.length() - 1; i++)
			pdu.append(row[i]);
		parseRxPDU(pdu, "Unit ID : ");
	}
	else
		ret.append("Error! Cannot parse Message");
	return ret;
}

QString RegWindow::parseRxPDU(QStringList pdu, QString slave)
{
	QString ret;
	bool ok;
	int fcode = pdu[1].toInt(&ok, 16);
	if (fcode == 1 || fcode == 2 || fcode == 3 || fcode == 4) {//read
		if (pdu.length() < 4) {//check message length
			ret.append("Error! Cannot parse Message");
			return ret;
		}
		ret.append(slave + pdu[0]);
		ret.append("Function Code : " + pdu[1]);
		ret.append("Byte Count : " + pdu[2]);
		int byteCount = pdu[2].toInt(&ok, 16);
		QString inputValues = "";
		for (int i = 3; i < 3 + byteCount; i++)
			inputValues += pdu[i] + " ";
		ret.append("Register Values : " + inputValues);
	}
	else if (fcode == 5 || fcode == 6) {//write
		if (pdu.length() < 6) {//check message length
			ret.append("Error! Cannot parse Message");
			return ret;
		}
		ret.append(slave + pdu[0]);
		ret.append("Function Code : " + pdu[1]);
		ret.append("Starting Address : " + pdu[2] + pdu[3]);
		ret.append("Output Value : " + pdu[4] + pdu[5]);
	}
	else if (fcode == 15 || fcode == 16) {//write multiple
		if (pdu.length() < 6) {//check message length
			ret.append("Error! Cannot parse Message");
			return ret;
		}
		ret.append(slave + pdu[0]);
		ret.append("Function Code : " + pdu[1]);
		ret.append("Starting Address : " + pdu[2] + pdu[3]);
		ret.append("Quantity of Registers : " + pdu[4] + pdu[5]);
	}
	else if (fcode > 0x80) {//exception
		if (pdu.length() < 3) {//check message length
			ret.append("Error! Cannot parse Message");
			return ret;
		}
		ret.append(slave + pdu[0]);
		ret.append("Function Code [80 + Rx Function Code] : " + pdu[1]);
		ret.append("Exception Code : " + pdu[2]);
	}
	return ret;
}

QString RegWindow::parseSysMsg(QString msg)
{
	QString ret;
	ret.append("Type : System Message");
	QStringList row = msg.split(QRegExp("\\s+"));
	ret.append("Timestamp : " + row[2]);
	ret.append("Message" + msg.mid(msg.indexOf(" : ")));
	return ret;
}
#pragma endregion

