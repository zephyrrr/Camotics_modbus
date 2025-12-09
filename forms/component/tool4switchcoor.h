#pragma once

#include <QWidget>
#include "ui_tool4switchcoor.h"
#include "tool4base.h"

class NCMachine;

class Tool4SwitchCoor : public QToolWidget
{
	Q_OBJECT

public:
	Tool4SwitchCoor(NCMachine* ncMachine, QWidget *parent = nullptr);
	~Tool4SwitchCoor();

	void UpdateState() override;
	static QString GetCoorName(int csIndex);
private:
	Ui::Tool4SwitchCoorClass ui;

	NCMachine* m_ncMachine;

	int m_currentCoor = 54;

private slots:
	void on_btnSwitchCoorLeft_clicked();
	void on_btnSwitchCoorRight_clicked();
};
