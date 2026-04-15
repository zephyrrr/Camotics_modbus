#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QScrollBar>
#include <QWidget>
#include <QDockWidget>
#include <QPushButton>
#include <cbang/log/Logger.h>
#include <cbang/io/LineBufferStream.h>
#include <cbang/SmartPointer.h>
#include "editor/CAMotics_gcode_editor.h"
#include "forms/manual/weizhiyidongform.h"

QtWin::QtWin(QWidget *parent)
    : ModbusMain(parent), BaseMainWindow(parent)
    , ui(new Ui::QtWin)
{
    ui->setupUi(this);

    //ui->txtLog->setReadOnly(true);
    //QScrollBar* sb = ui->txtLog->verticalScrollBar();
    //sb->setValue(sb->maximum());
    this->ui->verticalLayout->removeWidget(this->ui->txtLog);

    connect(ui->actionOptions, &QAction::triggered, this, &QtWin::openOptions);

    m_console = new CAMotics::ConsoleWriter(this);
    cb::SmartPointer<cb::LineBufferStream<CAMotics::ConsoleWriter>> consoleStream = new cb::LineBufferStream<CAMotics::ConsoleWriter>(*m_console);
    cb::Logger::instance().setScreenStream(consoleStream);
    m_console->setStyleSheet("QTextEdit { background-color: #ffffff; }");
    cb::Logger::instance().setLogCRLF(false);
    cb::Logger::instance().setVerbosity(2);
    //cb::Logger::instance().setLogDebug(true);

    this->ui->verticalLayout->addWidget(m_console);

    LOG_INFO(1, "Log Test");
    //*cb::Logger::instance().createStream("domain", 1, std::string(), 0, 0) << "Sdf";
    //auto line = tr("RZCS");
    //std::string utf8_str(line.toUtf8().constData());
    //LOG_INFO(1, utf8_str);
    //m_console->writeToConsole();

    m_statusbar = new QStatusBar(this);
    setStatusBar(m_statusbar);

    //***************************//
    // copy to QtWin
    m_logTimer = new QTimer(this);
    connect(m_logTimer, SIGNAL(timeout()), this, SLOT(on_logTimer_timeout()));
    m_logTimer->start(1000);

    m_dlgModbusRTU = NULL;
    m_dlgRegWindow = NULL;
    
    //m_regModel = new RegistersModel(this);
    //m_rawModel = new RawDataModel(this);
    //m_rawModel->enableAddLines(true);
    //connect(m_rawModel, SIGNAL(lineAdded(QString)), this, SLOT(logModbusMsg(QString)));

    m_dlgRegWindow = new RegWindow(this, m_ncMachine, m_modbusAdapter, m_modbusCommSettings);

    // reg window
    {
        auto dockWidget = addWidgetAsDock(tr("JCQ"), "Reg Window", m_dlgRegWindow, Qt::TopDockWidgetArea);
        dockWidget->widget()->setMinimumSize(QSize(300, 200));

    
        QtWin4Editor* mainWindow2 = new QtWin4Editor(this, m_console, m_ncMachine);
        this->addToolBar(mainWindow2->findChild<QToolBar*>("mainToolBar"));
        QWidget* centralWidget = mainWindow2->centralWidget();

        auto dockWidget2 = addWidgetAsDock(tr("DM"), "GCode Window", centralWidget, Qt::TopDockWidgetArea);

        tabifyDockWidget(dockWidget, dockWidget2);

        //WeiZhiYiDongForm* form1 = new WeiZhiYiDongForm(this);
        //auto dockWidget3 = addWidgetAsDock(tr("CS"), "Test Window", form1, Qt::TopDockWidgetArea);
        //tabifyDockWidget(dockWidget, dockWidget3);
    }
    {
        QWidget* widget1 = new QWidget();
        m_formNCMachinePosition.setupUi(widget1);

        QWidget* widget2 = new QWidget();
        m_formNCMachineState.setupUi(widget2);

        QVBoxLayout* layout = new QVBoxLayout();
        // Add the two Qt widgets to the layout object.
        layout->addWidget(widget1);
        layout->addWidget(widget2);

        QWidget* parentWidget = new QWidget();
        parentWidget->setLayout(layout);

        auto dockWidget = addWidgetAsDock(tr("YXZT"), "NCMachinePosition", parentWidget, Qt::TopDockWidgetArea);

        //QSize size = dockWidget->sizeHint();
        //dockWidget->setFixedHeight(size.height());
        dockWidget->setMaximumWidth(600);
    }

    modbusConnect(true);
    addNormalTasks();
    connect(m_modbusAdapter, SIGNAL(refreshView()), this, SLOT(UpdateState()));

    //connect(m_ncMachine, SIGNAL(log(int, QString)), this, SLOT(logWithLevel(int, QString)));

    showMaximized();
}

QtWin::~QtWin()
{
    deinit();
}

void QtWin::deinit()
{
    m_logTimer->stop();
    delete m_logTimer;

    if (m_dlgModbusRTU != NULL) {
        delete m_dlgModbusRTU;
    }
    if (m_dlgRegWindow != NULL) {
        delete m_dlgRegWindow;
    }
    
    //if (m_regModel != NULL) {
    //    delete m_regModel;
    //}
    //if (m_rawModel != NULL) {
    //    delete m_rawModel;
    //}
    

    delete m_console;
    delete ui;

    cb::Logger::instance().setScreenStream(std::cout);
}

QDockWidget* QtWin::addWidgetAsDock(QString title, QString name, QWidget* widget, Qt::DockWidgetArea area)
{
    // Create a dock widget object
    QDockWidget* dockWidget = new QDockWidget(title, this);
    dockWidget->setObjectName(name);
    dockWidget->setWidget(widget);

    dockWidget->setAllowedAreas(area);
    dockWidget->setFloating(false);
    dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);

    // Add the dock widget to the main window
    addDockWidget(area, dockWidget);

    return dockWidget;
}

void QtWin::on_logTimer_timeout()
{
    m_console->writeToConsole();
}

void QtWin::openOptions()
{
    if (m_dlgModbusRTU == NULL) {
        m_dlgModbusRTU = new SettingsModbusRTU(this,m_modbusCommSettings);
    }
    m_dlgModbusRTU->modbus_connected = m_modbusAdapter->isConnected();

    m_dlgModbusRTU->show();
    //if (m_dlgModbusRTU->exec()==QDialog::Accepted) {
    //    m_modbusCommSettings->saveSettings();
    //}
}

void QtWin::UpdateState()
{
    bool b = true;
    //bool b = m_ncMachine->ConvertModbusData1();
    if (b) {
        //auto pos = m_ncMachine->GetXYZUWithOffset();
        auto state = m_ncMachine->GetState();
        /*QString line = QString("%1, %2, %3, %4. %5, %6").arg(
            QString::fromWCharArray(std::to_wstring(pos.x()).c_str()),
            QString::fromWCharArray(std::to_wstring(pos.y()).c_str()),
            QString::fromWCharArray(std::to_wstring(pos.z()).c_str()),
            QString::fromWCharArray(std::to_wstring(pos.data[3]).c_str()),
            QString::fromWCharArray(NCMachine::GetStateDesc(state[2]).c_str()),
            QString::fromWCharArray(NCMachine::GetRLSTDesc(state[0], state[1]).c_str()));
        log(line);*/
        //auto sss = std::to_wstring(pos.x());
        //QString xxx = QString::fromWCharArray(sss.c_str());

        auto x = m_ncMachine->GetController()->getAxisPosition('X');
        auto y = m_ncMachine->GetController()->getAxisPosition('Y');
        auto z = m_ncMachine->GetController()->getAxisPosition('Z');
        auto u = m_ncMachine->GetController()->getAxisPosition('U');

        m_formNCMachinePosition.xLabel->setText(GCodeTool::FormatPos(x));
        m_formNCMachinePosition.yLabel->setText(GCodeTool::FormatPos(y));
        m_formNCMachinePosition.zLabel->setText(GCodeTool::FormatPos(z));
        m_formNCMachinePosition.uLabel->setText(GCodeTool::FormatPos(u));

        m_formNCMachineState.xLabel->setText(NCMachine::GetStateDesc(state[2]));
        m_formNCMachineState.yLabel->setText(NCMachine::GetRLSTDesc(state[0], state[1]));
        m_formNCMachineState.msgLabel->setText(m_ncMachine->GetDebugMsg());
    }
    m_console->writeToConsole();

    QString msg = QString("Time: %1 / %2, %3%. Packet: %4 / %5")
        .arg(m_modbusAdapter->getCommMSec()).arg(m_modbusAdapter->getAllMSec()).arg(m_modbusAdapter->getCommMSec() * 100 / (m_modbusAdapter->getAllMSec() + 1))
        .arg(m_modbusAdapter->getErrorsCount()).arg(m_modbusAdapter->getPacketsCount());
    m_statusbar->showMessage(msg);
}

void QtWin::logModbusMsg(QString line)
{
    //logWithLevel(1, line);
    /*const std::string s = line.tos();
    m_console.append(s);*/

    //std::string utf8_str(line.toUtf8().constData());
    //m_console->append(utf8_str);
    //m_console->writeToConsole();
    LOG_INFO(10, EUtils::QString2StdString(line));
}

//void QtWin::logWithLevel(int level, QString line)
//{
//    if (level > 1) {
//        line = tr("【JG】") + line;
//    }
//    ui->txtLog->appendPlainText(line);
//    //ui->txtLog->appendPlainText(RegWindow::parseMsg(line));
//}