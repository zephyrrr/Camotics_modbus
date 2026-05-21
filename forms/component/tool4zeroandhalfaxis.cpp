#include "tool4zeroandhalfaxis.h"
#include "tool4zeroaxis.h"
#include "tool4halfaxis.h"
#include "tool4switchcoor.h"
#include "tool4loadsave.h"
#include "tool4posset.h"
#include "tool4coorset.h"
#include "tool4gcode.h"
#include "tool4itercoor.h"
#include "tool4buttons.h"

Tool4All::Tool4All(NCMachine* ncMachine, QWidget *parent, int type)
	: QWidget(parent), m_ncMachine(ncMachine)
{
	ui.setupUi(this);

	QToolWidget* item = nullptr;
	if ((type & TOOL_LOAD_SAVE) == TOOL_LOAD_SAVE)
	{
		item = new Tool4LoadSave(qobject_cast<BaseChildWindow*>(parent), this);
		AddItem(item);
	}
	if ((type & TOOL_ZERO_AXIS) == TOOL_ZERO_AXIS)
	{
		item = new Tool4ZeroAxis(ncMachine, this);
		AddItem(item);
	}
	if ((type & TOOL_HALF_AXIS) == TOOL_HALF_AXIS)
	{
		item = new Tool4HalfAxis(ncMachine, this);
		AddItem(item);
	}
	if ((type & TOOL_SWITCH_COOR) == TOOL_SWITCH_COOR)
	{
		item = new Tool4SwitchCoor(ncMachine, this);
		AddItem(item);
	}
	
	if ((type & TOOL_POS_SET) == TOOL_POS_SET)
	{
		item = new Tool4PosSet(ncMachine, this);
		AddItem(item);
	}
	if ((type & TOOL_COOR_SET) == TOOL_COOR_SET)
	{
		item = new Tool4CoorSet(ncMachine, this);
		AddItem(item);
	}
	if ((type & TOOL_GCODE) == TOOL_GCODE)
	{
		item = new Tool4GCode(qobject_cast<BaseChildWindow*>(parent), this);
		AddItem(item);
	}
	if ((type & TOOL_COOR_ITER) == TOOL_COOR_ITER)
	{
		item = new Tool4IterCoor(qobject_cast<BaseChildWindow*>(parent), this);
		AddItem(item);
	}

	{
		QList<Tool4Buttons*> items2 = Tool4Plugins::create(qobject_cast<BaseChildWindow*>(parent), this);
		if (items2.count() > 0) {
			for each(Tool4Buttons * item2 in items2)
			{
				AddItem(item2);
			}
		}
	}

	ui.verticalLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

Tool4All::~Tool4All()
{}

void Tool4All::AddItem(QToolWidget* item)
{
	m_tools.append(item);
	ui.verticalLayout->addWidget(item);
}

QPushButton* Tool4All::AddButton(const QString& caption)
{
	Tool4Buttons* selectItem = NULL;
	for each(QToolWidget* item in m_tools)
	{
		selectItem = qobject_cast<Tool4Buttons*>(item);
		if (selectItem != NULL && selectItem->getCount() < MAX_BUTTONS_IN_TOOL)
		{
			break;
		}
		else
		{
			selectItem = NULL;
		}
	}
	if (selectItem == NULL) {
		selectItem = new Tool4Buttons(this);
		AddItem(selectItem);
	}
	QPushButton* btn = selectItem->addButton(caption);
	ShowIt(this);
	return btn;
}

void Tool4All::ShowIt(Tool4All* tool, int width, int height, int x, int y)
{
	tool->setObjectName("tool4ZeroAndHalfAxis");
	//tool->setParent(this);

	//int cnt = tool->ui.verticalLayout->count() - 1;
	int cnt = 0;
	for (QToolWidget* tool : tool->m_tools)
	{
		cnt += tool->GetHeightCount();
	}
	if (width > 0 && height > 0)
	{
		tool->setGeometry(0, 0, width, height);
	}
	else
	{
		tool->setGeometry(0, 0, 380, 60 * cnt);
	}
	if (x > 0 && y > 0)
	{
		tool->move(x, y);
	}
	else
	{
		QSize mainWindowSize = tool->window()->size();
		tool->move(mainWindowSize.width() * 870 / 1920, 50);
	}
	//tool->adjustSize();
	tool->setStyle(tool->style());
	tool->show();
}

void Tool4All::UpdateState()
{
	// iterate m_tools
	for (QToolWidget* tool : m_tools)
	{
		tool->UpdateState();
	}
}


