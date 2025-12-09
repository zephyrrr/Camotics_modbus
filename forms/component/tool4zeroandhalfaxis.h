#pragma once

#include <QWidget>
#include <QList>
#include <QPushButton>
#include "ui_tool4zeroandhalfaxis.h"
#include "tool4base.h"

class NCMachine;

enum  Tool4Type { 
	TOOL_ZERO_AXIS = 2,
	TOOL_HALF_AXIS = 4,
	TOOL_SWITCH_COOR = 1,
	TOOL_LOAD_SAVE = 8,
	TOOL_POS_SET = 16,
	TOOL_GCODE = 32,
	TOOL_COOR_SET = 64,
	TOOL_COOR_ITER = 128,
};

class Tool4All : public QWidget
{
	Q_OBJECT

public:
	Tool4All(NCMachine* ncMachine, QWidget *parent = nullptr, int type= TOOL_ZERO_AXIS | TOOL_HALF_AXIS);
	~Tool4All();

	void AddItem(QToolWidget* item);
	QPushButton* AddButton(const QString& caption);
	static void ShowIt(Tool4All* tool, int width = 0, int height = 0, int x = 0, int y = 0);

	void UpdateState();
private:
	Ui::Tool4AllClass ui;

	NCMachine* m_ncMachine;
	QList<QToolWidget*> m_tools;
};

class Tool4AllWrapper : public QObject {

	Q_OBJECT

public Q_SLOTS:
	void setObjectName(Tool4All* o, const QString& name) { o->setObjectName(name); }

	void AddItem(Tool4All* o, QToolWidget* tool) { o->AddItem(tool); }
	//void removeButton(MultiButtonsWidget* o, int idx) { return o->removeButton(idx); }
	QPushButton* AddButton(Tool4All* o, const QString& caption) { return o->AddButton(caption); }
};