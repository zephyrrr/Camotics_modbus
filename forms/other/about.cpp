#include "about.h"
#include "modbus/NCMachineDefines.h"
#include "modbus/NCCommand.h"
#include "modbus/modbusadapter.h"
#include "modbus/NCMachine.h"

AboutForm::AboutForm(QWidget *parent)
	: BaseChildWindow(parent)
{
	ui.setupUi(this);

	ui.lblSoftVersion->setText(QApplication::applicationVersion());
	ui.lblHardwareVersion1->setText(PropertyObjects::getInstance()->propertyObjectVersion->versionSkh());
	ui.lblHardwareVersion2->setText(PropertyObjects::getInstance()->propertyObjectVersion->versionXskz());
	ui.lblHardwareVersion3->setText(PropertyObjects::getInstance()->propertyObjectVersion->versionXwj());
	ui.lblHardwareVersion4->setText(PropertyObjects::getInstance()->propertyObjectVersion->versionXwjnh());
}

AboutForm::~AboutForm()
{}
