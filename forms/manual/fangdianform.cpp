#include "fangdianform.h"
#include "ui_fangdianform.h"
#include <QLabel>
#include <QButtonGroup>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include "zhuzhongxingform.h"
#include "../modbus/NCMachine.h"
#include "../other/fangdiancheshiform.h"
#include "../BaseMainWindow.h"

FangDianForm::FangDianForm(QWidget *parent) :
    BaseChildWindowWithTools(parent),
    ui(new Ui::FangDianForm)
{
    ui->setupUi(this);

    ZhuZhongXingForm::setButton4LineEdit(ui->btnX);
    ZhuZhongXingForm::setButton4LineEdit(ui->btnY);
    ZhuZhongXingForm::setButton4LineEdit(ui->btnZ);
    if (SPK_AXIS_LEN >= 4) {
		ZhuZhongXingForm::setButton4LineEdit(ui->btnU);
    }
    else {
        ui->btnU->setEnabled(false);
    }
    //m_toClearInputWidgets.append(QList<QWidget*>() << ui->btnX_3_P << ui->btnX_3_N << ui->btnY_3_P << ui->btnY_3_N << ui->btnZ_3_P << ui->btnZ_3_N << ui->btnU_3_P << ui->btnU_3_N);
    
    m_inputBtns = QList<QPushButton*>() << ui->btnX_3_P << ui->btnX_3_N << ui->btnY_3_P << ui->btnY_3_N << ui->btnZ_3_P << ui->btnZ_3_N << ui->btnU_3_P << ui->btnU_3_N;
    for (QPushButton* btn : m_inputBtns) {
        connect(btn, &QPushButton::clicked, [this, btn]() {
            for (QPushButton* btn2 : m_inputBtns)
            {
                if (btn != btn2)
                    btn2->setChecked(false);
            }
            if (btn == ui->btnX_3_P || btn == ui->btnX_3_N) {
                QLineEdit* x = dynamic_cast<QLineEdit*>(m_fangdiancheshiForm->GetDynamicForm(1)->getLineEdit("LN"));
                QRegExp regExp("^[0-4][0-2]2$");
                x->setValidator(new QRegExpValidator(regExp, x));
                if (x->text().length() == 3) {
					x->setText(x->text().left(2) + "2");
                }
            }
            else if (btn == ui->btnY_3_P || btn == ui->btnY_3_N) {
                QLineEdit* x = dynamic_cast<QLineEdit*>(m_fangdiancheshiForm->GetDynamicForm(1)->getLineEdit("LN"));
                QRegExp regExp("^[0-4][0-2]1$");
                x->setValidator(new QRegExpValidator(regExp, x));
                if (x->text().length() == 3) {
                    x->setText(x->text().left(2) + "1");
                }
            }
            else if (btn == ui->btnZ_3_P || btn == ui->btnZ_3_N) {
                QLineEdit* x = dynamic_cast<QLineEdit*>(m_fangdiancheshiForm->GetDynamicForm(1)->getLineEdit("LN"));
                QRegExp regExp("^[0-4][0-2]0$");
                x->setValidator(new QRegExpValidator(regExp, x));
                if (x->text().length() == 3) {
                    x->setText(x->text().left(2) + "0");
                }
            }
            });
    }

    m_fangdiancheshiForm = new FangDianCanShuSingleForm(this);
    m_fangdiancheshiForm->setObjectName("FangDianForm_fangdiancheshiForm");
    m_fangdiancheshiForm->GetDynamicForm()->setMargin(0);
    m_fangdiancheshiForm->GetDynamicForm()->setSpacing(0);
    m_fangdiancheshiForm->GetDynamicForm()->setContentsMargins(0, 0, 0, 0);
    m_fangdiancheshiForm->GetDynamicForm()->setHorizontalSpacing(0);
    m_fangdiancheshiForm->GetDynamicForm()->setVerticalSpacing(0);
    m_fangdiancheshiForm->setContentsMargins(0, 0, 0, 0);
    m_fangdiancheshiForm->GetDynamicForm(0)->hideItem("C.No");
    m_fangdiancheshiForm->GetDynamicForm()->hideItem("DS");
    ui->inJgtjDetail->insertWidget(0, m_fangdiancheshiForm);
    m_fangdiancheshiForm->setMaximumWidth(820);
    m_fangdiancheshiForm->setMinimumWidth(820);

    ui->inJgtj->setLen(3);
    ui->inJgtj->setInputMask("000");
    ui->inJgtj->setValidator(NULL);
    //connect(ui->inJgtj, &QLineEdit::textEdited, this, &FangDianForm::on_inJgtj_textEdited);

    //ui->inHtl->setValidator(new QIntValidator(1, 100000, ui->inHtl));
    //ui->inHtl->setAllowNull(true);

    //QList<QLineEdit*> inputEdits = QList<QLineEdit*>() << ui->inX << ui->inY << ui->inZ << ui->inU;
    //for (QLineEdit* edit : inputEdits) {
    //    connect(edit, &QLineEdit::textEdited, [this, edit, inputEdits]() {
    //        for (QLineEdit* edit2 : inputEdits)
    //        {
    //            if (edit != edit2)
    //                edit2->clear();
    //        }
    //        });
    //}
}

FangDianForm::~FangDianForm()
{
    delete ui;
}
void FangDianForm::on_inJgtj_textChanged(const QString& text)
{
    if (text.length() == 3) {
        m_fangdiancheshiForm->LoadFromParametersC(text.toInt());
    }
}

void FangDianForm::on_btnX_clicked()
{
    //ZhuZhongXingForm::toggleLineEdit(!ui->btnX->isChecked(), ui->inX);
    bool b = ui->btnX->isChecked();
    ZhuZhongXingForm::toggleButtonEdit(b, ui->btnX_3_P, ui->btnX_3_N);
}
void FangDianForm::on_btnY_clicked()
{
    //ZhuZhongXingForm::toggleLineEdit(!ui->btnY->isChecked(), ui->inY);
    bool b = ui->btnY->isChecked();
    ZhuZhongXingForm::toggleButtonEdit(b, ui->btnY_3_P, ui->btnY_3_N);
}
void FangDianForm::on_btnZ_clicked()
{
    //ZhuZhongXingForm::toggleLineEdit(!ui->btnZ->isChecked(), ui->inZ);
    bool b = ui->btnZ->isChecked();
    ZhuZhongXingForm::toggleButtonEdit(b, ui->btnZ_3_P, ui->btnZ_3_N);
}
void FangDianForm::on_btnU_clicked()
{
    //ZhuZhongXingForm::toggleLineEdit(!ui->btnU->isChecked(), ui->inU);
    bool b = ui->btnU->isChecked();
    ZhuZhongXingForm::toggleButtonEdit(b, ui->btnU_3_P, ui->btnU_3_N);
}

void FangDianForm::RunGCode()
{
    //double inX = ui->inX->text().toDouble();
    //double inY = ui->inY->text().toDouble();
    //double inZ = ui->inZ->text().toDouble();
    //double inU = ui->inU->text().toDouble();
    double inX = ui->btnX_3_P->isChecked() ? 999 : (ui->btnX_3_N->isChecked() ? -999 : 0);
    double inY = ui->btnY_3_P->isChecked() ? 999 : (ui->btnY_3_N->isChecked() ? -999 : 0);
    double inZ = ui->btnZ_3_P->isChecked() ? 999 : (ui->btnZ_3_N->isChecked() ? -999 : 0);
    double inU = ui->btnU_3_P->isChecked() ? 999 : (ui->btnU_3_N->isChecked() ? -999 : 0);
    QString inJgtj = ui->inJgtj->text();

	int lastJogSpeed = PropertyObjects::getInstance()->propertyObjectShouKongHe->sdxskz();
    QString gcode = "do_jogspeed 0\n";
    gcode += m_fangdiancheshiForm->GetValuesAsCCode();
    gcode += QString("C%01 G01 ").arg(inJgtj);
        
    bool inputValidate = false;
    
    if (inX != 0) {
        gcode += QString("%01%02").arg("X").arg(inX, 0, 'f', 3);
        inputValidate = true;
    }
    if (inY != 0) {
        gcode += QString("%01%02").arg("Y").arg(inY, 0, 'f', 3);
        inputValidate = true;
    }
    if (inZ != 0) {
        gcode += QString("%01%02").arg("Z").arg(inZ, 0, 'f', 3);
        inputValidate = true;
    }
    if (inU != 0) {
        gcode += QString("%01%02").arg("U").arg(inU, 0, 'f', 3);
        inputValidate = true;
    }
    if (inJgtj.isEmpty()) {
        inputValidate = false;
    }

    if (inputValidate) {
        SystemSettings::instance().SetValue("FDDWHTJL", QString::number(int(ui->inHtl->text().toDouble() * 1000)));

        //if (!ui->inHtl->text().isEmpty()) {
        //    PropertyObjects::getInstance()->propertyObjectYd->settcqhtjl(ui->inHtl->text().toInt());
        //}

        //gcode += QString("\ndo_jogspeed %01").arg(lastJogSpeed);
        BaseMainWindow* mainWindow = BaseChildWindow::GetMainWindow();
        QPushButton* btn1 = mainWindow->findChild<QPushButton*>("btnHuoYan");
        QPushButton* btn2 = mainWindow->findChild<QPushButton*>("btnYouWen");
        QPushButton* btn3 = mainWindow->findChild<QPushButton*>("btnYouWei");
        bool stateBtn1 = btn1->isChecked();
        bool stateBtn2 = btn2->isChecked();
        bool stateBtn3 = btn3->isChecked();

		btn1->setChecked(false);
		btn2->setChecked(false);
		btn3->setChecked(false);

        m_ncMachine->RunGCode(gcode, [this, lastJogSpeed, btn1, btn2, btn3, stateBtn1, stateBtn2, stateBtn3]() {
            m_ncMachine->enterSetPriority(0);
            m_ncMachine->SetSpeedGear(lastJogSpeed);
            m_ncMachine->exitSetPriority();

            QMetaObject::invokeMethod(this, [this, btn1, btn2, btn3, stateBtn1, stateBtn2, stateBtn3]() {
                btn1->setChecked(stateBtn1);
				btn2->setChecked(stateBtn2);
				btn3->setChecked(stateBtn3);
                }, Qt::QueuedConnection);
        });
       // ui->textBrowser->setText(gcode);
    }

    ClearInputWidgets();
}


