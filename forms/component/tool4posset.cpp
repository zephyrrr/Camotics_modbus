#include "tool4posset.h"
#include "widgets\qnumericpad.h"
#include "../modbus/NCMachine.h"
#include "widgets\filedialog.h"
#include "widgets/qnumericpad.h"

#define POS_SET_COUNT_PERPAGE 6

Tool4PosSet::Tool4PosSet(NCMachine* ncMachine, QWidget *parent)
	: QToolWidget(parent), ncMachine(ncMachine)
{
	ui.setupUi(this);
	ui.btnCurrentPosIdx->setText(QString::number(m_currentPosIdx));
    ui.btnCurrentPosIdx->setEnabled(true);

	QNumericPad::setButtonDownModeWhenInTouch(this);
}

Tool4PosSet::~Tool4PosSet()
{}

void Tool4PosSet::on_btnRecord_clicked()
{
    auto controller = this->ncMachine->GetController();

    // G54 = 1
    //int row = 1 + controller->get(CURRENT_COORD_SYSTEM) - 1;
    int row = m_currentPosIdx;
    if (row > POS_SET_COUNT_PERPAGE) {
        return;
    }
    for (int i = 0; i < LOC_AXIS_LEN + 1; ++i)
    {
        double v;
        if (i == 0)
        {
            v = controller->get(CURRENT_COORD_SYSTEM) - 1;
            controller->set(GCodeTool::GetPosMomoryName(row - 1, i), v);
        }
        else {
            v = controller->getAxisPosition(GCodeTool::axis[i - 1]);
            controller->set(GCodeTool::GetPosMomoryName(row - 1, i), v);
        }

        //QString s;
        //if (i == 0) {
        //    int csIndex = (int)v;
        //    s = QString("%1%2").arg(csIndex / 6).arg(GCodeTool::csNames[csIndex % 6]);
        //}
        //else {
        //    s = QString::number(v, 'f', 3);
        //}
    }
    ncMachine->serialize();

    m_currentPosIdx++;
    if (m_currentPosIdx > POS_SET_COUNT_PERPAGE)
        m_currentPosIdx = 1;
    ui.btnCurrentPosIdx->setText(QString::number(m_currentPosIdx));
}

void Tool4PosSet::on_btnBatchSet_clicked()
{
    auto controller = this->ncMachine->GetController();
    double ds[4];
    for (int i = 0; i < 4; ++i)
    {
        ds[i] = controller->getAxisPosition(GCodeTool::axis[i]) - controller->get(GCodeTool::GetPosMomoryName(m_currentPosIdx - 1, i + 1));
    }

    for (int row = 1; row <= POS_SET_COUNT_PERPAGE; row++)
    {
        for (int i = 0; i < LOC_AXIS_LEN + 1; ++i)
        {
            double v;
            if (i == 0)
            {
            }
            else {
                v = controller->get(GCodeTool::GetPosMomoryName(row - 1, i));
				v += ds[i - 1];
                controller->set(GCodeTool::GetPosMomoryName(row - 1, i), v);
            }
        }
    }
    ncMachine->serialize();
}

void Tool4PosSet::on_btnCurrentPosIdx_clicked()
{
    m_currentPosIdx++;
    if (m_currentPosIdx > POS_SET_COUNT_PERPAGE)
        m_currentPosIdx = 1;
    ui.btnCurrentPosIdx->setText(QString::number(m_currentPosIdx));
}

