#pragma once

#include "../modbus/NCMachine.h"
#include <QtWidgets/QMainWindow>
#include <QBoxLayout>
#include "camotics/qt/ConsoleWriter.h"
#include "camotics/qt/NCEdit.h"
#include "ui_CAMotics_gcode_editor.h"


class QtWin4Editor : public QMainWindow
{
    Q_OBJECT

public:
    QtWin4Editor(QWidget *parent = nullptr, CAMotics::ConsoleWriter* console = nullptr, NCMachine* machine = nullptr);
    ~QtWin4Editor();

    void createNCEdit(QVBoxLayout* layout);
    void load(QString filename);
private:
    Ui::CAMotics_gcode_editorClass ui;
    CAMotics::ConsoleWriter* m_console;
    CAMotics::NCEdit* editor;
    NCMachine* m_machine;

    void save(QString filename);

    void RunCmd(WCHAR commandLine[MAX_PATH], bool showWindow = false);
    void LogToConsole();
private slots:
    void on_actionLoad_triggered();
    void on_actionSave_triggered();
    void on_actionParse_triggered();
    void on_actionSimu_triggered();
    void on_actionRun_triggered();
};
