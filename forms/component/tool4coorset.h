#pragma once

#include <QWidget>
#include "ui_tool4coorset.h"
#include "tool4base.h"
#include "BaseChildWindow.h"

class NCMachine;

class Tool4CoorSet : public QToolWidget
{
	Q_OBJECT

public:
	Tool4CoorSet(NCMachine* ncMachine, QWidget *parent = nullptr);
	~Tool4CoorSet();

private:
	Ui::Tool4CoorSetClass ui;

	NCMachine* ncMachine;
	int m_currentPosIdx = 1;

private slots:
	void on_btnRecord_clicked();
	void on_btnBatchSet_clicked();
};
