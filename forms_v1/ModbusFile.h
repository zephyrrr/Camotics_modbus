#pragma once

#include <QObject>
#include <QDialog>
#include "../modbus/modbusadapter.h"

namespace Ui {
	class ModbusFile;
}

class ModbusFile  : public QDialog
{
	Q_OBJECT

public:
	ModbusFile(QWidget* parent, ModbusAdapter* adapter);
	~ModbusFile();

private:
	Ui::ModbusFile* ui;

	ModbusAdapter* m_modbusAdapter;

private slots:
	void on_btnRead_clicked();
	void on_btnWrite_clicked();
};
