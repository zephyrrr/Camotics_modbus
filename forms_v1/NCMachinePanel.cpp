#include "NCMachinePanel.h"
#include "ui_NCMachinePanel.h"
#include "regwindow.h"

NCMachinePanel::NCMachinePanel(QWidget *parent, NCMachine* ncMachine, ModbusAdapter* adapter)
	: BaseChildWindow(parent), m_modbusAdapter(adapter), ui(new Ui::NCMachinePanel)
{
    m_ncMachine = ncMachine;
	ui->setupUi(this);
}

NCMachinePanel::~NCMachinePanel()
{}

void NCMachinePanel::on_btnSendKaiji_clicked()
{
    //on_btnStart_clicked();
    m_ncMachine->PowerOn();
}

void NCMachinePanel::on_btnSendGuanji_clicked()
{
    m_ncMachine->PowerOff();
    //on_btnStop_clicked();
}

void NCMachinePanel::on_btnSendBackToZero1_clicked()
{
    RegWindow::MessageBoxYesNo(tr("QSY SZ-GL"));
}

void NCMachinePanel::on_btnSendBackToZero3_clicked()
{
    RegWindow::MessageBoxYesNo(tr("QSY SZ-GL"));
}

void NCMachinePanel::on_btnSendBackToZero5_clicked()
{
    RegWindow::MessageBoxYesNo(tr("QSY SZ-GL"));
}

void NCMachinePanel::on_btnDebug1_clicked()
{
    m_ncMachine->SetState4Debug(ui->lineEditState1->text().toShort(),
        ui->lineEditState2->text().toShort(),
        ui->lineEditState3->text().toShort());
    ui->lineEditState1->setText("0");
    ui->lineEditState2->setText("0");
    ui->lineEditState3->setText("0");
}


void NCMachinePanel::on_btnDebug2_clicked()
{
    //m_ncMachine->SetNctState4Debug(0);
}


