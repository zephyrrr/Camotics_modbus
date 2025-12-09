#pragma once

#include <QWidget>
#include "ui_tool4loadsave.h"
#include "tool4base.h"
#include "BaseChildWindow.h"

class NCMachine;

class Tool4LoadSave : public QToolWidget
{
	Q_OBJECT

public:
	Tool4LoadSave(BaseChildWindow* childWindow, QWidget *parent = nullptr);
	~Tool4LoadSave();

private:
	Ui::Tool4LoadSaveClass ui;

	BaseChildWindow* m_childWindow;
	QString m_path;
private slots:
	void on_btnLoad_clicked();
	void on_btnSave_clicked();
};
