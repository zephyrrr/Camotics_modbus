#pragma once

#include <QWidget>
#include "ui_tool4gcode.h"
#include "tool4base.h"
#include "BaseChildWindow.h"

class NCMachine;

class Tool4GCode : public QToolWidget
{
	Q_OBJECT

public:
	Tool4GCode(BaseChildWindow* childWindow, QWidget *parent = nullptr);
	~Tool4GCode();

private:
	Ui::Tool4GCodeClass ui;

	BaseChildWindow* m_childWindow;
	QString m_path;
private slots:
	void on_btnExport_clicked();
};
