#include "tool4coorset.h"
#include "widgets\qnumericpad.h"
#include "../modbus/NCMachine.h"
#include "widgets\filedialog.h"
#include "../utils/formutils.h"
#include "tool4switchcoor.h"

#define MAX_COOR_SET_COUNT 6

Tool4CoorSet::Tool4CoorSet(NCMachine* ncMachine, QWidget* parent)
    : QToolWidget(parent), ncMachine(ncMachine)
{
	ui.setupUi(this);

	QNumericPad::setButtonDownModeWhenInTouch(this);
}

Tool4CoorSet::~Tool4CoorSet()
{}

void Tool4CoorSet::on_btnRecord_clicked()
{
	auto controller = this->ncMachine->GetController();
	//int cs0 = 0;
	int csNow = controller->get(CURRENT_COORD_SYSTEM) - 1;
    if (!FormUtils::MessageBoxYesNo(tr("JLZBXSJ（SFYX？").arg(Tool4SwitchCoor::GetCoorName(csNow)))) {
        return;
    }

    double vsNow[LOC_AXIS_LEN + 1] = {};

    for (int csI = csNow; csI >= 0; --csI)
	{
        int row = csI + 1;
        for (int i = 0; i < LOC_AXIS_LEN + 1; ++i)
        {
            double v;
            if (i == 0)
            {
                v = csI < csNow + 1 ? csI : 0;
            }
            else {
                v = controller->getAxisPosition(GCodeTool::axis[i - 1]);
                double offset0 = controller->getAxisCSOffset(GCodeTool::axis[i - 1], csNow + 1);
                double offset = controller->getAxisCSOffset(GCodeTool::axis[i - 1], (csI < csNow + 1 ? csI : 0) + 1);
                v = v - offset + offset0;
            }
            if (i > 0) {
                if (csI == csNow) {
                    vsNow[i] = v;
                    v = 0;
                }
                else {
                    v -= vsNow[i];
                }
            }
            controller->set(GCodeTool::GetPosMomoryName(row - 1, i), v);
        }
	}

    QMetaObject::invokeMethod(BaseChildWindow::GetChildWindow(this), "ReloadData", Qt::QueuedConnection);
}

void Tool4CoorSet::on_btnBatchSet_clicked()
{
    if (!FormUtils::MessageBoxYesNo(tr("HFSJDZBX，SFYX？"))) {
        return;
    }

    auto controller = this->ncMachine->GetController();
    int csNow = controller->get(CURRENT_COORD_SYSTEM) - 1;
    int selectedIdx = -1;
    double delta[LOC_AXIS_LEN];
    for (int idx = 0; idx < POS_MEMORY_LEN; ++idx) {
        int csI = controller->get(GCodeTool::GetPosMomoryName(idx, 0));
        if (csI == csNow) {
            selectedIdx = idx;
            double v;
            for (size_t i = 0; i < LOC_AXIS_LEN; i++)
            {
                delta[i] = controller->get(GCodeTool::GetPosMomoryName(idx, i + 1));
                v = controller->getAxisPosition(GCodeTool::axis[i]);

				delta[i] = delta[i] - v;
            }
            break;
        }
    }
	if (selectedIdx == -1) {
		return;
	}

    QString gcode;

    for (int idx = 0; idx <= selectedIdx; ++idx) {
        int csI = controller->get(GCodeTool::GetPosMomoryName(idx, 0));
        if (idx > 0 && csI == 0)
            break;
        double vs[LOC_AXIS_LEN];
        for (size_t i = 0; i < LOC_AXIS_LEN; i++)
        {
            vs[i] = controller->get(GCodeTool::GetPosMomoryName(idx, i + 1)) - delta[i];
        }
        //if (LOC_AXIS_LEN == 3) {
        QString s = QString("G10 L20 P%1 X%2Y%3Z%4\n").arg(csI + 1).arg(vs[0], 0, 'f', 3).arg(vs[1], 0, 'f', 3).arg(vs[2], 0, 'f', 3);
        //}
        gcode += s;
    }
    this->ncMachine->RunGCode(gcode);
}

