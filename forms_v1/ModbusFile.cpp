#include "ModbusFile.h"
#include "ui_ModbusFile.h"
#include "../modbus/libmodbus/modbus.h"
#include <QMessageBox>
#include <cbang/log/Logger.h>

ModbusFile::ModbusFile(QWidget* parent, ModbusAdapter* adapter)
	: QDialog(parent), ui(new Ui::ModbusFile), m_modbus(adapter)
{
	ui->setupUi(this);
}

ModbusFile::~ModbusFile()
{}

void ModbusFile::on_btnRead_clicked()
{
	int addr = ui->editFileNum->text().toInt();
	int sub_addr = ui->editRecord->text().toInt();
	int nb = ui->editNum->text().toInt();
	uint16_t* dest = new uint16_t[nb];
	int ret = modbus_read_file_record(m_modbus->GetRawInterface(), addr, sub_addr, nb, dest);

	if (ret == nb) {
		char* hexString = new char[nb * 5 + 1];
		//hexString = { 0 };
		for (int i = 0; i < nb; i++) {
			sprintf(&hexString[i * 5], "%04X ", dest[i]);

		}
		ui->editData->setText(QString(hexString));
		delete[] hexString;
	}
	else {
		ui->editData->setText("");
	}

	delete[] dest;

	if (ret == -1) {
		LOG_WARNING("Modbus: read failed: " << addr << ", " << sub_addr << ", " << EUtils::QString2StdString(EUtils::libmodbus_strerror(errno)));
	}
}

void ModbusFile::on_btnWrite_clicked()
{
	int addr = ui->editFileNum->text().toInt();
	int sub_addr = ui->editRecord->text().toInt();
	int nb = ui->editNum->text().toInt();
	uint16_t* src = new uint16_t[nb];

	QString hexString = ui->editData->text();
	hexString = hexString.remove(QRegularExpression("\\s+"));
	bool ok;
	if (4 * nb != hexString.length())
	{
		QMessageBox msgBox;
		msgBox.setText(tr("SJGSBF"));
		msgBox.setWindowTitle("Error");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.exec();
		return;
	}
	for (int i = 0; i < hexString.length(); i += 4) {
		// Convert each pair of hex digits to a uint16_t value
		uint16_t value = hexString.mid(i, 4).toUInt(&ok, 16);
		src[i / 4] = value;
	}

	int ret = modbus_write_file_record(m_modbus->GetRawInterface(), addr, sub_addr, nb, src);
	delete[] src;
	if (ret == -1) {
		LOG_ERROR("Modbus: write failed: " << addr << ", " << sub_addr << ", " << EUtils::QString2StdString(EUtils::libmodbus_strerror(errno)));
	}
}
