#pragma once

#include <QObject>
#include <QDialog>
#include "BaseChildWindow.h"
#include "../modbus/modbusadapter.h"
#include "../modbus/modbuscommsettings.h"
#include "../modbus/NCMachine.h"

namespace Ui {
	class NCMachinePanel;
}

class NCMachinePanel : public BaseChildWindow
{
	Q_OBJECT

public:
	NCMachinePanel(QWidget* parent, NCMachine* machine, ModbusAdapter* adapter, ModbusCommSettings* settings);
	~NCMachinePanel();

	void RunGCode() override {}
private:
	Ui::NCMachinePanel* ui;

	ModbusCommSettings* m_modbusCommSettings;
	ModbusAdapter* m_modbus;
	NCMachine* m_ncMachine;

private slots:
	void on_btnSendKaiji_clicked();
	void on_btnSendGuanji_clicked();
	void on_btnSendBackToZero1_clicked();

	void on_btnSendBackToZero3_clicked();

	void on_btnSendBackToZero5_clicked();


	void on_btnDebug1_clicked();
	void on_btnDebug2_clicked();
};
