#include "runautomulti.h"
#include <QHeaderView>
#include <QComboBox>
#include <QCompleter>
#include <QToolButton>
#include <QDir>
#include <QHash>
#include <QPluginLoader>
#include "widgets/qlineedit4axisvalue.h"
#include "BaseMainWindow.h"
#include "modbus/NCMachineParametersC.h"
//#include "plugins/ipointgenerator.h"
//#include "plugins/pointgenratorbygrid.h"
#include "utils/dataform.h"
#include "modbus/NCMachine.h"
#include "widgets/multibuttonswidget.h"
#include "widgets/filedialog.h"
#include "utils/nfile.h"
#include "utils/formutils.h"

RunAutoMulti::RunAutoMulti(QWidget* parent)
	: RunManualMulti(parent, "runautomulti")
{
	m_name4RunAutoOne = "runautoone";
	
	mbw->removeButton(5);
	mbw->removeButton(4);

	ui.gridLayout->removeWidget(ui.label_1);
	ui.gridLayout->removeWidget(ui.btnJghs);
	ui.gridLayout->removeWidget(ui.btnJghs);
	ui.gridLayout->removeWidget(ui.btnJghs);
	ui.gridLayout->removeWidget(ui.label_5);
	ui.gridLayout->removeItem(ui.horizontalLayout);
	ui.label_1->hide();
	ui.btnJghs->hide();
	ui.btnJgff->hide();
	ui.btnJgff2->hide();
	ui.label_5->hide();

	this->table3->hideColumn(6);
	this->table3->hideColumn(7);
}

RunAutoMulti::~RunAutoMulti()
{
}

void RunAutoMulti::showEvent(QShowEvent* event)
{
	BaseChildWindow::showEvent(event);

	//if (m_tool4ZeroAndHalfAxis == NULL) {
	//	m_tool4ZeroAndHalfAxis = new Tool4ZeroAndHalfAxis(ncMachine, this, TOOL_LOAD_SAVE | TOOL_GCODE);
	//	Tool4ZeroAndHalfAxis::ShowIt(m_tool4ZeroAndHalfAxis);
	//}
}