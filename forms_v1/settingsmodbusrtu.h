#ifndef SETTINGSMODBUSRTU_H
#define SETTINGSMODBUSRTU_H

#include <QDialog>
#include <QSettings>
#include "BaseChildWindow.h"
#include "../modbus/modbuscommsettings.h"

namespace Ui {
    class SettingsModbusRTU;
}

class SettingsModbusRTU : public BaseChildWindow
{
    Q_OBJECT;

public:
    explicit SettingsModbusRTU(QWidget *parent = 0 ,ModbusCommSettings * settings = 0);
    ~SettingsModbusRTU();
    bool modbus_connected;

    void RunGCode() override {}
private:
    Ui::SettingsModbusRTU *ui;
    ModbusCommSettings * m_settings;

private slots:
    void changesAccepted();

//Q_SIGNALS:
//    void accepted();
//    void rejected();
//
//public Q_SLOTS:
//    virtual void accept();
//    virtual void reject();

protected:
    void showEvent(QShowEvent * event);

};

#endif // SETTINGSMODBUSRTU_H
