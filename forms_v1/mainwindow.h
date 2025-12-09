#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "settingsmodbusrtu.h"
#include "regwindow.h"
#include "../modbus/registersmodel.h"
#include "../modbus/rawdatamodel.h"
#include "ui_NCMachinePosition.h"
#include "ui_NCMachineState.h"

#include "camotics/qt/ConsoleWriter.h"
#include "BaseMainWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class QtWin; }
QT_END_NAMESPACE

class QtWin : public BaseMainWindow
{
    Q_OBJECT;

public:
    QtWin(QWidget *parent = nullptr);
    ~QtWin();

private:
    Ui::QtWin* ui;

//***************************//
    // copy to QtWin
private:
    void deinit();

    QDockWidget* addWidgetAsDock(QString title, QString name, QWidget* widget, Qt::DockWidgetArea area);

private:
    CAMotics::ConsoleWriter* m_console;

    QTimer* m_logTimer;

    SettingsModbusRTU *m_dlgModbusRTU;
    RegWindow *m_dlgRegWindow;

    
    //RegistersModel* m_regModel;
    //RawDataModel* m_rawModel;

    Ui::FormNCMachinePosition m_formNCMachinePosition;
    Ui::FormNCMachineState m_formNCMachineState;

    QStatusBar* m_statusbar;
private slots:
    void on_logTimer_timeout();

    void openOptions();
    void UpdateState();
    void logModbusMsg(QString line);
    
    //void logWithLevel(int level, QString line);
};
#endif // MAINWINDOW_H
