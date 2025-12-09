#include "kongzhongxingform.h"
#include "ui_kongzhongxingform.h"
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

KongZhongXingForm::KongZhongXingForm(QWidget *parent) :
    BaseChildWindowWithTools(parent, TOOL_ZERO_AXIS | TOOL_SWITCH_COOR | TOOL_COOR_ITER),
    ui(new Ui::KongZhongXingForm)
{
    ui->setupUi(this);

    //ui->formLayout->removeRow(4);
    //ui->inSetToZTouch->parentWidget()->setVisible(false);
    //ui->formLayout->labelForField(ui->inSetToZTouch->layout())->setVisible(false);

    ZhuZhongXingForm::SetupTable4Result(ui->tableView);

    ZhuZhongXingForm::setButton4LineEdit(ui->btnX);
    ZhuZhongXingForm::setButton4LineEdit(ui->btnY);
    ZhuZhongXingForm::setButton4LineEdit(ui->btnZ);

    QRegExp rx("[123]");
    QRegExpValidator* validator = new QRegExpValidator(rx, this);
    ui->inCnt->setValidator(validator);
}

KongZhongXingForm::~KongZhongXingForm()
{
    delete ui;
}

void KongZhongXingForm::on_btnX_clicked()
{
    ZhuZhongXingForm::toggleLineEdit(!ui->btnX->isChecked(), ui->inX);
}
void KongZhongXingForm::on_btnY_clicked()
{
    ZhuZhongXingForm::toggleLineEdit(!ui->btnY->isChecked(), ui->inY);
}
void KongZhongXingForm::on_btnZ_clicked()
{
    ZhuZhongXingForm::toggleLineEdit(!ui->btnZ->isChecked(), ui->inZ);
}

void KongZhongXingForm::RunGCode()
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

QString KongZhongXingForm::GetGCode()
{
    double inKuaiJingX = ui->inX->text().toDouble();
    double inKuaiJingY = ui->inY->text().toDouble();
    double inKuaiJingZ = ui->inZ->text().toDouble();
    //double inCheDingZhiZhiJing = 0;
    double inJieChuHouFanZhuang = ui->inBack->text().toDouble();
    bool inSetToZero = ui->inSetToZero->isChecked();
    bool inTouchSenseZ = false;  // ui->inSetToZTouch->isChecked();
    int inCnt = ui->inCnt->text().toInt();
    if (inCnt <= 0 || inCnt > 3) {
        inCnt = 1;
    }
    
    QString gcode2 = GCodeUtils::Kongzhongxing(inKuaiJingX, inKuaiJingY, inKuaiJingZ, inJieChuHouFanZhuang, inSetToZero, inTouchSenseZ);
    //ui->textBrowser->setText(gcode2);

    QString gcode = gcode2;
    for (int i = 0; i < inCnt; ++i) {
        gcode += "o902 call\n";
    }
    gcode += "do_beep 100\n";
    return gcode;
}


