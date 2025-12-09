#include "jiaodingweiform.h"
#include "ui_jiaodingweiform.h"
#include <QLabel>
#include <QButtonGroup>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollArea>
#include "zhuzhongxingform.h"
#include "../modbus/NCMachine.h"
#include "utils/gcodeutils.h"

JiaoDingWeiForm::JiaoDingWeiForm(QWidget *parent) :
    BaseChildWindowWithTools(parent, TOOL_ZERO_AXIS | TOOL_SWITCH_COOR | TOOL_COOR_ITER),
    ui(new Ui::JiaoDingWeiForm)
{
    ui->setupUi(this);

    //ui->formLayout->removeRow(5);
    ZhuZhongXingForm::SetupTable4Result(ui->tableView, QStringList() << "X" << "Y");

    ZhuZhongXingForm::setButton4LineEdit(ui->btnX);
    ZhuZhongXingForm::setButton4LineEdit(ui->btnY);
    ZhuZhongXingForm::setButton4LineEdit(ui->btnZ);

    QRegExp rx("[123]");
    QRegExpValidator* validator = new QRegExpValidator(rx, this);
    ui->inCnt->setValidator(validator);
}

JiaoDingWeiForm::~JiaoDingWeiForm()
{
    delete ui;
}

void JiaoDingWeiForm::on_btnX_clicked()
{
    ZhuZhongXingForm::toggleLineEdit(!ui->btnX->isChecked(), ui->inX);
}
void JiaoDingWeiForm::on_btnY_clicked()
{
    ZhuZhongXingForm::toggleLineEdit(!ui->btnY->isChecked(), ui->inY);
}
void JiaoDingWeiForm::on_btnZ_clicked()
{
    ZhuZhongXingForm::toggleLineEdit(!ui->btnZ->isChecked(), ui->inZ);
}


void JiaoDingWeiForm::RunGCode()
{
    QString gcode = GetGCode();

    double inKuaiJingX = ui->inX->text().toDouble();
    double inKuaiJingY = ui->inY->text().toDouble();
    bool inSetToZero = ui->inSetToZero->isChecked();
    ZhuZhongXingForm::ClearTable4Result(ui->tableView);

    m_ncMachine->GetRealtimeJsonMachine()->setFunctionDoWhenCallUI([this](std::string s) {
        QMetaObject::invokeMethod(this, [this, s]() {
            ZhuZhongXingForm::AddTable4Result(ui->tableView, QString::fromStdString(s));
            }, Qt::QueuedConnection);
        });

    m_ncMachine->RunGCode(gcode, [this, inSetToZero, inKuaiJingX, inKuaiJingY] {
        QMetaObject::invokeMethod(this, [this, inSetToZero, inKuaiJingX, inKuaiJingY]() {
            try {
                QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->tableView->model());
                QStandardItem* x = model->item(model->rowCount() - 1, 1);
                QStandardItem* y = model->item(model->rowCount() - 1, 2);
                if (x != NULL && y != NULL) {
                    QString x1 = inKuaiJingX != 0 ? QString("X%1").arg(x->text().toDouble(), 0, 'f', 3) : "";
                    QString y1 = inKuaiJingY != 0 ? QString("Y%1").arg(y->text().toDouble(), 0, 'f', 3) : "";
                    QString gcodeHere = "";
                    if (!x1.isEmpty() || y1.isEmpty()) {
                        gcodeHere += QString("G90 G00 %1%2\n").arg(x1).arg(y1);
                    }
                    if (inSetToZero) {
                        QString x2 = inKuaiJingX != 0 ? QString("X0") : "";
                        QString y2 = inKuaiJingY != 0 ? QString("Y0") : "";
                        gcodeHere += QString("G92 %1%2\n").arg(x2).arg(y2);
                    }
                    m_ncMachine->RunGCode(gcodeHere);
                }
            }
            catch (...) {

            }
            }, Qt::QueuedConnection);
        });
}

QString JiaoDingWeiForm::GetGCode()
{
    //int inPosType = ui->buttonGroupJiaoWeiZhi->checkedId();

    double inKuaiJingX = ui->inX->text().toDouble();
    double inKuaiJingY = ui->inY->text().toDouble();
    double inKuaiJingZ = ui->inZ->text().toDouble();
    //double inCheDingZhiZhiJing = 0;
    double inJieChuHouFanZhuang = ui->inBack->text().toDouble();
    bool inSetToZero = ui->inSetToZero->isChecked();
    bool inTouchSenseZ = ui->inSetToZTouch->isChecked();

    int dX = 1;
    int dY = 0;
    if (ui->inBtnType1->isChecked())
    {
        dX = 1;
        dY = 0;
    }
    else if (ui->inBtnType2->isChecked())
    {
        dX = 1;
        dY = 1;
    }
    else if (ui->inBtnType3->isChecked())
    {
        dX = 0;
        dY = 0;
    }
    else
    {
        dX = 0;
        dY = 1;
    }
    if (dX == 1)
        inKuaiJingX = -inKuaiJingX;
    if (dY == 1)
        inKuaiJingY = -inKuaiJingY;

    int inCnt = ui->inCnt->text().toInt();
    if (inCnt <= 0 || inCnt > 3) {
        inCnt = 1;
    }
    

    QString gcode2 = GCodeUtils::Jiaodingwei(dX, dY, inKuaiJingX, inKuaiJingY, inKuaiJingZ, inJieChuHouFanZhuang, inSetToZero, inTouchSenseZ);
    //ui->textBrowser->setText(gcode2);

    QString gcode = gcode2;
    for (int i = 0; i < inCnt; ++i) {
        gcode += "o903 call\n";
    }
    gcode += "do_beep 100\n";
    return gcode;
}


