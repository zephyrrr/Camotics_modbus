#include "duanmianform.h"
#include "ui_duanmianform.h"
#include <QLabel>
#include <QButtonGroup>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include "zhuzhongxingform.h"
#include "../modbus/NCMachine.h"
#include "utils/gcodeutils.h"
#include "utils/dataform.h"

DuanMianForm::DuanMianForm(QWidget *parent) :
    BaseChildWindowWithTools(parent),
    ui(new Ui::DuanMianForm)
{
    ui->setupUi(this);

    

    ZhuZhongXingForm::setButton4ButtonEdit(ui->btnX);
    ZhuZhongXingForm::setButton4ButtonEdit(ui->btnY);
    ZhuZhongXingForm::setButton4ButtonEdit(ui->btnZ);
    if (LOC_AXIS_LEN >= 4) {
        ZhuZhongXingForm::setButton4ButtonEdit(ui->btnU);
    }
    else {
        ui->btnU->setEnabled(false);
    }

    m_toClearInputWidgets.append(QList<QWidget*>() << ui->btnX_P << ui->btnX_N << ui->btnY_P << ui->btnY_N << ui->btnZ_P << ui->btnZ_N << ui->btnU_P << ui->btnU_N);
    m_inputBtns = QList<QPushButton*>() << ui->btnX_P << ui->btnX_N << ui->btnY_P << ui->btnY_N << ui->btnZ_P << ui->btnZ_N << ui->btnU_P << ui->btnU_N;
    for (QPushButton* btn : m_inputBtns) {
        connect(btn, &QPushButton::clicked, [this, btn]() {
            for (QPushButton* btn2 : m_inputBtns)
            {
                if (btn != btn2)
                    btn2->setChecked(false);
            }
            });
    }
}

DuanMianForm::~DuanMianForm()
{
    delete ui;
}

void DuanMianForm::on_btnX_clicked()
{
    //ZhuZhongXingForm::toggleLineEdit(ui->inX);
    ZhuZhongXingForm::toggleButtonEdit(ui->btnX->isChecked(), ui->btnX_P, ui->btnX_N);
}
void DuanMianForm::on_btnY_clicked()
{
    ZhuZhongXingForm::toggleButtonEdit(ui->btnY->isChecked(), ui->btnY_P, ui->btnY_N);
}
void DuanMianForm::on_btnZ_clicked()
{
    ZhuZhongXingForm::toggleButtonEdit(ui->btnZ->isChecked(), ui->btnZ_P, ui->btnZ_N);
}
void DuanMianForm::on_btnU_clicked()
{
    ZhuZhongXingForm::toggleButtonEdit(ui->btnU->isChecked(), ui->btnU_P, ui->btnU_N);
}

void DuanMianForm::RunGCode()
{
    bool inSetToZero = ui->inSetToZero->isChecked();
    //double inX = ui->inX->text().toDouble();
    //double inY = ui->inY->text().toDouble();
    //double inZ = ui->inZ->text().toDouble();
    //double inU = ui->inU->text().toDouble();
    double inX = ui->btnX_P->isChecked() ? 1 : (ui->btnX_N->isChecked() ? -1 : 0);
    double inY = ui->btnY_P->isChecked() ? 1 : (ui->btnY_N->isChecked() ? -1 : 0);
    double inZ = ui->btnZ_P->isChecked() ? 1 : (ui->btnZ_N->isChecked() ? -1 : 0);
    double inU = ui->btnU_P->isChecked() ? 1 : (ui->btnU_N->isChecked() ? -1 : 0);
    double inJieChuHouFanZhuang = ui->inBack->text().toDouble();

    QString gcode = GCodeUtils::Duanmian(inX, inY, inZ, inU, inJieChuHouFanZhuang, inSetToZero);
    //ui->lblMessage->setText(gcode);

    if (!gcode.isEmpty()) {
        gcode += "do_beep 100\n";

        m_ncMachine->RunGCode(gcode);
    }
    //ui->textBrowser->setText(gcode);
    ClearInputWidgets();
}


