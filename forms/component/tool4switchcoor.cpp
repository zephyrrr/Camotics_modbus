#include "tool4switchcoor.h"
#include "widgets\qnumericpad.h"
#include "../modbus/NCMachine.h"

Tool4SwitchCoor::Tool4SwitchCoor(NCMachine* ncMachine, QWidget *parent)
	: QToolWidget(parent), m_ncMachine(ncMachine)
{
	ui.setupUi(this);

	QNumericPad::setButtonDownModeWhenInTouch(this);

	UpdateState();
}

Tool4SwitchCoor::~Tool4SwitchCoor()
{}

void Tool4SwitchCoor::UpdateState()
{
	int csIndex = m_ncMachine->GetController()->get(CURRENT_COORD_SYSTEM) - 1;
	m_currentCoor = csIndex / 6 * 100 + (54 + csIndex % 6);

	int x = m_currentCoor % 100;
	int n = m_currentCoor / 100;
	ui.btnCurrentCoor->setText(QString("G%1%2").arg(n).arg(x));

	bool b = m_ncMachine->IsGCodeRunning();
	ui.btnSwitchCoorLeft->setEnabled(!b);
	ui.btnSwitchCoorRight->setEnabled(!b);
}

QString Tool4SwitchCoor::GetCoorName(int csIndex)
{
	int currentCoor = csIndex / 6 * 100 + (54 + csIndex % 6);

	int x = currentCoor % 100;
	int n = currentCoor / 100;
	return QString("G%1%2").arg(n).arg(x);
}

void Tool4SwitchCoor::on_btnSwitchCoorLeft_clicked()
{
	bool isRunning = m_ncMachine->IsGCodeRunning();
	if (isRunning)
		return;

	int x = m_currentCoor % 100;
	int n = m_currentCoor / 100;
	if (x == 54)
	{
		if (n > 0) {
			x = 59;
			n--;
		}
	}
	else
	{
		x--;
	}
	m_currentCoor = n * 100 + x;
	//ui.btnCurrentCoor->setText(QString("G%1%2").arg(n).arg(x));
	m_ncMachine->RunGCodeSync(QString("G%1").arg(m_currentCoor));
}

void Tool4SwitchCoor::on_btnSwitchCoorRight_clicked()
{
	bool isRunning = m_ncMachine->IsGCodeRunning();
	if (isRunning)
		return;

	int x = m_currentCoor % 100;
	int n = m_currentCoor / 100;
	if (x == 59)
	{
		if (n < 9) {
			x = 54;
			n++;
		}
	}
	else
	{
		x++;
	}
	m_currentCoor = n * 100 + x;
	//ui.btnCurrentCoor->setText(QString("G%1%2").arg(n).arg(x));
	m_ncMachine->RunGCodeSync(QString("G%1").arg(m_currentCoor));
}
