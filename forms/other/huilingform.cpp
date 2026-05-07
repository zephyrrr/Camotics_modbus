#include "huilingform.h"
#include "zhuzhongxingform.h"
#include "utils/formutils.h"
#include "modbus/NCMachine.h"

HuiLingForm::HuiLingForm(QWidget *parent)
	: BaseChildWindow(parent)
{
	ui.setupUi(this);

    ui.btnU->setEnabled(false);
}

HuiLingForm::~HuiLingForm()
{}

void HuiLingForm::on_btnX_clicked()
{
    //ZhuZhongXingForm::toggleLineEdit(ui->inX);
    ZhuZhongXingForm::toggleButtonEdit(ui.btnX->isChecked(), ui.btnX_P, ui.btnX_N);
}
void HuiLingForm::on_btnY_clicked()
{
    ZhuZhongXingForm::toggleButtonEdit(ui.btnY->isChecked(), ui.btnY_P, ui.btnY_N);
}
void HuiLingForm::on_btnZ_clicked()
{
    ZhuZhongXingForm::toggleButtonEdit(ui.btnZ->isChecked(), ui.btnZ_P, ui.btnZ_N);
}
void HuiLingForm::on_btnU_clicked()
{
    ZhuZhongXingForm::toggleButtonEdit(ui.btnU->isChecked(), ui.btnU_P, ui.btnU_N);
}

void HuiLingForm::RunGCode()
{
	m_ncMachine->enterSetPriority(1);

	int selectedAxisI = -1;
    if (ui.btnX_P->isChecked()) {
		m_ncMachine->BacktoZero(DdfxEnum::X_PLUS);
		selectedAxisI = 0;
    } else if (ui.btnX_N->isChecked()) {
        m_ncMachine->BacktoZero(DdfxEnum::X_MINUS);
		selectedAxisI = 0;
    }
    else if (ui.btnY_P->isChecked()) {
		m_ncMachine->BacktoZero(DdfxEnum::Y_PLUS);
		selectedAxisI = 1;
    }
    else if (ui.btnY_N->isChecked()) {
		m_ncMachine->BacktoZero(DdfxEnum::Y_MINUS);
		selectedAxisI = 1;
    }
    else if (ui.btnZ_P->isChecked()) {
		m_ncMachine->BacktoZero(DdfxEnum::Z_PLUS);
		selectedAxisI = 2;
    }
    else if (ui.btnZ_N->isChecked()) {
		m_ncMachine->BacktoZero(DdfxEnum::Z_MINUS);
		selectedAxisI = 2;
    }

    m_ncMachine->GoApi();
	std::function<void()> doFuncZeroAxis2 = [this, selectedAxisI]() {
		QMetaObject::invokeMethod(this, [this, selectedAxisI]() {
			bool b = FormUtils::MessageBoxYesNo(tr("SFQLJXZB"));
			if (b) {
				m_ncMachine->enterSetPriority(1);
				m_ncMachine->GoApi();
				for (int i = selectedAxisI; i < selectedAxisI + 1; ++i) {
					m_ncMachine->SetPosOne(TMBS_MAP0_ID_XPOS + i, 0);
					//std::function<void(int, ModbusTask*, ModbusAdapter*)> function = [this, i](int ret, ModbusTask* task, ModbusAdapter* adapter) {
					//	m_ncMachine->GetController()->setAxisAbsolutePosition(GCodeTool::axis[i], 0, GCode::Units::METRIC);
					//	task->startAddr = 1;
					//	};
					//m_ncMachine->executeCmdWait(function);
					std::function<void()> doFuncZeroAxisInController = [this, i]() {
						m_ncMachine->GetController()->setAxisAbsolutePosition(GCodeTool::axis[i], 0, GCode::Units::METRIC);
						};
					m_ncMachine->executeCmdWait(m_ncMachine->convertWaitFunction(m_ncMachine->waitno(doFuncZeroAxisInController)), "doFuncZeroAxisInController");
				}
				m_ncMachine->GoJog();
				m_ncMachine->exitSetPriority();
			}
			}, Qt::QueuedConnection);
		};
	m_ncMachine->executeCmdWait(m_ncMachine->convertWaitFunction(m_ncMachine->waitno(doFuncZeroAxis2)), "doFuncZeroAxis");

	m_ncMachine->GoJog();
	m_ncMachine->exitSetPriority();
}
