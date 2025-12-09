#include "tool4itercoor.h"
#include "widgets\qnumericpad.h"
#include "../modbus/NCMachine.h"
#include "../utils/formutils.h"

Tool4IterCoor::Tool4IterCoor(BaseChildWindow* childWindow, QWidget *parent)
	: QToolWidget(parent), m_childWindow(childWindow), thread_finished(true)
{
	ui.setupUi(this);
	//ui.btnCurrentCoor->hide();
	QNumericPad::setButtonDownModeWhenInTouch(this);
}

Tool4IterCoor::~Tool4IterCoor()
{}

void Tool4IterCoor::UpdateState()
{
	auto ncMachine = m_childWindow->GetMachine();
	if (ncMachine->IsGCodeRunning() || !thread_finished.load()) {
		ui.btnRunIterCoor->setEnabled(false);
		return;
	}
	int csIndexNow = ncMachine->GetController()->get(CURRENT_COORD_SYSTEM) - 1;
	int currentCoor = csIndexNow / 6 * 100 + (54 + csIndexNow % 6);

	int x = currentCoor % 100;
	int n = currentCoor / 100;
	QString gcode = QString("G%1%2").arg(n).arg(x);

	ui.btnCurrentCoor->setText(gcode);

	ui.btnRunIterCoor->setEnabled(thread_finished.load());
}

void Tool4IterCoor::on_btnRunIterCoor_clicked()
{
	if (!FormUtils::MessageBoxYesNo(QStringLiteral("ÊÇ·ñÔËÐÐ£¿"))) {
		return;
	}
	//
	//QString gcode;

	//QRegularExpression regex("o(\\d+)\\ssub.*?o(\\d+)\\sendsub", QRegularExpression::DotMatchesEverythingOption);

	if (m_thread.get_id() != std::thread::id())
	{
		if (m_thread.joinable()) {
			m_thread.join();
		}
	}
	thread_finished.store(false);

	m_thread = std::thread([this]() {
		auto ncMachine = m_childWindow->GetMachine();
		int csIndexNow = ncMachine->GetController()->get(CURRENT_COORD_SYSTEM) - 1;
		for (int csIndex = 0; csIndex <= csIndexNow; ++csIndex) {
			int currentCoor = csIndex / 6 * 100 + (54 + csIndex % 6);

			int x = currentCoor % 100;
			int n = currentCoor / 100;
			QString gcode = "";
			gcode += QString("G%1%2\n").arg(n).arg(x);
			gcode += "G90 G00 X0Y0\n";

			//QString gcode2 = m_childWindow->GetGCode();
			//if (csIndex > 0) {
			//	gcode2 = gcode2.replace(regex, "");
			//}
			//gcode += gcode2;

			ncMachine->RunGCode(gcode);
			int r = ncMachine->WaitUntilRunFinished();
			if (r != 1)
				break;
			m_childWindow->RunGCode();
			r = ncMachine->WaitUntilRunFinished();
			if (r != 1)
				break;
		}
		thread_finished.store(true);
		});
	//m_thread.join();
}
