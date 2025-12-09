#include "gaotiao4jiagong.h"
#include "boollineedit.h"
#include "qlineedit4axisvalue.h"
#include "../modbus/NCMachineProperties.h"
#include "../modbus/NCMachine.h"

Gaotiao4Jiagong::Gaotiao4Jiagong(NCMachine* ncMachine, QWidget* parent)
	: BaseChildWindow(parent)
{
	BaseChildWindow::SetMachine(ncMachine);

	ui.setupUi(this);

	m_dynamicForm = new DynamicForm(this);
	m_dynamicForm->setObjectName(this->objectName() + "_gaotiao4jiagong");
	m_dynamicForm->isHorizontalLayout = false;

	std::function<ILineEdit* (QString, QWidget*)> funcToCreateEditor = [](QString caption, QWidget* parent) {
		ILineEdit* ret = NULL;
		if (caption == "Open") {
			ret = new BoolLineEdit(parent);
		}
		else {
			auto r = new QLineEdit4Keyboard(parent);
			r->setAllowNull(false);
			ret = r;
		}
		return ret;
		};
	m_dynamicForm->createForm(QStringList() << "Open" << "Interval" << "Height", funcToCreateEditor);
	m_dynamicForm->setLabelCaption("Open", QStringLiteral("设定"));	
	m_dynamicForm->setLabelCaption("Interval", QStringLiteral("周期"));
	m_dynamicForm->setLabelCaption("Height", QStringLiteral("高度"));

	this->setLayout(m_dynamicForm);
	//this->ui.btnOk->hide();
}

Gaotiao4Jiagong::~Gaotiao4Jiagong()
{}

void Gaotiao4Jiagong::LoadData(QString filePath)
{
	if (filePath != NULL) {
		throw std::invalid_argument("filePath must be NULL");
	}
	auto p = PropertyObjects::getInstance()->propertyObjectFdwc;
	if (p->ectdxydtdcs() == 0 && p->ectdgdbs() == 0)
	{
		m_dynamicForm->setValue("Open", "0");
	}
	else {
		m_dynamicForm->setValue("Open", "1");
	}
	m_dynamicForm->setValue("Interval", QString::number(p->ectdxydtdcs()));
	m_dynamicForm->setValue("Height", QString::number(p->ectdgdbs()));
}

void Gaotiao4Jiagong::RunGCode()
{
	auto p = PropertyObjects::getInstance()->propertyObjectFdwc;
	if (m_dynamicForm->getValue("Open") == "0") {
		p->setectdxydtdcs(0);
		p->setectdgdbs(0);
	}
	else {
		p->setectdxydtdcs(m_dynamicForm->getValue("Interval").toInt());
		p->setectdgdbs(m_dynamicForm->getValue("Height").toInt());
	}
	m_ncMachine->enterSetPriority(0);
	p->ExecuteCmds(m_ncMachine);
	m_ncMachine->exitSetPriority();
}