#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include <QMainWindow>
#include <QHash>
#include <QMenu>
//#include <QToolBox>
#include "BaseMainWindow.h"
#include "BaseChildWindow.h"
#include "utils/LineLogger.h"
#include "../forms_v1/regwindow.h"
#include "component/displayloran.h"
#include "component/displayz.h"

class MdiForm;

QT_BEGIN_NAMESPACE
namespace Ui { class QtWin2; }
QT_END_NAMESPACE

class Gaotiao4Jiagong;
class FangDianCanShuSingleForm;

class ModbusMain
{
public:
    ModbusMain(QObject* parent);
    ~ModbusMain();

public:
    void modbusConnect(bool connect);
    virtual void addNormalTasks();
    static void modbusConnect(bool connect, ModbusAdapter* modbus, ModbusCommSettings* modbusCommSettings, NCMachine* ncMachine);

protected:
    ModbusCommSettings* m_modbusCommSettings;
    ModbusAdapter* m_modbusAdapter;
    NCMachine* m_ncMachine;

    RawDataModel* m_rawModel = NULL;
    RegistersModel* m_regModel = NULL;

private:
    QObject* m_parent;
};


class QtWin2 : public BaseMainWindow, public ModbusMain
{
    Q_OBJECT

public:
    QtWin2(QWidget *parent = nullptr);
    ~QtWin2();

    bool serialize();
    bool deserialize();

    static void hideTaskbar();
    static void showTaskbar();
    void showWindowMode(bool isRelease);

    BaseChildWindow* getChildWindow(QString windowTitle) override;
    void addNormalTasks() override;
    void showChildWindow(QString windowTitle) override;
protected:
    void showEvent(QShowEvent* event) override;

private slots:
    void UpdateStateByTimer();
    void UpdateState();
    void on_actionRun_triggered();
    void on_actionCancel_triggered();
    void on_actionDebug_triggered();
    void on_actionTouchSense_triggered();
    void on_actionTouchSense2_triggered();
    void on_actionLoadStylesheet_triggered();
    void on_actionPythonScript_triggered();
    void on_actionPowerOn_triggered();
    void on_actionPowerOff_triggered();
    void on_actionSaveScreen_triggered();
    void on_actionSaveGCode_triggered();
    //void on_btnErrorDetail_clicked();

    void onNavigatorButtonClicked();
    
    void on_btnJgtj_clicked();
    void on_btnJgtjGt_clicked();
    
private:
    void switchUserMode();
    static QString SecondsToStr(qint64 totalSeconds);
    void OpenRegWindow();

    int m_currentChildWindowCnt = 0;
    void AddChildWindow(QString title, QBoxLayout* layout, BaseChildWindow* child);
    void AddChildWindow(QString title, QBoxLayout* layout, std::function<BaseChildWindow*()> func, bool insertAtBegin=false);

    void LoadPluginsAsChildWindow();

protected:
	void keyPressEvent(QKeyEvent* event) override;

private:
    Ui::QtWin2 *ui;
    QWidget* mainMenu;
    QLineEdit* lineEdit4TouchSense2;
    QButtonGroup* m_buttonGroup4Menu;
    QHash<QAbstractButton*, int> m_buttonIdxs;
    QHash<QAbstractButton*, std::function<BaseChildWindow*()>> m_buttonForms;
    QHash<QString, QAbstractButton*> m_buttonsText;
    QList<QString> m_historyMenus;
    RegWindow* m_dlgRegWindow = NULL;
    QList<QBoxLayout*> m_layouts;
    DisplayLoran* m_displayLoran;
    DisplayZ* m_displayZ;
    int m_displayIdx = 0;
#ifdef _DEBUG
    bool m_isUserMode = true;
#else
	bool m_isUserMode = true;
#endif

    QScrollArea* scrollArea;
    QMenu m_g54menu;

	Gaotiao4Jiagong* m_formGaotiao4Jiagong = NULL;
    QWidget* m_dialog4Gaotiao = NULL;
    FangDianCanShuSingleForm* m_formFangDianCheShi = NULL;
	QWidget* m_dialog4FangDianCheShi = NULL;

	QList<QPushButton*> m_topinfobarButtons;
    ulong m_msecUI = 0;
    ulong m_cntUI = 0;

    bool m_lastInputFlagError = false;

    QTimer* m_pollTimer;
    int m_pollTimerCounter = 0;
    bool m_serialPortNeedReconnect = false;
};

class QtWin3 : public BaseMainWindow, public ModbusMain
{
	Q_OBJECT

public:
    QtWin3(QWidget* parent = nullptr);
    ~QtWin3();

private slots:
    void UpdateState();

private:
	MdiForm* mdiForm = nullptr;
    QStatusBar* statusBar;
};
#endif // MAINWINDOW_H
