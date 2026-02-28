#include "weizhiyidongform.h"
#include "ui_weizhiyidongform.h"
#include "zhuzhongxingform.h"
#include "../../modbus/NCMachine.h"
#include <utils/formutils.h>

WeiZhiYiDongForm::WeiZhiYiDongForm(QWidget* parent) :
	BaseChildWindowWithTools(parent), ui(new Ui::WeiZhiYiDongForm)
{
	ui->setupUi(this);

	ZhuZhongXingForm::setButton4LineEdit(ui->btnX);
	ZhuZhongXingForm::setButton4LineEdit(ui->btnY);
	ZhuZhongXingForm::setButton4LineEdit(ui->btnZ);

	ZhuZhongXingForm::setButton4ButtonEdit(ui->btnX_3);
	ZhuZhongXingForm::setButton4ButtonEdit(ui->btnY_3);
	ZhuZhongXingForm::setButton4ButtonEdit(ui->btnZ_3);

	if (LOC_AXIS_LEN >= 4) {
		ZhuZhongXingForm::setButton4LineEdit(ui->btnU);
		//ZhuZhongXingForm::setButton4ButtonEdit(ui->btnU_3);
	}
	else
	{
		ui->btnU->setEnabled(false);
	}
	ui->btnU_3->setEnabled(false);

	//ZhuZhongXingForm::setButton4LineEdit(ui->btnX_2);
	//ZhuZhongXingForm::setButton4LineEdit(ui->btnY_2);
	//ZhuZhongXingForm::setButton4LineEdit(ui->btnZ_2);
	connect(ui->inX, &QLineEdit4Keyboard::focusChanged, [this](bool b) {
		if (b && ui->inX->text().isEmpty()) {
			ui->inX->setText("0.000");
		}
		});
	connect(ui->inY, &QLineEdit4Keyboard::focusChanged, [this](bool b) {
		if (b && ui->inY->text().isEmpty()) {
			ui->inY->setText("0.000");
		}
		});
	connect(ui->inZ, &QLineEdit4Keyboard::focusChanged, [this](bool b) {
		if (b && ui->inZ->text().isEmpty()) {
			ui->inZ->setText("0.000");
		}
		});
	connect(ui->inU, &QLineEdit4Keyboard::focusChanged, [this](bool b) {
		if (b && ui->inU->text().isEmpty()) {
			ui->inU->setText("0.000");
		}
		});

	m_toClearInputWidgets.append(QList<QWidget*>() << ui->inX << ui->inY << ui->inZ << ui->inU << ui->inCoor);
	m_toClearInputWidgets.append(QList<QWidget*>() << ui->btnX_2 << ui->btnY_2 << ui->btnZ_2 << ui->btnU_2 << ui->inCoor_2);
	m_toClearInputWidgets.append(QList<QWidget*>() << ui->btnX_3_P << ui->btnX_3_N << ui->btnY_3_P << ui->btnY_3_N << ui->btnZ_3_P << ui->btnZ_3_N << ui->btnU_3_P << ui->btnU_3_N);
}

WeiZhiYiDongForm::~WeiZhiYiDongForm()
{
	delete ui;
}

void WeiZhiYiDongForm::RunGCode()
{
	tr("to_clear_machine_axis");

	QString gcode;
	gcode += "G27\n";
	gcode += "G09\n";
	if (ui->tabWidget->currentIndex() == 0)
	{
		if (ui->inAbsolute->isChecked())
			gcode += "G90\n";
		else
			gcode += "G91\n";
		if (!ui->inCoor->text().isEmpty())
			gcode += QString("G%0\n").arg(ui->inCoor->text());

		if (!ui->inTouchSense->isChecked())
			gcode += "M05";

		QString pos;
		if (!ui->inX->text().isEmpty())
			pos += QString("X%0").arg(ui->inX->text());
		if (!ui->inY->text().isEmpty())
			pos += QString("Y%0").arg(ui->inY->text());
		if (!ui->inZ->text().isEmpty())
			pos += QString("Z%0").arg(ui->inZ->text());
		if (!ui->inU->text().isEmpty())
			pos += QString("U%0").arg(ui->inU->text());
		if (!pos.isEmpty())
			gcode += QString("G00%0\n").arg(pos);
	}
	else if (ui->tabWidget->currentIndex() == 1) {
		gcode += "G90\n";
		if (!ui->inCoor_2->text().isEmpty())
			gcode += QString("G%0\n").arg(ui->inCoor_2->text());

		if (!ui->inTouchSense_2->isChecked())
			gcode += "M05";

		QString pos;
		if (ui->btnX_2->isChecked())
			pos += QString("X2 ");
		if (ui->btnY_2->isChecked())
			pos += QString("Y2 ");
		if (ui->btnZ_2->isChecked())
			pos += QString("Z2 ");
		if (ui->btnU_2->isChecked())
			pos += QString("U2 ");

		if (!pos.isEmpty()) {
			gcode += QString("G82 %01").arg(pos);
		}
	}
	else if (ui->tabWidget->currentIndex() == 2) {
		/*if (true) {
			if (ui->btnX_3_P->isChecked()) {
				gcode += "G81 X+\n";
			}
			else if (ui->btnX_3_N->isChecked()) {
				gcode += "G81 X-\n";
			}
		}
		if (true) {
			if (ui->btnY_3_P->isChecked()) {
				gcode += "G81 Y+\n";
			}
			else if (ui->btnY_3_N->isChecked()) {
				gcode += "G81 Y-\n";
			}
		}
		if (true) {
			if (ui->btnZ_3_P->isChecked()) {
				gcode += "G81 Z+\n";
			}
			else if (ui->btnZ_3_N->isChecked()) {
				gcode += "G81 Z-\n";
			}
		}
		if (true) {
			if (ui->btnU_3_P->isChecked()) {
				gcode += "G81 U+\n";
			}
			else if (ui->btnU_3_N->isChecked()) {
				gcode += "G81 U-\n";
			}
		}*/
		int selectedAxisI = -1;
		if (ui->btnX_3_P->isChecked()) {
			//ncMachine->BacktoZero(DdfxEnum::X_PLUS);
			gcode = "do_backtozero X\n";
			selectedAxisI = 0;
		}
		else if (ui->btnX_3_N->isChecked()) {
			//ncMachine->BacktoZero(DdfxEnum::X_MINUS);
			gcode = "do_backtozeron Z\n";
			selectedAxisI = 0;
		}
		else if (ui->btnY_3_P->isChecked()) {
			//ncMachine->BacktoZero(DdfxEnum::Y_PLUS);
			gcode = "do_backtozero Y\n";
			selectedAxisI = 1;
		}
		else if (ui->btnY_3_N->isChecked()) {
			//ncMachine->BacktoZero(DdfxEnum::Y_MINUS);
			gcode = "do_backtozeron Y\n";
			selectedAxisI = 1;
		}
		else if (ui->btnZ_3_P->isChecked()) {
			//ncMachine->BacktoZero(DdfxEnum::Z_PLUS);
			gcode = "do_backtozero Z\n";
			selectedAxisI = 2;
		}
		else if (ui->btnZ_3_N->isChecked()) {
			//ncMachine->BacktoZero(DdfxEnum::Z_MINUS);
			gcode = "do_backtozeron Z\n";
			selectedAxisI = 2;
		}
		if (!gcode.isEmpty() && selectedAxisI != -1) {
			gcode += "do_message_yn to_clear_machine_axis\n";
			gcode += QString(R"(o101 if[#<message_ret> EQ 1]
do_zeroaxis %1
o101 endif)").arg(GCodeTool::axisInt[selectedAxisI]);
		}
		//ncMachine->enterSetPriority(1);
		//ncMachine->GoApi();
		//std::function<void()> doFuncZeroAxis2 = [this, selectedAxisI]() {
		//	QMetaObject::invokeMethod(this, [this, selectedAxisI]() {
		//		bool b = FormUtils::MessageBoxYesNo(tr("SFQLJXZB？"));
		//		if (b) {
		//			ncMachine->enterSetPriority(1);
		//			ncMachine->GoApi();
		//			for (int i = selectedAxisI; i < selectedAxisI + 1; ++i) {
		//				ncMachine->SetPosOne(TMBS_MAP0_ID_XPOS + i, 0);
		//				//std::function<void(int, ModbusTask*, ModbusAdapter*)> function = [this, i](int ret, ModbusTask* task, ModbusAdapter* adapter) {
		//				//	m_ncMachine->GetController()->setAxisAbsolutePosition(GCodeTool::axis[i], 0, GCode::Units::METRIC);
		//				//	task->startAddr = 1;
		//				//	};
		//				//m_ncMachine->executeCmdWait(function);
		//				std::function<void()> doFuncZeroAxisInController = [this, i]() {
		//					ncMachine->GetController()->setAxisAbsolutePosition(GCodeTool::axis[i], 0, GCode::Units::METRIC);
		//					};
		//				ncMachine->executeCmdWait(ncMachine->convertWaitFunction(ncMachine->waitno(doFuncZeroAxisInController)), "doFuncZeroAxisInController");
		//			}
		//			ncMachine->GoJog();
		//			ncMachine->exitSetPriority();
		//		}
		//		}, Qt::QueuedConnection);
		//	};
		//ncMachine->executeCmdWait(ncMachine->convertWaitFunction(ncMachine->waitno(doFuncZeroAxis2)), "doFuncZeroAxis");

		//ncMachine->GoJog();
		//ncMachine->exitSetPriority();
	}

	if (!gcode.isEmpty())
	{
		//ui->textBrowser->setText(gcode);
		m_ncMachine->RunGCode(gcode);
	}

	ClearInputWidgets();
}

void WeiZhiYiDongForm::ClearInputWidgets(QWidget* parent)
{
	if (parent == NULL) {
		BaseChildWindow::ClearInputWidgets(ui->tabWidget->currentWidget());
	}
	else {
		BaseChildWindow::ClearInputWidgets(parent);
	}
}

void WeiZhiYiDongForm::on_btnX_clicked()
{
	ZhuZhongXingForm::toggleLineEdit(!ui->btnX->isChecked(), ui->inX);
}
void WeiZhiYiDongForm::on_btnY_clicked()
{
	ZhuZhongXingForm::toggleLineEdit(!ui->btnY->isChecked(), ui->inY);
}
void WeiZhiYiDongForm::on_btnZ_clicked()
{
	ZhuZhongXingForm::toggleLineEdit(!ui->btnZ->isChecked(), ui->inZ);
}
void WeiZhiYiDongForm::on_btnU_clicked()
{
	ZhuZhongXingForm::toggleLineEdit(!ui->btnU->isChecked(), ui->inU);
}

void WeiZhiYiDongForm::on_btnX_2_clicked()
{
	//ZhuZhongXingForm::toggleLineEdit(!ui->btnX_2->isChecked(), ui->inX_2);
}

void WeiZhiYiDongForm::on_btnY_2_clicked()
{
}

void WeiZhiYiDongForm::on_btnZ_2_clicked()
{
}

void WeiZhiYiDongForm::on_btnU_2_clicked()
{
}

void WeiZhiYiDongForm::on_btnX_3_clicked()
{
	bool b = ui->btnX_3->isChecked();

	ZhuZhongXingForm::toggleButtonEdit(b, ui->btnX_3_P, ui->btnX_3_N);
}

void WeiZhiYiDongForm::on_btnY_3_clicked()
{
	bool b = ui->btnY_3->isChecked();
	ZhuZhongXingForm::toggleButtonEdit(b, ui->btnY_3_P, ui->btnY_3_N);
}

void WeiZhiYiDongForm::on_btnZ_3_clicked()
{
	bool b = ui->btnZ_3->isChecked();
	ZhuZhongXingForm::toggleButtonEdit(b, ui->btnZ_3_P, ui->btnZ_3_N);
}

void WeiZhiYiDongForm::on_btnU_3_clicked()
{
	bool b = ui->btnU_3->isChecked();
	ZhuZhongXingForm::toggleButtonEdit(b, ui->btnU_3_P, ui->btnU_3_N);
}
