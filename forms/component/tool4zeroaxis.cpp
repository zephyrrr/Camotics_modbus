#include "tool4zeroaxis.h"
#include "widgets\qnumericpad.h"
#include "../modbus/NCMachine.h"

Tool4ZeroAxis::Tool4ZeroAxis(NCMachine* ncMachine, QWidget *parent, QList<char> axisNames)
	: QToolWidget(parent), m_ncMachine(ncMachine)
{
	ui.setupUi(this);
	if (axisNames.count() <= 3 && axisNames.count() > 0) {
		m_axisNames = axisNames;
	}
	else {
		m_axisNames = {'X', 'Y', 'Z'};
	}
	ui.btnSetXZero->setText(QString("%1 = 0").arg(m_axisNames[0]));
	if (m_axisNames.count() > 1)
		ui.btnSetYZero->setText(QString("%1 = 0").arg(m_axisNames[1]));
	else {
		ui.btnSetYZero->setText("");
		ui.btnSetYZero->setEnabled(false);
	}
	if (m_axisNames.count() > 2)
		ui.btnSetZZero->setText(QString("%1 = 0").arg(m_axisNames[2]));
	else {
		ui.btnSetZZero->setText("");
		ui.btnSetZZero->setEnabled(false);
	}

	QNumericPad::setButtonDownModeWhenInTouch(this);
}

Tool4ZeroAxis::~Tool4ZeroAxis()
{}

void Tool4ZeroAxis::on_btnSetXZero_clicked()
{
	m_ncMachine->RunGCodeSync(QString("G92 %010").arg(m_axisNames[0]));
}

void Tool4ZeroAxis::on_btnSetYZero_clicked()
{
	m_ncMachine->RunGCodeSync(QString("G92 %010").arg(m_axisNames[1]));
}

void Tool4ZeroAxis::on_btnSetZZero_clicked()
{
	m_ncMachine->RunGCodeSync(QString("G92 %010").arg(m_axisNames[2]));
}
