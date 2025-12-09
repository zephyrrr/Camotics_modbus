#pragma once

#include <QWidget>
#include "ui_tool4zeroaxis.h"
#include "tool4base.h"

class NCMachine;

class Tool4ZeroAxis : public QToolWidget
{
	Q_OBJECT

public:
	Tool4ZeroAxis(NCMachine* ncMachine, QWidget *parent = nullptr, QList<char> axisNames = QList<char>());
	~Tool4ZeroAxis();

private:
	Ui::Tool4ZeroAxisClass ui;

	NCMachine* m_ncMachine;
	QList<char> m_axisNames;

private slots:
	void on_btnSetXZero_clicked();
	void on_btnSetYZero_clicked();
	void on_btnSetZZero_clicked();
};
