#include "tool4halfaxis.h"
#include "widgets\qnumericpad.h"
#include "../modbus/NCMachine.h"

Tool4HalfAxis::Tool4HalfAxis(NCMachine* ncMachine, QWidget *parent, QList<char> axisNames)
	: QToolWidget(parent), m_ncMachine(ncMachine)
{
	ui.setupUi(this);
	if (axisNames.count() <= 3 && axisNames.count() > 0) {
		m_axisNames = axisNames;
	}
	else {
		m_axisNames = { 'X', 'Y', 'Z' };
	}
	ui.btnSetXHalf->setText(QString("%1 1/2").arg(m_axisNames[0]));
	if (m_axisNames.count() > 1)
		ui.btnSetYHalf->setText(QString("%1 1/2").arg(m_axisNames[1]));
	else {
		ui.btnSetYHalf->setText("");
		ui.btnSetYHalf->setEnabled(false);
	}
	if (m_axisNames.count() > 2)
		ui.btnSetZHalf->setText(QString("%1 = 1").arg(m_axisNames[2]));
	else {
		ui.btnSetZHalf->setText("");
		ui.btnSetZHalf->setEnabled(false);
	}
	QNumericPad::setButtonDownModeWhenInTouch(this);
}

Tool4HalfAxis::~Tool4HalfAxis()
{}


void Tool4HalfAxis::on_btnSetXHalf_clicked()
{
	char a = m_axisNames[0];
	m_ncMachine->RunGCodeSync(QString("G92 %01%02").arg(a).arg(m_ncMachine->GetController()->getAxisPosition(a) / 2, 0, 'f', 3));
}

void Tool4HalfAxis::on_btnSetYHalf_clicked()
{
	char a = m_axisNames[1];
	m_ncMachine->RunGCodeSync(QString("G92 %01%02").arg(a).arg(m_ncMachine->GetController()->getAxisPosition(a) / 2, 0, 'f', 3));
}

void Tool4HalfAxis::on_btnSetZHalf_clicked()
{
	//char a = m_axisNames[2];
	m_ncMachine->RunGCodeSync(QString("G92 Z1"));
	//m_ncMachine->RunGCodeSync(QString("G92 %01%02").arg(a).arg(m_ncMachine->GetController()->getAxisPosition(a) / 2, 0, 'f', 3));
}
