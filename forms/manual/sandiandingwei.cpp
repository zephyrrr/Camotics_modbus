#include "sandiandingwei.h"
#include "ui_sandiandingwei.h"
#include <QLabel>
#include <QButtonGroup>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollArea>
#include "zhuzhongxingform.h"
#include "utils/MathTool.h"
#include "../modbus/NCMachine.h"
#include "utils/gcodeutils.h"
#include "utils/pluginutils.h"

SanDianDingWeiForm::SanDianDingWeiForm(QWidget* parent) :
	BaseChildWindowWithTools(parent, TOOL_ZERO_AXIS | TOOL_SWITCH_COOR | TOOL_COOR_ITER),
	ui(new Ui::SanDianDingWeiForm)
{
	ui->setupUi(this);

	//ui->formLayout->setHorizontalSpacing(20);
	//ui->formLayout->setVerticalSpacing(20);

	//ui->gridLayout->removeWidget(ui->label_6);
	//ui->gridLayout->removeWidget(ui->label_3);
	//ui->gridLayout->removeItem(ui->horizontalLayout);
	//ui->gridLayout->removeItem(ui->horizontalLayout_3);

	ZhuZhongXingForm::SetupTable4Result(ui->tableView, QStringList() << "X" << "Y" << "R");

	ZhuZhongXingForm::setButton4LineEdit(ui->btnX);
	ZhuZhongXingForm::setButton4LineEdit(ui->btnZ);
	ZhuZhongXingForm::setButton4LineEdit(ui->btnA1);
	ZhuZhongXingForm::setButton4LineEdit(ui->btnA2);
	ZhuZhongXingForm::setButton4LineEdit(ui->btnA3);

	QRegExp rx("[123]");
	QRegExpValidator* validator = new QRegExpValidator(rx, this);
	ui->inCnt->setValidator(validator);
}

SanDianDingWeiForm::~SanDianDingWeiForm()
{
	delete ui;
}

void SanDianDingWeiForm::on_btnX_clicked()
{
	ZhuZhongXingForm::toggleLineEdit(!ui->btnX->isChecked(), ui->inX);
}
void SanDianDingWeiForm::on_btnA1_clicked()
{
	ZhuZhongXingForm::toggleLineEdit(!ui->btnA1->isChecked(), ui->inA1);
}
void SanDianDingWeiForm::on_btnA2_clicked()
{
	ZhuZhongXingForm::toggleLineEdit(!ui->btnA2->isChecked(), ui->inA2);
}
void SanDianDingWeiForm::on_btnA3_clicked()
{
	ZhuZhongXingForm::toggleLineEdit(!ui->btnA3->isChecked(), ui->inA3);
}
void SanDianDingWeiForm::on_btnZ_clicked()
{
	ZhuZhongXingForm::toggleLineEdit(!ui->btnZ->isChecked(), ui->inZ);
}

void SanDianDingWeiForm::RunGCode()
{
	double inKuaiJingR = ui->inX->text().toDouble();
	//double inKuaiJingY = ui->inY->text().toDouble();
	double inKuaiJingZ = ui->inZ->text().toDouble();
	double inKuaiJingA1 = ui->inA1->text().toDouble();
	double inKuaiJingA2 = ui->inA2->text().toDouble();
	double inKuaiJingA3 = ui->inA3->text().toDouble();
	double inJieChuHouFanZhuang = ui->inBack->text().toDouble();
	//bool inSetToZero = ui->inSetToZero->isChecked();
	//bool inTouchSenseZ = ui->inSetToZTouch->isChecked();
	bool inWaiJing = ui->inPositionOuter->isChecked();

	int inCnt = ui->inCnt->text().toInt();
	if (inCnt <= 0 || inCnt > 3) {
		inCnt = 1;
	}
	ZhuZhongXingForm::ClearTable4Result(ui->tableView);

	QString gcode2 = GCodeUtils::Sandiandingwei(inKuaiJingR, inKuaiJingZ, inKuaiJingA1, inKuaiJingA2, inKuaiJingA3, inJieChuHouFanZhuang, inWaiJing);
	//ui->textBrowser->setText(gcode2);
#ifdef PYTHON_SUPPORT
	QMap<QString, QString> pyFiles = PluginUtils::loadPythonScripts(this->objectName());
	for (const auto& pyFileName : pyFiles.keys()) {
		if (pyFileName == "rungcode") {
			this->SetData("inKuaiJingR", inKuaiJingR);

			QString value = pyFiles.value(pyFileName);
			PluginUtils::RunFile(value, this);

			QVariant result = this->GetData("result");
			if (result != NULL && result.isValid()) {
				gcode2 = this->GetData("result").value<QString>();
			}
		}
	}
#endif

	QString gcode = gcode2;
	for (int i = 0; i < inCnt; ++i) {
		gcode += "o904 call\n";
	}
	gcode += "do_beep 100\n";

	m_ncMachine->GetRealtimeJsonMachine()->setFunctionDoWhenCallUI([this](std::string s) {
		QMetaObject::invokeMethod(this, [this, s]() {
			auto ss = QString::fromStdString(s).split(',');
			if (ss.count() != 6)
				return;
			double cx, cy, r;
			MathTool::GetCircleFromThreePoint(ss[0].toDouble(), ss[1].toDouble(), ss[2].toDouble(), ss[3].toDouble(), ss[4].toDouble(), ss[5].toDouble(), cx, cy, r);
			ZhuZhongXingForm::AddTable4Result(ui->tableView, QString("%01,%02,%03").arg(cx, 0, 'f', 3).arg(cy, 0, 'f', 3).arg(r, 0, 'f', 3));

			}, Qt::QueuedConnection);
		});

	m_ncMachine->RunGCode(gcode, [this] {
		QMetaObject::invokeMethod(this, [this]() {
			try {
				QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->tableView->model());
				QStandardItem* x = model->item(model->rowCount() - 1, 1);
				QStandardItem* y = model->item(model->rowCount() - 1, 2);
				if (x != NULL && y != NULL) {
					QString x1 = QString("X%1").arg(x->text().toDouble(), 0, 'f', 3);
					QString y1 = QString("Y%1").arg(y->text().toDouble(), 0, 'f', 3);
					QString gcodeHere = "";
					if (!x1.isEmpty() || y1.isEmpty()) {
						gcodeHere += QString("G90 G00 %1%2\n").arg(x1).arg(y1);
					}
					m_ncMachine->RunGCode(gcodeHere);
				}
			}
			catch (...) {

			}
			}, Qt::QueuedConnection);
		});
}