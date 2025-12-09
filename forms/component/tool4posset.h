#pragma once

#include <QWidget>
#include "ui_tool4posset.h"
#include "tool4base.h"
#include "BaseChildWindow.h"

class NCMachine;

class Tool4PosSet : public QToolWidget
{
	Q_OBJECT

public:
	Tool4PosSet(NCMachine* ncMachine, QWidget *parent = nullptr);
	~Tool4PosSet();

private:
	Ui::Tool4PosSetClass ui;

	NCMachine* ncMachine;
	int m_currentPosIdx = 1;

private slots:
	void on_btnRecord_clicked();
	void on_btnBatchSet_clicked();
	void on_btnCurrentPosIdx_clicked();
};
