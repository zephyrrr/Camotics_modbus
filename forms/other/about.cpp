#include "about.h"
#include <QKeyEvent>
#include "modbus/NCMachineDefines.h"
#include "modbus/NCCommand.h"
#include "modbus/modbusadapter.h"
#include "modbus/NCMachine.h"

AboutForm::AboutForm(QWidget *parent)
	: BaseChildWindow(parent)
{
	ui.setupUi(this);

	ui.gridLayout->setColumnStretch(0, 1);
	ui.gridLayout->setColumnStretch(1, 1);
	ui.gridLayout->setColumnStretch(2, 1);
	ui.gridLayout->setColumnStretch(3, 1);
	ui.gridLayout->setColumnStretch(4, 0);

	for (int i = 0; i < 5; ++i) {
		ui.gridLayout->setRowStretch(i, 1);
		ui.gridLayout->setRowMinimumHeight(i, 35);
	}

	ui.lblSoftVersion->setText(QApplication::applicationVersion());
	ui.lblHardwareVersion1->setText(PropertyObjects::getInstance()->propertyObjectVersion->versionSkh());
	ui.lblHardwareVersion2->setText(PropertyObjects::getInstance()->propertyObjectVersion->versionXskz());
	ui.lblHardwareVersion3->setText(PropertyObjects::getInstance()->propertyObjectVersion->versionXwj());
	ui.lblHardwareVersion4->setText(PropertyObjects::getInstance()->propertyObjectVersion->versionXwjnh());
	ui.lblSerialNumber->setText(QString::number(PropertyObjects::getInstance()->propertyObjectReg89->serial()));
	ui.lblReg87Date->setText(QString::number(PropertyObjects::getInstance()->propertyObjectReg87->date()));

	// 连续点击版本号 3 次进入管理员模式
	ui.lblSoftVersion->installEventFilter(this);

	connect(ui.btnDecrypt, &QPushButton::clicked, [this]() {
		// 读取输入
		uint date = ui.txtReg87Date->text().toUInt();
		uint password = ui.txtDecryptPassword->text().toUInt();

		// 设置并写入 Reg87
		PropertyObjects::getInstance()->propertyObjectReg87->setdate(date);
		PropertyObjects::getInstance()->propertyObjectReg87->setpassword(password);
		PropertyObjects::getInstance()->propertyObjectReg87->ExecuteCmds(m_ncMachine, 0);

		// 重新读取 Reg87 更新显示
		ModbusAdapter* adapter = m_ncMachine->getModbus();
		std::function<void(int, ModbusTask*, ModbusAdapter*)> callback = [this](int ret, ModbusTask*, ModbusAdapter* ad) {
			if (ret == -1) return;
			uint16_t* readData = ad->GetReadedData16();
			PropertyObjects::getInstance()->propertyObjectReg87->SetValues(
				std::vector<uint16_t> { readData[0], readData[1], readData[2], readData[3] });
			QMetaObject::invokeMethod(this, [this]() {
				ui.lblReg87Date->setText(QString::number(PropertyObjects::getInstance()->propertyObjectReg87->date()));
			}, Qt::QueuedConnection);
		};
		ModbusTask* task = adapter->getTaskRead(TMBS_MAP0_ID_REG87, TMBS_MAP0_ID_REG87_LEN);
		task->setPostFunction(callback, "Read Reg87 After Decrypt");
		adapter->addTask(task, 0);
	});

	//for (int row = 0; row < ui.gridLayout->rowCount(); ++row) {
	//	for (int col = 2; col <= 4; ++col) {
	//		QLayoutItem* item = ui.gridLayout->itemAtPosition(row, col);
	//		if (item && item->widget()) {
	//			item->widget()->setVisible(false);
	//		}
	//	}
	//}
}

bool AboutForm::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == ui.lblSoftVersion && event->type() == QEvent::MouseButtonPress) {
		m_adminClickCount++;
		if (m_adminClickCount >= 3) {
			m_adminClickCount = 0;
			QKeyEvent keyEvent(QEvent::KeyPress, Qt::Key_F12, Qt::NoModifier);
			QCoreApplication::sendEvent(window(), &keyEvent);
		}
	}
	return BaseChildWindow::eventFilter(obj, event);
}

AboutForm::~AboutForm()
{}
