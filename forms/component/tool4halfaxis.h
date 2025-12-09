#pragma once

#include <QWidget>
#include "ui_tool4halfaxis.h"
#include "tool4base.h"

class NCMachine;

class Tool4HalfAxis : public QToolWidget
{
	Q_OBJECT

public:
	Tool4HalfAxis(NCMachine* ncMachine, QWidget *parent = nullptr, QList<char> axisNames = QList<char>());
	~Tool4HalfAxis();

private:
	Ui::Tool4HalfAxisClass ui;

	NCMachine* m_ncMachine;
	QList<char> m_axisNames;
private slots:

	void on_btnSetXHalf_clicked();
	void on_btnSetYHalf_clicked();
	void on_btnSetZHalf_clicked();
};
