#ifndef BASEMAINWINDOW_H
#define BASEMAINWINDOW_H

#include <QMainWindow>
#include "modbus/modbuscommsettings.h"
#include "modbus/modbusadapter.h"
#include "BaseChildWindow.h"

class NCMachine;

class BaseMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    BaseMainWindow(QWidget* parent);
    ~BaseMainWindow();

public:
    void modbusConnect(bool connect);
    virtual void addNormalTasks();
    static void modbusConnect(bool connect, ModbusAdapter* modbus, ModbusCommSettings* modbusCommSettings, NCMachine* ncMachine);

    virtual BaseChildWindow* getChildWindow(QString windowTitle) { return NULL; }
    void showChildWindow(QString windowTitle);
protected:
    ModbusCommSettings* m_modbusCommSettings;
    ModbusAdapter* m_modbus;
    NCMachine* m_ncMachine;

    RawDataModel* m_rawModel = NULL;
    RegistersModel* m_regModel = NULL;
};

#endif

